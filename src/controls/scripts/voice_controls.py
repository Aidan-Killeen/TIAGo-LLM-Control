import rospy
import speech_recognition as sr
from std_msgs.msg import String
from controls.msg import arm_position
from controls.msg import Turn
import math

#Extraneous Error clean up
from ctypes import *

# From alsa-lib Git 3fd4ab9be0db7c7430ebd258f2717a976381715d
# Define our error handler type
ERROR_HANDLER_FUNC = CFUNCTYPE(None, c_char_p, c_int, c_char_p, c_int, c_char_p)

def py_error_handler(filename, line, function, err, fmt):

  #print ('messages are yummy')
  return

c_error_handler = ERROR_HANDLER_FUNC(py_error_handler)

asound = cdll.LoadLibrary('libasound.so')

# Set error handler
asound.snd_lib_error_set_handler(c_error_handler)

safe = True #Used to enable safe mode, avoiding any issues from lack of safeties

def interpreter(command, arm_pub, turn_pub):
    
    
    if "test" in command:
        print("Arm Test Initialised")
        #msg.coords = []
        msg = arm_position()
        msg.torso_lift_joint = 0.0
        msg.arm_1_joint = 2.5
        msg.arm_2_joint = 1.0
        msg.arm_3_joint = 1.0
        msg.arm_4_joint = 1.0
        msg.arm_5_joint = 1.0
        msg.arm_6_joint = 1.0
        msg.arm_7_joint = 1.0

        rospy.loginfo(msg)
        arm_pub.publish(msg)

    if "forward" in command:
        pass
    elif "backward" in command:
        pass
    elif "turn" in command and not safe:
        #Bugfix needed - turn around can cause loop because of swappinng between pos and negative numbers
            #add edge case if Target > math.pi or < math.pi
        #Safeties needed - tuck arm before turn? use IR  sensors to avoid moving?
        print("Turn Test Initialised")
        msg = Turn()

        #Rotation Direction
        if "right" in command:           
            msg.clockwise = True           
        elif "left" in command:
            msg.clockwise = False
        else:
            msg.clockwise = True

        #Amount of Rotation
        if "slight" in command:
            msg.rad_rotate = math.pi/4
        elif "around" in command:
            msg.rad_rotate = math.pi
        else:
            msg.rad_rotate = math.pi/2
        turn_pub.publish(msg)

    if "quit" in command:
        return True
    return False

def listener():
    #Speech Rec Setup
    r = sr.Recognizer()
    mic = sr.Microphone()

    i = 0
    quit = False

    #ROS Setup
    arm_pub = rospy.Publisher('control_msgs', arm_position, queue_size=1000)
    turn_pub = rospy.Publisher('rot_input', Turn, queue_size=1000)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10)

    with mic as source:
        #i=i+1

        #with mic as source:
        while not rospy.is_shutdown() and not quit:
            i=i+1
            r.adjust_for_ambient_noise(source)
            
            try:
                print("Talk")
                #audio = r.listen(source)        #this seems to pause during turning
                audio = r.listen(source, 10, 5) #apparently, only one instance of each listener function can run - trying to rlaunch mid action is ignored
                print("Time over, thanks")
                # using google speech recognition
                text = r.recognize_google(audio)
                print(i, "Your voice command has been recognised as: "+ text)
                quit = interpreter(text, arm_pub, turn_pub)
            except Exception as e:
                print(i, "Sorry, I did not get that", e)

            rate.sleep()

if __name__ == "__main__":
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
