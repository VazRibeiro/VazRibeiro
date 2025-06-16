cmake_minimum_required(VERSION 3.8)
project(can_gateway)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(can_msgs REQUIRED)
find_package(diagnostic_updater REQUIRED)

add_library(${PROJECT_NAME} SHARED
rosidl_get_typesupport_target(cpp ${PROJECT_NAME}_interfaces "rosidl_typesupport_cpp")
rosidl_generate_interfaces(${PROJECT_NAME}_interfaces srv/SendFrame.srv DEPENDENCIES can_msgs)
  src/can_gateway_node.cpp
  src/diagnostics.cpp)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp rclcpp_lifecycle can_msgs diagnostic_updater)

target_compile_features(${PROJECT_NAME} PUBLIC c_std_17 cxx_std_17)

add_executable(can_gateway_node src/main.cpp)
ament_target_dependencies(can_gateway_node rclcpp rclcpp_lifecycle)

target_link_libraries(can_gateway_node ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} can_gateway_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

install(DIRECTORY include/
  DESTINATION include)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_gateway test/test_gateway.cpp)
  target_link_libraries(test_gateway ${PROJECT_NAME})
endif()

install(DIRECTORY srv DESTINATION share/${PROJECT_NAME}/srv)
ament_package()
