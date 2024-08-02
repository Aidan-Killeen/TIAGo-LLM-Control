#include <ros/ros.h>

//Neeeded for base controls
//#include <geometry_msgs/Twist.h>

//#include "arm_controls.cpp" //fix this- create a .h file
#include "gripper_controls.cpp"
#include "std_msgs/String.h"

//#include "base_controls.cpp"





int main (int argc , char ** argv)
{
    ros::init(argc, argv , "ROS_controls");
    ros::NodeHandle nh;
    ROS_INFO("Starting ROS_controls application ...");
    //ros::Duration(1).sleep();
    

    //ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter", 1000);
    ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("control_msgs", 1000);

    ros::Duration(1).sleep();

    std_msgs::String msg;
    std::stringstream ss;
    ss << "3";// << count;
    //Currently, arm_client is broken - something is causing the  node to stop runnning


    msg.data = ss.str();

    chatter_pub.publish(msg);
    ROS_INFO("%s", msg.data.c_str());
    ROS_INFO("Message Sent!");
    
    //ros::NodeHandle nh;

    //gripper_test();   //Not working currently, causes error
    //arm_joint_test(); //Currently works
    //base_forward(5);  // change in structure,, no longer is imported
    ros:: spin();
}
