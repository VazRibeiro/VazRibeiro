#ifndef CAN_GATEWAY_NODE_HPP
#define CAN_GATEWAY_NODE_HPP

#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <can_msgs/msg/frame.hpp>
#include <diagnostic_updater/diagnostic_updater.hpp>
#include "can_gateway/srv/send_frame.hpp"
#include <thread>
#include <atomic>

namespace can_gateway
{
class CanGatewayNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit CanGatewayNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_configure(const rclcpp_lifecycle::State & state) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_activate(const rclcpp_lifecycle::State & state) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State & state) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_cleanup(const rclcpp_lifecycle::State & state) override;

private:
  void poll_loop();
  void handle_tx(const can_msgs::msg::Frame::SharedPtr msg);
  bool send_service(
    const std::shared_ptr<can_gateway::srv::SendFrame::Request> req,
    std::shared_ptr<can_gateway::srv::SendFrame::Response> res);
  void produce_diagnostics(diagnostic_updater::DiagnosticStatusWrapper & stat);

  int sock_{-1};
  std::string interface_;
  std::thread poll_thread_;
  std::atomic<bool> running_{false};

  rclcpp_lifecycle::LifecyclePublisher<can_msgs::msg::Frame>::SharedPtr pub_raw_;
  rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr sub_tx_;
  rclcpp::Service<can_gateway::srv::SendFrame>::SharedPtr srv_send_;
  diagnostic_updater::Updater updater_;
};
}  // namespace can_gateway

#endif  // CAN_GATEWAY_NODE_HPP
