# TIAGo-LLM-Control
This code lets the user control a THIAGo Robot by using a LLM to interface with ROS

Before this code is lauched, a TIAGo simulation should be lauched first
    Note: ensure tthe sinulation has "tuck_arm:=false", to avoid having to wait for the motion to finish playing

To run, first run catkin_make in the terminal with this directory open, then run "source devel/setup.bash"
Then run "rosrun controls node"
Ensure roscore has been opened in a different terminal first
