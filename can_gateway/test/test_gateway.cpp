#include <gtest/gtest.h>
#include "can_gateway/can_gateway_node.hpp"

TEST(CanGateway, construct)
{
  rclcpp::init(0, nullptr);
  auto node = std::make_shared<can_gateway::CanGatewayNode>();
  ASSERT_NE(node, nullptr);
  rclcpp::shutdown();
}
