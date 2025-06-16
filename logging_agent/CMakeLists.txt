cmake_minimum_required(VERSION 3.8)
project(logging_agent)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(can_msgs REQUIRED)

add_executable(logging_agent_node src/logging_agent.cpp)
ament_target_dependencies(logging_agent_node rclcpp can_msgs)

install(TARGETS logging_agent_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
