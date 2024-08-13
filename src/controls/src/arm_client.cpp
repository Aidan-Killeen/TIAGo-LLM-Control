//Needed for joint controls


#include <exception>
#include <string>   

#include <boost/shared_ptr.hpp>
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <ros/topic.h>

//Moveit - also needs string and ros, but nothing else
#include <moveit/move_group_interface/move_group_interface.h>
#include <vector>
#include <map>

//subscriber
#include "std_msgs/String.h"
#include "controls/arm_position.h"


//typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> arm_control_client;
//typedef boost::shared_ptr< arm_control_client>  arm_control_client_Ptr;

//typedef actionlib::SimpleActionClient<pr2_controllers_msgs::Pr2GripperCommandAction> GripperClient;


int move_it_arm(std::map<std::string, double> target_position)
{
    
    if ( target_position.size() < 8 )
    {

        ROS_INFO("\tmove_it_joint called on map with invalid number of elements");
        ROS_INFO(" ");
        return EXIT_FAILURE;
    }

    //ros::NodeHandle nh;
    ros::AsyncSpinner spinner(1);
    spinner.start();

    std::vector<std::string> torso_arm_joint_names;
    //select group of joints
    //moveit::planning_interface::MoveGroup group_arm_torso("arm_torso");
    moveit::planning_interface::MoveGroupInterface group_arm_torso("arm_torso");
    //choose your preferred planner
    group_arm_torso.setPlannerId("SBLkConfigDefault");

    torso_arm_joint_names = group_arm_torso.getJoints();

    group_arm_torso.setStartStateToCurrentState();
    group_arm_torso.setMaxVelocityScalingFactor(1.0);

    for (unsigned int i = 0; i < torso_arm_joint_names.size(); ++i)
        if ( target_position.count(torso_arm_joint_names[i]) > 0 )
        {
            ROS_INFO_STREAM("\t" << torso_arm_joint_names[i] << " goal position: " << target_position[torso_arm_joint_names[i]]);
            group_arm_torso.setJointValueTarget(torso_arm_joint_names[i], target_position[torso_arm_joint_names[i]]);
        }
    ROS_INFO("Planning start...");
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    //moveit::planning_interface::MoveGroup::Plan my_plan;
    group_arm_torso.setPlanningTime(5.0);
    //bool success = group_arm_torso.plan(my_plan);
    bool success = static_cast<bool>(group_arm_torso.plan(my_plan));

    if ( !success )
        throw std::runtime_error("No plan found");

    ROS_INFO_STREAM("Plan found in " << my_plan.planning_time_ << " seconds");

    // Execute the plan
    ros::Time start = ros::Time::now();

    group_arm_torso.move();

    ROS_INFO_STREAM("Motion duration: " << (ros::Time::now() - start).toSec());

    spinner.stop();

    return EXIT_SUCCESS;

}

void motionCallback(const controls::arm_position::ConstPtr& msg)
{
    ROS_INFO("Message Recieved");


    std::map<std::string, double> target_position;

    target_position["torso_lift_joint"] = msg->torso_lift_joint;
    target_position["arm_1_joint"] = msg->arm_1_joint;
    target_position["arm_2_joint"] = msg->arm_2_joint;
    target_position["arm_3_joint"] = msg->arm_3_joint;
    target_position["arm_4_joint"] = msg->arm_4_joint;
    target_position["arm_5_joint"] = msg->arm_5_joint;
    target_position["arm_6_joint"] = msg->arm_6_joint;
    target_position["arm_7_joint"] = msg->arm_7_joint;

    move_it_arm(target_position);
    
    
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "arm_controls");
    ros::NodeHandle nh;

    ROS_INFO("Initializing arm_controls node ...");

    
    if (!ros::Time::waitForValid(ros::WallDuration(10.0))) // NOTE: Important when using simulated clock
    {
        ROS_FATAL("Timed-out waiting for valid time.");
        return EXIT_FAILURE;
    }
    

    ros::Subscriber sub = nh.subscribe("control_msgs",1000, motionCallback);

    ROS_INFO("Waiting for Msg ...");

    ros::Rate rate(2);
    while(ros::ok())
    {
        ros::spinOnce();
        rate.sleep();
    }
    //ros:: spin();
    ROS_INFO("Error");
} 