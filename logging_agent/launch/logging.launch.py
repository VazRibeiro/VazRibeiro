from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='logging_agent',
            executable='logging_agent_node',
            name='logging_agent',
            output='screen')
    ])
