import rospy
from nav_msgs.msg import Odometry

from tf.transformations import euler_from_quaternion, quaternion_from_euler
from geometry_msgs.msg import Twist
from controls.msg import Turn
import math

roll = pitch = yaw = 0.0
clockwise = True
target = 0

def get_rotation (msg):
    global roll, pitch, yaw
    orientation_q = msg.pose.pose.orientation
    orientation_list = [orientation_q.x, orientation_q.y, orientation_q.z, orientation_q.w]
    (roll, pitch, yaw) = euler_from_quaternion (orientation_list)
    #print(yaw)

def start_rotation(msg):
    rot_msg =Twist()
    pub = rospy.Publisher('mobile_base_controller/cmd_vel', Twist, queue_size=10)

    global clockwise
    clockwise = msg.clockwise
    rotate = msg.rad_rotate
    target_rad = 0
    max_rot = .5
    if clockwise:
        target_rad = yaw - rotate
        max_rot = -max_rot
    else:
        target_rad = yaw + rotate

    r = rospy.Rate(10)
    i = 0
    while not rospy.is_shutdown():
        diff = target_rad - yaw

        if (i% 10 == 0):
            rot_msg.angular.z = min([diff, max_rot], key=abs)  #set some way to guarantee which diirection it is going by setting +/-
        pub.publish(rot_msg)
        print(f"target: {target_rad} current: {yaw}")

        i+=1
        r.sleep()
        if (abs(target_rad-yaw) < .01):
            break

    rot_msg.angular.z = 0  #set some way to guarantee which diirection it is going by setting +/-
    pub.publish(rot_msg)

    return

def rotation():
    rospy.init_node('rotation_controls')

    sub = rospy.Subscriber ('mobile_base_controller/odom', Odometry, get_rotation)
    sub2 = rospy.Subscriber ('rot_input', Turn, start_rotation)

    rospy.sleep(1)
    msg = Turn()
    msg.clockwise = True
    msg.rad_rotate = math.pi/2
    start_rotation(msg)

if __name__ == "__main__":
    try:
        rotation()
    except rospy.ROSInterruptException:
        pass