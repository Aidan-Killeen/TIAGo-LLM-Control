#include <ros/ros.h>

#include <exception>
#include <string>   

//Neeeded for base controls
#include <geometry_msgs/Twist.h>

#include <iostream>

double pi = 3.14;


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


int base_rotate(int sec)
{
    ROS_INFO_STREAM( "Starting rotation setup") ;
    ros::NodeHandle nh;
    ros::Publisher base_pub = nh.advertise<geometry_msgs::Twist>("mobile_base_controller/cmd_vel", 10000);
    int hz = 10;
    ros::Rate rate(hz);  //rate is in hz - units per second
    int i = 0;
    geometry_msgs::Twist base_var;
    while(ros::ok() && i < hz*sec)
    {
        
        //base_var.linear.x = .5;
        base_var.angular.z = pi/8;      //Radians per second -   2 * pi for full rotation, current speed makes 1/8 rotation/sec
        base_pub.publish(base_var);

        ROS_INFO_STREAM( "Sending motion") ;
        i++;
        rate.sleep();
    }

    base_var.angular.z = 0;      //Radians per second -   2 * pi for full rotation 

    base_pub.publish(base_var);

    ROS_INFO_STREAM( "Turning motion complete") ; 
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
    //set up listener instead
    //base_rotate(4);

    ROS_DEBUG("Test Complete!");
    ros:: spin();
} 