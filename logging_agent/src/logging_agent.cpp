#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("logging_agent");
  RCLCPP_INFO(node->get_logger(), "logging agent started");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
