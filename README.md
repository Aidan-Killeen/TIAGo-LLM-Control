# TIAGo-LLM-Control
This code is intended to let the user control a TIAGo Robot by using a LLM to interface with ROS. In it's current state, it allows for the usage of Voice Recognition to control the robot using a series of keywords, which are displayed by a UI

## Installation
### Prequisites
1. Ubuntu 20.04 OS
    - If trying to run this code on a Windows machine, [VirtualBox](https://www.virtualbox.org/) is a good VM to run this on
2. [Install ROS](http://wiki.ros.org/Robots/TIAGo/Tutorials/Installation/InstallUbuntuAndROS)
    - The Version installed is noetic, which is being run on a Ubuntu 20.04 Virtual Machine
3. Install pip
    - In order for the Dependencies of this package to be installed, pip needs to be installed beforehand. Run the following in a terminal in Ubuntu:
        > sudo apt install python3-pip

### Building Code
1. Before running code the first time
    1. Installing Dependencies using [rosdep](http://wiki.ros.org/rosdep)
        - To install the dependencies that this package needs in order to run properly:
            > rosdep install --from-paths src --ignore-src -r -y
        - Note that you can troubleshoot error messages for dependencies needed for the package by running:
            > rosdep check --from-paths src --ignore-src -r -y
        - If you do not have pip installed, this step will fail
    2. Using catkin
        - To build the code, run:
            > catkin_make
2. After Changing the code
    - The code needs to be rebuilt to reflect updates. Run:
        > catkin_make

## Running Code
1. Launch a [TIAGo Simulation](http://wiki.ros.org/Robots/TIAGo/Tutorials/Installation/Testing_simulation) or connect to the physical robot. Below is an adjusted command to raise the simulation, with a breakdown of why it should be used.
    - Simulation Example: 
        > roslaunch tiago_2dnav_gazebo tiago_mapping.launch public_sim:=true end_effector:=pal-gripper rviz:=false tuck_arm:=false world:=empty
        - You should use tiago_2dnav_gazebo, as the default tiago_gazebo does not advertise the /scan topic, which is required to prevent collisions.
        - Optional: "rviz:=false" - prevents rviz from being launched, which is used to create a map based off of the sensors.
        - Optional: "tuck_arm:=false", to avoid having to wait for the motion to finish playing when first launching
2. Launch this code
    - Each time a new terminal is opened, you need to first run the following for ros to locate your package:
        > source devel/setup.bash
    - Otherwise, you just have to run
        > roslaunch controls run.launch
        - Upon Launching this code, you have a prompt that asks you if you want to run the code in Safe mode
            - Safe Mode prevents voice commands from being available, as the movements they call up do not have safeties built into them properly
        - After this, an infinite loop shall occur, that will prompt you with various potential voice commands
            - You may quit this by speaking "quit", or pressing <kbd>Ctrl</kbd> + <kbd>C</kbd> on the keyboard


## Optional: Troubleshooting node interactions
It is possible to run some aspects of the code without having to launching a TIAGo simulation. This is only suggested for troubleshooting interactions between different nodes
1. Launch roscore in a terminal, and leave it running
    > roscore
2. In a new terminal, 
    > source devel/setup.bash
    > rosrun controls NODE_NAME

The current valid values for NODE_NAME are base_client.py, voice_controls.py, and arm_controls
