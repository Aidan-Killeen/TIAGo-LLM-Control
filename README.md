# TIAGo-LLM-Control
This code lets the user control a THIAGo Robot by using a LLM to interface with ROS

Before this code is launched, a TIAGo simulation should already be running eg
    roslaunch tiago_gazebo tiago_gazebo.launch public_sim:=true tuck_arm:=false end_effector:=pal-gripper world:=empty
    Note: ensure the simulation has "tuck_arm:=false", to avoid having to wait for the motion to finish playing

To run, first run catkin_make in the terminal with this directory open, then run "source devel/setup.bash"
Then run "roslaunch controls run.launch"
Ensure roscore has been opened in a different terminal first
