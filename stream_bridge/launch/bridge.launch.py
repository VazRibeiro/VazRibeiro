from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='stream_bridge',
            executable='stream_bridge_node',
            name='stream_bridge',
            output='screen')
    ])
