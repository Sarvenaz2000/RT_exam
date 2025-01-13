#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    // Initialize the ROS node
    ros::init(argc, argv, "ui_node");
    ros::NodeHandle nh;

    // Give some time for turtlesim node to start and advertise services
    ROS_INFO("Waiting for turtlesim spawn service...");
    ros::Duration(1.0).sleep(); // Sleep for 1 second to allow service to be advertised

    // Spawn the second turtle (turtle2) at a new location
    ros::ServiceClient spawn_client = nh.serviceClient<turtlesim::Spawn>("spawn");
    turtlesim::Spawn spawn_srv;
    spawn_srv.request.x = 5.0;   // Set x position of turtle2
    spawn_srv.request.y = 2.0;   // Set y position of turtle2
    spawn_srv.request.theta = 0; // Set orientation of turtle2
    spawn_srv.request.name = "turtle2"; // Name of the new turtle

    if (spawn_client.call(spawn_srv))
    {
        ROS_INFO("Successfully spawned turtle2 at position ('%f', '%f')", spawn_srv.request.x, spawn_srv.request.y);
    }
    else
    {
        ROS_ERROR("Failed to spawn turtle2.");
        return 1;
    }

    // Print instructions for user input
    std::cout << "Control turtle2 using keyboard commands." << std::endl;
    std::cout << "Enter 'w', 'a', 's', 'd' to move the turtle, or 'q' to quit." << std::endl;

    // Publisher to control turtle2's velocity
    ros::Publisher pub_turtle2 = nh.advertise<geometry_msgs::Twist>"/turtle2/cmd_vel", 10);

    // User input loop
    char input;
    geometry_msgs::Twist cmd;
    while (ros::ok())
    {
        std::cin >> input;

        // Stop the turtle by default
        cmd.linear.x = 0.0;
        cmd.angular.z = 0.0;

        switch (input)
        {
        case 'w':
            cmd.linear.x = 1.0;
            break;
        case 's':
            cmd.linear.x = -1.0;
            break;
        case 'a':
            cmd.angular.z = 1.0;
            break;
        case 'd':
            cmd.angular.z = -1.0;
            break;
        case 'q':
            ROS_INFO("Exiting UI node.");
            return 0;
        default:
            ROS_WARN("Invalid input. Use 'w', 'a', 's', 'd' to move or 'q' to quit.");
            break;
        }

        pub_turtle2.publish(cmd);
    }

    return 0;
}
