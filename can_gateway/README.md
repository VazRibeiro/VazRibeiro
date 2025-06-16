# CAN Gateway

ROS2 Lifecycle node that bridges a SocketCAN interface to ROS topics.

## Build

```
source /opt/ros/humble/setup.bash
colcon build --packages-select can_gateway
```

## Usage with vcan

```
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
ros2 launch can_gateway gateway.launch.py interface:=vcan0
```
