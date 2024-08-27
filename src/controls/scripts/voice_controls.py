import rospy
import speech_recognition as sr
from std_msgs.msg import String
from std_msgs.msg import Bool
from std_msgs.msg import Int32
from controls.msg import arm_position
from controls.msg import Turn
import math
import threading

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

paused = False

def unpause(msg):
    global paused
    paused = msg.data
    print("Movement controls renabled:", not paused)

def UI():
    global paused
    global safe
    print("Please speak a combination of command words:\n")
    print("Available commands:")
    if(not paused):
        print("\tArm controls: test")
    if(not safe and not paused):
        print("\tGeneral Movement: forward, backward, turn")
        print("\t\tTurning modifiers: left, right, slightly, around")

    print("\tSystem: quit")







def interpreter(command, arm_pub, turn_pub, mov_pub):
    global paused
    if not paused:
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

            #rospy.loginfo(msg)
            arm_pub.publish(msg)

        if "forward" in command and not safe:
            print("Unsafe forward motion Initialised")
            # To be added to safe code, would need Sensors to be able to detect potential collision with narrow objects 
            #   (Currently poles or strings could cause issues)
            msg = Int32()
            msg.data = 2
            mov_pub.publish(msg)
            paused = True
        elif "backward" in command:
            pass
        elif "turn" in command and not safe:
            print("Unsafe Turn Initialised")
            # To be added to safe code, would have to tuck arm before rotation, or be able to detect if arm is about to collide with something
            # Alternatively, could refuse to turn if something is detected too close
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
            #pause()
            paused = True

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

    #Combine Turn and Mov?
    turn_pub = rospy.Publisher('rot_input', Turn, queue_size=1000)
    mov_pub = rospy.Publisher('mov_input', Int32, queue_size=1000)
    rospy.init_node('talker', anonymous=True)
    sub_pause = rospy.Subscriber ('controls_pause', Bool, unpause, queue_size=10)
    #thread = threading.Thread(target=spin_thread)
    #thread.start()

    #Toggle Safe Mode  
    global safe
    while True:
        mode = input("Run in Safe Mode? (y/n) ").casefold()
        if mode == "y":
            break
        elif mode == "n":
            safe = False
            break
        else:
            print("Error: you must enter 'y' or 'n'")

    rate = rospy.Rate(10)
    with mic as source:
        #i=i+1

        #with mic as source:
        while not rospy.is_shutdown() and not quit:
            i=i+1
            r.adjust_for_ambient_noise(source)
            
            try:
                UI()
                #audio = r.listen(source)
                audio = r.listen(source, 10, 5) #apparently, only one instance of each listener function can run - trying to rlaunch mid action is ignored
                print("Time over, thanks")
                # using google speech recognition
                text = r.recognize_google(audio)
                print(i, "Your voice command has been recognised as: "+ text)
                quit = interpreter(text, arm_pub, turn_pub, mov_pub)
            except Exception as e:
                print(i, "Sorry, I did not get that", e)

            rate.sleep()
            #rospy.sleep(rospy.Duration(nsecs=1000))

if __name__ == "__main__":
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
