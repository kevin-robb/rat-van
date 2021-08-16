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

# start serial comm with arduino
print('Running. Press CTRL-C or Left Bumper to exit.')
with serial.Serial("/dev/ttyACM0", 9600, timeout=1) as arduino:
    time.sleep(0.1) # wait for serial to open
    if arduino.isOpen():
        print("{} connected!".format(arduino.port))
        try:
            # check for activity on the gamepad
            for event in gp.read_loop():
                # if nothing is being pressed, stop
                if not gp.active_keys():
                    msg = 'S'
                # buttons
                if event.type == ecodes.EV_KEY:
                    print(event)
                    if event.value == 0:
                        msg = 'S'
                    elif event.code in msg_codes.keys():
                        if msg_codes[event.code] == 'Q':
                            #sys.exit()
                            raise KeyboardInterrupt
                        else:
                            msg = msg_codes[event.code]
                        # send the message over serial to the arduino
                        arduino.write(msg.encode())
                        # wait for arduino to answer
                        while arduino.inWaiting()==0: pass
                        if  arduino.inWaiting()>0: 
                            response=str(arduino.readline())
                            # print received data to console
                            print(response + "\n")
                            # TODO format received data
                            # dataList=response.split("x")
                            # print("led state : {}".format(dataList[0]))
                            # print("Analog input A0 : {}".format(dataList[1]))
                            # print("Analog input A1: {}".format(dataList[2]))
                            
                            # remove data after reading
                            arduino.flushInput()
                # analog
                elif event.type == ecodes.EV_ABS:
                    # do nothing but print info
                    absevent = categorize(event)
                    print(ecodes.bytype[absevent.event.type][absevent.event.code], absevent.event.value)
   
        except KeyboardInterrupt: # Ctrl-C pressed
            print("KeyboardInterrupt has been caught.")

