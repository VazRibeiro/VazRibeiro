#include "stream_bridge/bridge.hpp"
#include <uWS/uWS.h>

namespace stream_bridge
{

Bridge::Bridge()
{
  node_ = rclcpp::Node::make_shared("stream_bridge");
}

void Bridge::run()
{
  hub_.onConnection([this](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest const &){
    RCLCPP_INFO(node_->get_logger(), "client connected");
  });
  hub_.listen(9002);
  hub_.run();
}

}  // namespace stream_bridge

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  stream_bridge::Bridge b;
  b.run();
  rclcpp::shutdown();
  return 0;
}
