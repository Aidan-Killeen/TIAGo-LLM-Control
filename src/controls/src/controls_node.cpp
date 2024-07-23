#include <ros/ros.h>

//Neeeded for base controls
#include <geometry_msgs/Twist.h>

#include "arm_controls.h"





int main (int argc , char ** argv)
{
    ros::init(argc, argv , "ROS_controls");
    ros::NodeHandle nh;

    test();
    ros::Publisher base_pub = nh.advertise<geometry_msgs::Twist>("mobile_base_controller/cmd_vel", 10000);//need the node name - should match rostopic list, without leading /'teleop_twist_keyboard'
    //ros::Publisher arm_pub = nh.advertise<geometry_msgs::Twist>("controller_manager/arm_controller", 10000);

    ros::Rate rate(3);  //rate is in hz - units per second
    while(ros::ok())
    {
        geometry_msgs::Twist base_var;
        base_var.linear.x = .5;
        //msg.angular.x = 0;

        base_pub.publish(base_var);

        ROS_INFO_STREAM( "Sending motion") ;
        rate.sleep();
    }

    //ROS_INFO_STREAM( "Hello, ROS!") ;
}