#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>
#include <turtlesim/Pose.h>
#include <assignment1_rt/distance.h>  // Include the custom message for distance
#include <cmath>
#include <vector>
#include <algorithm>

// Declare global publishers and position variables
ros::Publisher pub_turtle1;
ros::Publisher pub_turtle2;
ros::Publisher pub_distance;

// Store the position of turtle1 globally
float turtle1_x, turtle1_y;

// Distance threshold for stopping turtles
float distance_threshold = 1.0;
float obstacle_threshold = 0.5;  // Threshold distance to obstacles

// Store obstacle distances
std::vector<double> obstacle_distances(16, std::numeric_limits<double>::infinity());

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
    pub_distance.publish(dist_msg);

    // Check for obstacle proximity
    if (*std::min_element(obstacle_distances.begin(), obstacle_distances.end()) < obstacle_threshold) {
        ROS_WARN("Stopping turtle2 due to nearby obstacle.");
        geometry_msgs::Twist stop_msg;
        stop_msg.linear.x = 0.0;
        stop_msg.angular.z = 0.0;
        pub_turtle2.publish(stop_msg);
        return;
    }

    // Default behavior if no obstacle is too close
    geometry_msgs::Twist move_msg;
    move_msg.linear.x = (distance > distance_threshold) ? 1.0 : 0.0;
    move_msg.angular.z = 0.0;
    pub_turtle2.publish(move_msg);
}

void obstaclesCallback(const std_msgs::Float32MultiArray::ConstPtr& msg) {
    // Update obstacle distances
    if (msg->data.size() == 16) {
        obstacle_distances = msg->data;
    } else {
        ROS_ERROR("Received obstacle data of unexpected size.");
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "distance_node_with_obstacles");
    ros::NodeHandle nh;

    // Publishers
    pub_turtle1 = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
    pub_turtle2 = nh.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 10);
    pub_distance = nh.advertise<assignment1_rt::distance>("/distance", 10);

    // Subscribers
    ros::Subscriber sub_turtle1 = nh.subscribe("/turtle1/pose", 10, turtle1PoseCallback);
    ros::Subscriber sub_turtle2 = nh.subscribe("/turtle2/pose", 10, turtle2PoseCallback);
    ros::Subscriber sub_obstacles = nh.subscribe("/obstacles", 10, obstaclesCallback);

    ros::spin();
    return 0;
}
