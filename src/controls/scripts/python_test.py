import rospy
import pathlib
import ctypes

rospy.loginfo("Testing Start")
#print("Testing Start")


if __name__ == "__main__":
    print("Testing Start")
    rospy.init_node("ROS_controls")
    rospy.logdebug("Testing Start")
    rospy.loginfo("Testing Start")
    #libname = pathlib.Path().absolute() / "libbase_controls.so"
    #c_lib = ctypes.CDLL(libname)

    #c_lib.base_forward(5)

    print("Testing End")
