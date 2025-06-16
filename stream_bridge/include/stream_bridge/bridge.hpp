#ifndef STREAM_BRIDGE_BRIDGE_HPP
#define STREAM_BRIDGE_BRIDGE_HPP

#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>
#include <uWS/uWS.h>

namespace stream_bridge
{
class Bridge
{
public:
  Bridge();
  void run();
private:
  rclcpp::Node::SharedPtr node_;
  uWS::Hub hub_;
};
}  // namespace stream_bridge

#endif
