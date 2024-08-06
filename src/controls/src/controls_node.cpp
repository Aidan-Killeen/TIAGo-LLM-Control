#include <ros/ros.h>

//Neeeded for base controls
//#include <geometry_msgs/Twist.h>

//#include "arm_controls.cpp" //fix this- create a .h file
#include "gripper_controls.cpp"
#include "std_msgs/String.h"
#include "controls/arm_position.h"

#include <vector>

//#include "base_controls.cpp"





int main (int argc , char ** argv)
{
    ros::init(argc, argv , "ROS_controls");
    ros::NodeHandle nh;
    ROS_INFO("Starting ROS_controls application ...");
    //ros::Duration(1).sleep();
    
    //ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("control_msgs", 1000);
    ros::Publisher chatter_pub = nh.advertise<controls::arm_position>("control_msgs", 1000);

    ros::Duration(1).sleep();

    /*
    std_msgs::String msg;
    std::stringstream ss;
    ss << "3";// << count;
    msg.data = ss.str();
    **/

    controls::arm_position msg; //Double check data entry formatting

    msg.coords.push_back(0.0);
    msg.coords.push_back(2.5);
    msg.coords.push_back(1.0);
    msg.coords.push_back(1.0);
    msg.coords.push_back(1.0);
    msg.coords.push_back(1.0);
    msg.coords.push_back(1.0);
    msg.coords.push_back(1.0);

    msg.torso_lift_joint = 0.0;
    msg.arm_1_joint = 2.5;
    msg.arm_2_joint = 1.0;
    msg.arm_3_joint = 1.0;
    msg.arm_4_joint = 1.0;
    msg.arm_5_joint = 1.0;
    msg.arm_6_joint = 1.0;
    msg.arm_7_joint = 1.0;
    

    chatter_pub.publish(msg);
    ROS_INFO("Message Sent!");
    

    //gripper_test();   //Not working currently, causes error
    ros:: spin();
}
