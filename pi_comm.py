#!/usr/bin/env python
import serial,time, sys
from evdev import InputDevice, list_devices, ecodes, categorize

# use evdev to setup gamepad
gp = None
devices = [InputDevice(path) for path in list_devices()]
for dev in devices:
    print("Input Device Found: " + dev.name)
    if dev.name == "Microsoft X-Box 360 pad":
        gp = dev
if not devices or gp is None:
    print('No Joysticks Found. Quitting.')
    exit()
# print device info
print(gp)

# create serial messages for arduino
msg = ""
msg_codes = {308:'F',304:'B',307:'L',305:'R',311:'Q'}

# function to send a message to the arduino and wait for a response
def send_to_uno(msg: str):
    # send the message over serial to the arduino
    arduino.write(msg.encode())
    # wait for arduino to answer
    while arduino.inWaiting()==0: pass
    if  arduino.inWaiting()>0: 
        response=str(arduino.readline())
        # print received data to console
        print(response)
        # TODO format received data
        # dataList=response.split("x")
        # print("led state : {}".format(dataList[0]))
        # print("Analog input A0 : {}".format(dataList[1]))
        # print("Analog input A1: {}".format(dataList[2]))
        
        # remove data after reading
        arduino.flushInput()

# start serial comm with arduino
print('Running. Press CTRL-C or Left Bumper to exit.')
with serial.Serial("/dev/ttyACM0", 9600, timeout=1) as arduino:
    time.sleep(0.1) # wait for serial to open
    if arduino.isOpen():
        print("{} connected!".format(arduino.port))
        try:
            # check for activity on the gamepad
            for event in gp.read_loop():
                # buttons
                if event.type == ecodes.EV_KEY:
                    print(event)
                    if event.value == 0: # unpress
                        msg = 'S'
                    elif event.code in msg_codes.keys(): # press one of our defined keys
                        if msg_codes[event.code] == 'Q':
                            raise KeyboardInterrupt
                        else:
                            msg = msg_codes[event.code]
                    send_to_uno(msg)
                # # analog
                # elif event.type == ecodes.EV_ABS:
                #     # do nothing but print info
                #     absevent = categorize(event)
                #     print(ecodes.bytype[absevent.event.type][absevent.event.code], absevent.event.value)
   
        except KeyboardInterrupt: # Ctrl-C pressed
            print("KeyboardInterrupt has been caught.")
            # tell the arduino to STOP
            send_to_uno('S')
