import rospy
import speech_recognition as sr

#Extraneous Error clean up
from ctypes import *

# From alsa-lib Git 3fd4ab9be0db7c7430ebd258f2717a976381715d
# $ grep -rn snd_lib_error_handler_t
# include/error.h:59:typedef void (*snd_lib_error_handler_t)(const char *file, int line, const char *function, int err, const char *fmt, ...) /* __attribute__ ((format (printf, 5, 6))) */;

# Define our error handler type
ERROR_HANDLER_FUNC = CFUNCTYPE(None, c_char_p, c_int, c_char_p, c_int, c_char_p)

def py_error_handler(filename, line, function, err, fmt):

  #print ('messages are yummy')
  return

c_error_handler = ERROR_HANDLER_FUNC(py_error_handler)

asound = cdll.LoadLibrary('libasound.so')

# Set error handler
asound.snd_lib_error_set_handler(c_error_handler)



if __name__ == "__main__":
    print("Testing Start")


    print("Testing End")
    

    r = sr.Recognizer()
    mic = sr.Microphone()

    i = 0

    #while(True):
    with mic as source:
        #i=i+1

        #with mic as source:
        while(True):
            i=i+1
            r.adjust_for_ambient_noise(source)
            print("Talk")
            audio = r.listen(source)
            #audio = r.listen(source, 10, 3)
            print("Time over, thanks")
            try:
                # using google speech recognition
                print(i, "Text: "+r.recognize_google(audio))
            except:
                print(i, "Sorry, I did not get that")
    

