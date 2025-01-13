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
        ROS_INFO("Successfully spawned turtle2 at position (5.0, 2.0)");
    }
    else
    {
        ROS_ERROR("Failed to spawn turtle2");
        return 1;
    }

    // Publishers for controlling turtle1 and turtle2
    ros::Publisher pub1 = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 10);
    ros::Publisher pub2 = nh.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);

    // Create a Twist message
    geometry_msgs::Twist cmd_msg;
    std::string selected_turtle;
    double linear_velocity, angular_velocity;

    ros::Rate loop_rate(1); // 1 Hz

    while (ros::ok())
    {
        // Display menu and get input from the user
        std::cout << "Select the turtle to control (turtle1 or turtle2): ";
        std::cin >> selected_turtle;

        if (selected_turtle == "turtle1" || selected_turtle == "turtle2")
        {
            // Get linear velocity from user
            std::cout << "Enter the linear velocity (between 0 and 2): ";
            std::cin >> linear_velocity;

            // Validate the linear velocity input
            if (linear_velocity < 0 || linear_velocity > 2)
            {
                std::cout << "Invalid velocity, please enter a value between 0 and 2." << std::endl;
                continue;
            }

            // Get angular velocity from user
            std::cout << "Enter the angular velocity (between -2 and 2): ";
            std::cin >> angular_velocity;

            // Validate the angular velocity input
            if (angular_velocity < -2 || angular_velocity > 2)
            {
                std::cout << "Invalid angular velocity, please enter a value between -2 and 2." << std::endl;
                continue;
            }

            // Set linear and angular velocities based on user input
            cmd_msg.linear.x = linear_velocity;  // Set linear velocity
            cmd_msg.angular.z = angular_velocity; // Set angular velocity

            // Publish the command for 1 second
            if (selected_turtle == "turtle1")
            {
                pub1.publish(cmd_msg);
            }
            else if (selected_turtle == "turtle2")
            {
                pub2.publish(cmd_msg);
            }

            // Wait for 1 second
            ros::Duration(1.0).sleep();

            // Stop the turtle after 1 second
            cmd_msg.linear.x = 0;
            cmd_msg.angular.z = 0;
            if (selected_turtle == "turtle1")
            {
                pub1.publish(cmd_msg);
            }
            else if (selected_turtle == "turtle2")
            {
                pub2.publish(cmd_msg);
            }

            // Prompt the user for the next command
            std::cout << "Turtle " << selected_turtle << " has stopped." << std::endl;
        }
        else
        {
            std::cout << "Invalid turtle selection. Please choose turtle1 or turtle2." << std::endl;
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

