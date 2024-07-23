//Needed for joint controls
#include <ros/ros.h>

#include <boost/shared_ptr.hpp>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <ros/topic.h>

typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> arm_control_client;
typedef boost::shared_ptr< arm_control_client>  arm_control_client_Ptr;

void test()
{
    ROS_INFO_STREAM( "Hello, ROS!") ;
}
