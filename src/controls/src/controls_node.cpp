#include <ros/ros.h>

//Neeeded for base controls
//#include <geometry_msgs/Twist.h>

//#include "arm_controls.cpp" //fix this- create a .h file
#include "gripper_controls.cpp"
//#include "base_controls.cpp"





int main (int argc , char ** argv)
{
    ros::init(argc, argv , "ROS_controls");
    ROS_INFO("Starting ROS_controls application ...");

    //ros::NodeHandle nh;

    //gripper_test();   //Not working currently, causes error
    //arm_joint_test(); //Currently works
    //base_forward(5);  // change in structure,, no longer is imported
}
