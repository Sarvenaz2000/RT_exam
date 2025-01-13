#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>
#include <turtlesim/Pose.h>
#include <assignment1_rt/distance.h>  // Include the custom message for distance
#include <cmath>

// Declare global publishers and position variables
ros::Publisher pub_turtle1;
ros::Publisher pub_turtle2;
ros::Publisher pub_distance;

// Store the position of turtle1 globally
float turtle1_x, turtle1_y;

// Distance threshold for stopping turtles
float threshold = 1.0;

void turtle1PoseCallback(const turtlesim::Pose::ConstPtr& msg) {
    // Store the position of turtle1
    turtle1_x = msg->x;
    turtle1_y = msg->y;
}

void turtle2PoseCallback(const turtlesim::Pose::ConstPtr& msg) {
    // Get the position of turtle2
    float x2 = msg->x;
    float y2 = msg->y;

    // Calculate the distance between turtle1 and turtle2
    float distance = std::sqrt(std::pow(x2 - turtle1_x, 2) + std::pow(y2 - turtle1_y, 2));

    // Publish the distance to a custom message type
    assignment1_rt::distance dist_msg;
    dist_msg.distance = distance;

    // Publish the custom distance message to the /turtle_distance_topic
    pub_distance.publish(dist_msg);

    // Check if turtles are too close or near boundaries
    if (distance < threshold || x2 > 10.0 || y2 > 10.0 || x2 < 1.0 || y2 < 1.0) {
        geometry_msgs::Twist stop_msg;
        pub_turtle1.publish(stop_msg);  // Stop turtle1
        pub_turtle2.publish(stop_msg);  // Stop turtle2
        ROS_WARN("Turtles are too close or near boundaries. Stopping.");
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "distance_node");
    ros::NodeHandle nh;

    // Subscribers to get poses of turtle1 and turtle2
    ros::Subscriber sub_turtle1 = nh.subscribe("/turtle1/pose", 10, turtle1PoseCallback);
    ros::Subscriber sub_turtle2 = nh.subscribe("/turtle2/pose", 10, turtle2PoseCallback);

    // Publishers for controlling turtle velocities
    pub_turtle1 = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
    pub_turtle2 = nh.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 10);

    // Publisher for publishing custom distance message (distance)
    pub_distance = nh.advertise<assignment1_rt::distance>("distance_topic", 10);

    // Spin to keep the node active
    ros::spin();

    return 0;
}

