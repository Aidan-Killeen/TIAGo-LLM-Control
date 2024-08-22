import rospy
from nav_msgs.msg import Odometry

from tf.transformations import euler_from_quaternion, quaternion_from_euler
from geometry_msgs.msg import Twist
from controls.msg import Turn
from std_msgs.msg import Bool
from std_msgs.msg import Int32
import math

roll = pitch = yaw = 0.0
clockwise = True
target = 0

def unpause(pause_pub):
    #pub = rospy.Publisher('controls_pause', Bool, queue_size=10)
    msg = Bool()
    msg.data = False
    pause_pub.publish(msg)
    print("Voice rec unpaused")

def get_rotation (msg):
    global roll, pitch, yaw
    orientation_q = msg.pose.pose.orientation
    orientation_list = [orientation_q.x, orientation_q.y, orientation_q.z, orientation_q.w]
    (roll, pitch, yaw) = euler_from_quaternion (orientation_list)
    #print(yaw)

def start_rotation(msg):
    rot_msg =Twist()
    pub = rospy.Publisher('mobile_base_controller/cmd_vel', Twist, queue_size=10)
    pause_pub = rospy.Publisher('controls_pause', Bool, queue_size=10)

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

    #Resolving errors from transition between pos and negative
    clock_targ = target_rad + 2*math.pi
    anti_targ = target_rad - 2*math.pi

    r = rospy.Rate(10)
    i = 0
    while not rospy.is_shutdown():
        diff1 = target_rad - yaw
        diff2 = clock_targ - yaw
        diff3 = anti_targ - yaw

        diff = min([diff1, diff2, diff3], key=abs)


        if (i% 10 == 0):
            rot_msg.angular.z = min([diff, max_rot], key=abs)  #set some way to guarantee which diirection it is going by setting +/-
        pub.publish(rot_msg)
        print(f"target: {target_rad} current: {yaw} diff{diff}")

        i+=1
        r.sleep()
        if (abs(diff) < .01):
            break
        #Edge cases - transition between positiive and negative values
        
        # if (abs(target_rad) == math.pi and abs(-target_rad-yaw) < .01):
        #     break
    
    rot_msg.angular.z = 0  #set some way to guarantee which diirection it is going by setting +/-
    pub.publish(rot_msg)
    unpause(pause_pub)
    return

def start_mov(msg):
    sec = msg.data

    pub = rospy.Publisher('mobile_base_controller/cmd_vel', Twist, queue_size=10)
    pause_pub = rospy.Publisher('controls_pause', Bool, queue_size=10)
    mov_msg = Twist()

    speed = .5
    hz = 10
    i = 0
    r = rospy.Rate(hz)

    while not rospy.is_shutdown() and i < hz*sec:
        mov_msg.linear.x = speed
        pub.publish(mov_msg)
        r.sleep()
        i+=1
    
    mov_msg.linear.x = 0
    pub.publish(mov_msg)
    unpause(pause_pub)

def rotation():
    rospy.init_node('rotation_controls')

    sub = rospy.Subscriber ('mobile_base_controller/odom', Odometry, get_rotation)
    sub2 = rospy.Subscriber ('rot_input', Turn, start_rotation)
    sub3 = rospy.Subscriber ('mov_input', Int32, start_mov)

    # rospy.sleep(1)
    # msg = Turn()
    # msg.clockwise = True
    # msg.rad_rotate = math.pi/2
    # start_rotation(msg)
    rospy.spin()

if __name__ == "__main__":
    try:
        rotation()
    except rospy.ROSInterruptException:
        pass