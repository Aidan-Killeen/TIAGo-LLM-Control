#include <exception>
#include <string>   

#include <boost/shared_ptr.hpp>
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
//#include <control_msgs/GripperCommand.h>
#include <control_msgs/GripperCommand.h>
#include <ros/topic.h>

// Our Action interface type, provided as a typedef for convenience
typedef actionlib::SimpleActionClient<control_msgs::GripperCommand> gripper_control_client;
typedef boost::shared_ptr< gripper_control_client>  gripper_control_client_Ptr; 

//void createGripperClient(gripper_control_client_Ptr& actionClient)
//void createGripperClient(gripper_control_client actionClient)
//{
    /*
    ROS_INFO("Creating action client to gripper controller ...");
    actionClient.reset(new arm_control_client("/parallel_gripper_controller/follow_joint_trajectory")); ///gripper_controller/gripper_action
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
        throw std::runtime_error("Error in createGripperClient: gripper controller action server not available");
    }
    */


//}
class Gripper
{
    private:
        int MIN_EFFORT = 35;
        int MAX_EFFORT = 100;
        //gripper_control_client gripper_client_;

        ros::Publisher grip_pub;
        control_msgs::GripperCommand grip_command;
    public:
        Gripper()
        {
            //gripper_client_ = new GripperClient("r_gripper_controller/gripper_action", true);
            //gripper_client_.reset(new gripper_control_client("/parallel_gripper_controller/follow_joint_trajectory"));
            //gripper_client_ = new SimpleActionClient('gripper_controller/gripper_action', control_msgs::GripperCommand.action);
            ros::NodeHandle nh;
            grip_pub = nh.advertise<control_msgs::GripperCommand>("gripper_controller/gripper_action", 10000);
            //grip_pub = nh.advertise<control_msgs::GripperCommand>("gripper_controller/command", 10000);
        }
        void open()
        {           
            grip_command.position = .044;
            grip_command.max_effort = MIN_EFFORT;

            grip_pub.publish(grip_command);
            
        }

        void close()
        {
            grip_command.position = .004;
            grip_command.max_effort = MIN_EFFORT;

            grip_pub.publish(grip_command);
        }
};
/*
class Gripper{
private:
  GripperClient* gripper_client_;  

public:
  //Action client initialization
  Gripper(){

    //Initialize the client for the Action interface to the gripper controller
    //and tell the action client that we want to spin a thread by default
    gripper_client_ = new GripperClient("r_gripper_controller/gripper_action", true);
    
    //wait for the gripper action server to come up 
    while(!gripper_client_->waitForServer(ros::Duration(5.0))){
      ROS_INFO("Waiting for the r_gripper_controller/gripper_action action server to come up");
    }
  }

  ~Gripper(){
    delete gripper_client_;
  }

  //Open the gripper
  void open(){
    pr2_controllers_msgs::Pr2GripperCommandGoal open;
    open.command.position = 0.08;
    open.command.max_effort = -1.0;  // Do not limit effort (negative)
    
    ROS_INFO("Sending open goal");
    gripper_client_->sendGoal(open);
    gripper_client_->waitForResult();
    if(gripper_client_->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("The gripper opened!");
    else
      ROS_INFO("The gripper failed to open.");
  }

  //Close the gripper
  void close(){
    pr2_controllers_msgs::Pr2GripperCommandGoal squeeze;
    squeeze.command.position = 0.0;
    squeeze.command.max_effort = 50.0;  // Close gently
    
    ROS_INFO("Sending squeeze goal");
    gripper_client_->sendGoal(squeeze);
    gripper_client_->waitForResult();
    if(gripper_client_->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("The gripper closed!");
    else
      ROS_INFO("The gripper failed to close.");
  }
};
*/
int gripper_test(){
  //ros::init(argc, argv, "simple_gripper");

    /*
  Gripper gripper;

  gripper.open();
  gripper.close();

  return 0;
  */

    ROS_INFO_STREAM( "Starting gripper test...") ;

    ros::NodeHandle nh;
    if (!ros::Time::waitForValid(ros::WallDuration(10.0)))
    {
        ROS_FATAL("Timed-out waiting for valid time.");
        return EXIT_FAILURE;
    }


    
    //gripper_control_client_Ptr GripperClient;

    //control_msgs::GripperCommand base_var;

    Gripper gripper;
    gripper.close();
    //gripper_control_client GripperClient;
    
    //createGripperClient(GripperClient);
    /*
    ROS_INFO_STREAM( "Gripper test complete!") ;
    */
    ROS_INFO_STREAM( "Gripper test  complete!") ;
    return EXIT_SUCCESS;
}
