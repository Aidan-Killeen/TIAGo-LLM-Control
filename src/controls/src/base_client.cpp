#include <ros/ros.h>

#include <exception>
#include <string>   

//Neeeded for base controls
#include <geometry_msgs/Twist.h>

#include <iostream>


int base_forward(int sec)
{
    //Tuck arm before move?
    ros::NodeHandle nh;
    ros::Publisher base_pub = nh.advertise<geometry_msgs::Twist>("mobile_base_controller/cmd_vel", 10000);
    

    ROS_INFO_STREAM( "Starting forward motion") ;

    int hz = 3;
    ros::Rate rate(hz);  //rate is in hz - units per second
    int i = 0;
    while(ros::ok() && i < hz*sec)
    {
        geometry_msgs::Twist base_var;
        base_var.linear.x = .5;
        //msg.angular.x = 0;

        base_pub.publish(base_var);

        ROS_INFO_STREAM( "Sending motion") ;
        i++;
        rate.sleep();
    }
    ROS_INFO_STREAM( "Forward motion complete") ;
    return EXIT_SUCCESS;
}



int main(int argc, char** argv)
{
    ros::init(argc, argv, "base_controls");

    ROS_INFO("Initializing base_controls node ...");

    ros::NodeHandle nh;
    if (!ros::Time::waitForValid(ros::WallDuration(10.0))) // NOTE: Important when using simulated clock
    {
        ROS_FATAL("Timed-out waiting for valid time.");
        return EXIT_FAILURE;
    }

    ROS_DEBUG("Running movement Test...");

    //base_forward(5);
    //set up listener instea

    ROS_DEBUG("Test Complete!");
    ros:: spin();
} 