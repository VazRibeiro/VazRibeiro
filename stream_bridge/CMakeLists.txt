cmake_minimum_required(VERSION 3.8)
project(stream_bridge)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(can_msgs REQUIRED)

add_executable(stream_bridge_node src/bridge.cpp)
ament_target_dependencies(stream_bridge_node rclcpp can_msgs)

install(TARGETS stream_bridge_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch html
  DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
