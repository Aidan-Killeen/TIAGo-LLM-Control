//Needed for joint controls


#include <exception>
#include <string>   

#include <boost/shared_ptr.hpp>
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <ros/topic.h>


typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> arm_control_client;
typedef boost::shared_ptr< arm_control_client>  arm_control_client_Ptr;

//typedef actionlib::SimpleActionClient<pr2_controllers_msgs::Pr2GripperCommandAction> GripperClient;



void createArmClient(arm_control_client_Ptr& actionClient)
{
    ROS_INFO("Creating action client to arm controller ...");
    actionClient.reset(new arm_control_client("/arm_controller/follow_joint_trajectory"));    //this line causing issues in CMakee

    int iterations = 0, max_iterations = 3;
    while( !actionClient->waitForServer(ros::Duration(2.0)) && ros::ok() && iterations < max_iterations )
    {
        ROS_DEBUG("Waiting for the arm_controller_action server to come up");
        iterations++;
    }


    if(iterations == max_iterations)
    {
        throw std::runtime_error("Error in createArmClient: arm controller action server not available");
    }
    


}

/*
void createGripperClient(arm_control_client_Ptr& actionClient)
{
    ROS_INFO("Creating action client to gripper controller ...");
    actionClient.reset(new arm_control_client("/parallel_gripper_controller/follow_joint_trajectory"));
    //figure out size of this (Currently trying to use one node, but without joint name)
    //actionClient.reset(new arm_control_client("/gripper_controller/command"));

    int iterations = 0, max_iterations = 3;
    while( !actionClient->waitForServer(ros::Duration(2.0)) && ros::ok() && iterations < max_iterations )
    {
        ROS_DEBUG("Waiting for the gripper_controller_action server to come up");
        iterations++;
    }


    if(iterations == max_iterations)
    {
        throw std::runtime_error("Error in createArmClient: arm controller action server not available");
    }
    


}
*/

void waypoints_arm_goal(control_msgs::FollowJointTrajectoryGoal& goal)
{
    goal.trajectory.joint_names.push_back("arm_1_joint");
    goal.trajectory.joint_names.push_back("arm_2_joint");
    goal.trajectory.joint_names.push_back("arm_3_joint");
    goal.trajectory.joint_names.push_back("arm_4_joint");
    goal.trajectory.joint_names.push_back("arm_5_joint");
    goal.trajectory.joint_names.push_back("arm_6_joint");
    goal.trajectory.joint_names.push_back("arm_7_joint");

    int waypoints = 1;

    goal.trajectory.points.resize(waypoints);
    for(int i=0; i< waypoints; i++)
    {
        goal.trajectory.points[i].positions.resize(7);

        //set waypoint coords
        goal.trajectory.points[i].positions[0] = 2.5;
        for (int i1 = 1; i1 < 7; i1++)
        {
            goal.trajectory.points[i].positions[i1] = 1.0;
        }

        //set velocities
        goal.trajectory.points[i].velocities.resize(7);
        for (int j = 0; j < 7; ++j)
        {
            goal.trajectory.points[i].velocities[j] = 1.0;
        }

        goal.trajectory.points[i].time_from_start = ros::Duration(2.0);
    }


}
/*
void waypoints_gripper_goal(control_msgs::FollowJointTrajectoryGoal& goal)
{
    //goal.trajectory.joint_names.push_back("gripper_left_finger_joint");
    //goal.trajectory.joint_names.push_back("gripper_right_finger_joint");

    int waypoints = 1;

    goal.trajectory.points.resize(waypoints);
    for(int i=0; i< waypoints; i++)
    {
        goal.trajectory.points[i].positions.resize(1);

        //set waypoint coords - max is .044 for fully open, .001 is min in contorller, but ssafe is probably .020
        goal.trajectory.points[i].positions[0] = .020;  //make this take input from functionn input?
        //goal.trajectory.points[i].positions[1] = .020;

        //set velocities
        goal.trajectory.points[i].velocities.resize(1);
        for (int j = 0; j < 1; ++j)
        {
            goal.trajectory.points[i].velocities[j] = 1.0;
        }

        //set accelerations
        //goal.trajectory.points[i].accelerations.resize(2);
        //for (int j = 0; j < 2; ++j)
        //{
         //   goal.trajectory.points[i].accelerations[j] = 1.0;
        //}

        goal.trajectory.points[i].time_from_start = ros::Duration(2.0);
    }

    //ROS_INFO_STREAM(goal.trajectory) ;
}
**/
//gripper joint names: gripper_left_finger_joint and gripper_right_finger_joint with /parallel_gripper_controller/follow_joint_trajectory topic

int arm_joint_test()
{

    ROS_INFO_STREAM( "Starting arm joint test...") ;
    
    ros::NodeHandle nh;
    if (!ros::Time::waitForValid(ros::WallDuration(10.0)))
    {
        ROS_FATAL("Timed-out waiting for valid time.");
        return EXIT_FAILURE;
    }
    
    arm_control_client_Ptr ArmClient;
    createArmClient(ArmClient);
    
    control_msgs::FollowJointTrajectoryGoal arm_goal;
    waypoints_arm_goal(arm_goal);
    
    arm_goal.trajectory.header.stamp = ros::Time::now() + ros::Duration(1.0);
    ArmClient->sendGoal(arm_goal);

    while(!(ArmClient->getState().isDone()) && ros::ok())
    {
        ros::Duration(4).sleep(); // sleep for four seconds
    }
    
    ROS_INFO_STREAM( "Arm joint test completed") ;
    return EXIT_SUCCESS;

}

/*
int gripper_joint_test()
{

    ROS_INFO_STREAM( "Starting gripper joint test...") ;
    
    ros::NodeHandle nh;
    if (!ros::Time::waitForValid(ros::WallDuration(10.0)))
    {
        ROS_FATAL("Timed-out waiting for valid time.");
        return EXIT_FAILURE;
    }
    
    arm_control_client_Ptr GripperClient;
    createGripperClient(GripperClient);
    
    control_msgs::FollowJointTrajectoryGoal grip_goal;
    waypoints_gripper_goal(grip_goal);
    
    grip_goal.trajectory.header.stamp = ros::Time::now() + ros::Duration(1.0);
    GripperClient->sendGoal(grip_goal);

    while(!(GripperClient->getState().isDone()) && ros::ok())
    {
        ros::Duration(4).sleep(); // sleep for four seconds
    }
    
    ROS_INFO_STREAM( "Gripper joint test completed") ;
    return EXIT_SUCCESS;

}
*/