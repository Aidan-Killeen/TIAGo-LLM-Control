#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

int main (int argc , char ** argv)
{
    ros::init(argc, argv , "ROS_controls");
    ros::NodeHandle nh;


    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("mobile_base_controller/cmd_vel", 10000);//need the node name - should match rostopic list, without leading /'teleop_twist_keyboard'
    

    ros::Rate rate(2);
    while(ros::ok())
    {
        geometry_msgs::Twist msg;
        msg.linear.x = .5;
        //msg.angular.x = 0;

        pub.publish(msg);

        ROS_INFO_STREAM( "Sending motion") ;
        rate.sleep();
    }

    //ROS_INFO_STREAM( "Hello, ROS!") ;
}