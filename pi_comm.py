#!/usr/bin/env python
import serial,time
import pygame
# setup controller input
pygame.init()
# set display mode just to make it happy
pygame.display.set_mode((1, 1))

#joysticks = []
clock = pygame.time.Clock()
# # check all connected controllers
# for i in range(0, pygame.joystick.get_count()):
#     # create a Joystick object in our list
#     joysticks.append(pygame.joystick.Joystick(i))
#     # initialize them all
#     joysticks[-1].init()
#     # print a statement saying what the name of the controller is
#     print ("Detected joystick "),joysticks[-1].get_name(),"'"
j = pygame.joystick.Joystick(0)
j.init()
# setup button -> msg assignments.
# 0 = A, 1 = B, 2 = X, 3 = Y

msg_assn = {0:'B',1:'R',2:'L',3:'F'}
msg = ""

# define exception for pressing the quit button
class QuitException(Exception):
    pass

# start serial comm with arduino
print('Running. Press CTRL-C or Left Bumper to exit.')
with serial.Serial("/dev/ttyACM0", 9600, timeout=1) as arduino:
    time.sleep(0.1) # wait for serial to open
    if arduino.isOpen():
        print("{} connected!".format(arduino.port))
        try:
            while True:
                # update pygame clock and check for button press
                clock.tick(60)
                msg = None
                for event in pygame.event.get():
                    # assign the event to a message for the arduino
                    if event.type == pygame.JOYBUTTONDOWN:
                        # check if it's the QUIT button
                        if j.get_button(4):
                            raise QuitException
                        #print("button down\n")
                        for button_id in msg_assn.keys():
                            if j.get_button(button_id):
                                #print(str(button_id) + " is pressed\n")
                                msg = msg_assn[button_id]
                    elif event.type == pygame.JOYBUTTONUP:
                        #print("button up\n")
                        # set message to STOP if nothing is pressed (up event)
                        msg = "S"
                        # TODO make this smarter
                # make sure we have a message to send
                if msg is None:
                    continue
                # send message to the arduino
                arduino.write(msg.encode())
                # wait for arduino to answer
                while arduino.inWaiting()==0: pass
                if  arduino.inWaiting()>0: 
                    response=str(arduino.readline())
                    # print received data to console
                    #print("{}".format(response))
                    print(response + "\n")
                    # TODO format received data
                    # dataList=response.split("x")
                    # print("led state : {}".format(dataList[0]))
                    # print("Analog input A0 : {}".format(dataList[1]))
                    # print("Analog input A1: {}".format(dataList[2]))
                    
                    # remove data after reading
                    arduino.flushInput()
                        
        except KeyboardInterrupt: # Ctrl-C pressed
            print("KeyboardInterrupt has been caught.")

        except QuitException: # left bumper pressed
            print("Quit pressed on gamepad.")
