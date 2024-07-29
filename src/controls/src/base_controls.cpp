#include <ros/ros.h>

#include <exception>
#include <string>   

//Neeeded for base controls
#include <geometry_msgs/Twist.h>

int base_test()
{
    ros::NodeHandle nh;
    ros::Publisher base_pub = nh.advertise<geometry_msgs::Twist>("mobile_base_controller/cmd_vel", 10000);

    int hz = 3;
    int time = 5;
    ros::Rate rate(3);  //rate is in hz - units per second
    int i = 0;
    while(ros::ok() && i < hz*time)
    {
        geometry_msgs::Twist base_var;
        base_var.linear.x = .5;
        //msg.angular.x = 0;

        base_pub.publish(base_var);

        ROS_INFO_STREAM( "Sending motion") ;
        i++;
        rate.sleep();
    }

    return EXIT_SUCCESS;
}