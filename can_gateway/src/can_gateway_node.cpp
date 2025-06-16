#include "can_gateway/can_gateway_node.hpp"
#include <rclcpp/rclcpp.hpp>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "can_gateway/srv/send_frame.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
using namespace std::chrono_literals;

namespace can_gateway
{

CanGatewayNode::CanGatewayNode(const rclcpp::NodeOptions & options)
: rclcpp_lifecycle::LifecycleNode("can_gateway", options)
{
  declare_parameter("interface", "can0");
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CanGatewayNode::on_configure(const rclcpp_lifecycle::State &)
{
  interface_ = get_parameter("interface").as_string();
  sock_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (sock_ < 0) {
    RCLCPP_ERROR(get_logger(), "Failed to open socket: %s", strerror(errno));
    return CallbackReturn::FAILURE;
  }
  int flags = fcntl(sock_, F_GETFL, 0);
  fcntl(sock_, F_SETFL, flags | O_NONBLOCK);
  struct ifreq ifr{};
  std::strncpy(ifr.ifr_name, interface_.c_str(), IFNAMSIZ - 1);
  if (ioctl(sock_, SIOCGIFINDEX, &ifr) < 0) {
    RCLCPP_ERROR(get_logger(), "ioctl SIOCGIFINDEX failed");
    return CallbackReturn::FAILURE;
  }
  struct sockaddr_can addr{};
  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;
  if (bind(sock_, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
    RCLCPP_ERROR(get_logger(), "bind failed");
    return CallbackReturn::FAILURE;
  }
  pub_raw_ = create_publisher<can_msgs::msg::Frame>("/can/raw", rclcpp::SensorDataQoS());
  sub_tx_ = create_subscription<can_msgs::msg::Frame>("/can/tx", rclcpp::SensorDataQoS(),
      std::bind(&CanGatewayNode::handle_tx, this, std::placeholders::_1));
  srv_send_ = create_service<can_gateway::srv::SendFrame>("send_frame",
      std::bind(&CanGatewayNode::send_service, this, std::placeholders::_1, std::placeholders::_2));
  updater_.setHardwareID(interface_);
  updater_.add("can_stats", this, &CanGatewayNode::produce_diagnostics);
  return CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CanGatewayNode::on_activate(const rclcpp_lifecycle::State &)
{
  running_.store(true);
  pub_raw_->on_activate();
  poll_thread_ = std::thread(&CanGatewayNode::poll_loop, this);
  return CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CanGatewayNode::on_deactivate(const rclcpp_lifecycle::State &)
{
  running_.store(false);
  if (poll_thread_.joinable()) {
    poll_thread_.join();
  }
  pub_raw_->on_deactivate();
  return CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
CanGatewayNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  if (sock_ >= 0) {
    close(sock_);
    sock_ = -1;
  }
  return CallbackReturn::SUCCESS;
}

void CanGatewayNode::handle_tx(const can_msgs::msg::Frame::SharedPtr msg)
{
  struct can_frame frame{};
  frame.can_id = msg->id;
  frame.can_dlc = msg->dlc;
  std::memcpy(frame.data, msg->data.data(), msg->dlc);
  send(sock_, &frame, sizeof(frame), 0);
}

bool CanGatewayNode::send_service(
  const std::shared_ptr<can_gateway::srv::SendFrame::Request> req,
  std::shared_ptr<can_gateway::srv::SendFrame::Response> res)
{
  struct can_frame frame{};
  frame.can_id = req->frame.id;
  frame.can_dlc = req->frame.dlc;
  std::memcpy(frame.data, req->frame.data.data(), req->frame.dlc);
  if (send(sock_, &frame, sizeof(frame), 0) < 0) {
    res->success = false;
    res->message = "send failed";
  } else {
    res->success = true;
    res->message = "sent";
  }
  return true;
}

void CanGatewayNode::poll_loop()
{
  struct can_frame frame{};
  rclcpp::WallRate rate(1ms);
  while (running_.load()) {
    int n = recv(sock_, &frame, sizeof(frame), 0);
    if (n == sizeof(frame)) {
      auto msg = can_msgs::msg::Frame();
      msg.id = frame.can_id;
      msg.dlc = frame.can_dlc;
      msg.data.assign(frame.data, frame.data + frame.can_dlc);
      pub_raw_->publish(msg);
    }
    updater_.force_update();
    rate.sleep();
  }
}

void CanGatewayNode::produce_diagnostics(diagnostic_updater::DiagnosticStatusWrapper & stat)
{
  // simple placeholder diagnostics
  stat.summary(diagnostic_msgs::msg::DiagnosticStatus::OK, "ok");
}

}  // namespace can_gateway
