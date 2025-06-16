from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    interface = LaunchConfiguration('interface', default='can0')
    return LaunchDescription([
        Node(
            package='can_gateway',
            executable='can_gateway_node',
            name='can_gateway',
            parameters=[{'interface': interface}],
            output='screen')
    ])

