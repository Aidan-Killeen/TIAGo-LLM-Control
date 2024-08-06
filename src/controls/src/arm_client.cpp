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

    //control_msgs::FollowJointTrajectoryGoal arm_goal;
    //ROS_INFO("I heard: [%s]", msg->data.c_str());
    ROS_INFO("Message Recieved");
    //double[] coords = msg -> coords;

    std::map<std::string, double> target_position;
    /**
    target_position["torso_lift_joint"] = msg->coords[0];//0.0;//atof(argv[1]);
    target_position["arm_1_joint"] = msg->coords[1];//2.5;//atof(argv[2]);
    target_position["arm_2_joint"] = msg->coords[2];//1.0;//atof(argv[3]);
    target_position["arm_3_joint"] = msg->coords[3];//1.0;;//atof(argv[4]);
    target_position["arm_4_joint"] = msg->coords[4];//1.0;//atof(argv[5]);
    target_position["arm_5_joint"] = msg->coords[5];//1.0;//atof(argv[6]);
    target_position["arm_6_joint"] = msg->coords[6];//1.0;//atof(argv[7]);
    target_position["arm_7_joint"] = msg->coords[7];//1.0;//atof(argv[8]);
    */
    target_position["torso_lift_joint"] = msg->torso_lift_joint;//0.0;//atof(argv[1]);
    target_position["arm_1_joint"] = msg->arm_1_joint;//2.5;//atof(argv[2]);
    target_position["arm_2_joint"] = msg->arm_2_joint;//1.0;//atof(argv[3]);
    target_position["arm_3_joint"] = msg->arm_3_joint;//1.0;;//atof(argv[4]);
    target_position["arm_4_joint"] = msg->arm_4_joint;//1.0;//atof(argv[5]);
    target_position["arm_5_joint"] = msg->arm_5_joint;//1.0;//atof(argv[6]);
    target_position["arm_6_joint"] = msg->arm_6_joint;//1.0;//atof(argv[7]);
    target_position["arm_7_joint"] = msg->arm_7_joint;//1.0;//atof(argv[8]);
    //int num = atoi(msg->data.c_str());
    //ROS_INFO_STREAM("Motion duration: " << num
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
    

    //arm_joint_test(); //basic controls
    /*
    std::map<std::string, double> target_position;
    target_position["torso_lift_joint"] = 0.0;//atof(argv[1]);
    target_position["arm_1_joint"] = 2.5;//atof(argv[2]);
    target_position["arm_2_joint"] = 1.0;//atof(argv[3]);
    target_position["arm_3_joint"] = 1.0;;//atof(argv[4]);
    target_position["arm_4_joint"] = 1.0;//atof(argv[5]);
    target_position["arm_5_joint"] = 1.0;//atof(argv[6]);
    target_position["arm_6_joint"] = 1.0;//atof(argv[7]);
    target_position["arm_7_joint"] = 1.0;//atof(argv[8]);
    move_it_arm(target_position);//using moveit instead
    */
    //set up listener instead

    ros::Subscriber sub = nh.subscribe("control_msgs",1000, motionCallback);
    //ros::Subscriber sub = nh.subscribe("chatter", 1000, motionCallback);
    //ros::Subscriber sub = nh.subscribe("chatter", 1000, chatterCallback);
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