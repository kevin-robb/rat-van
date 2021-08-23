#!/usr/bin/env python
import serial,time, sys
from evdev import InputDevice, list_devices, ecodes, categorize

# only prints to console instead of sending to arduino and waiting for a response.
DEBUG_MODE = True

button_codes = {'Y':308,'A':304,'X':307,'B':305,'RB':311}
gp = None
def setup_gamepad():
    global gp
    # use evdev to setup gamepad
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

# current analog position
joy_pos = [0,0]

# calibration info
calibrating = False
cal_params = {"x_max":0,"x_min":0,"y_max":0,"y_min":0}
squish_factor = [1,1]
def calibrate():
    global cal_params, squish_factor
    cal_params["x_max"] = max(joy_pos[0],cal_params["x_max"])
    cal_params["x_min"] = min(joy_pos[0],cal_params["x_min"])
    cal_params["y_max"] = max(joy_pos[1],cal_params["y_max"])
    cal_params["y_min"] = min(joy_pos[1],cal_params["y_min"])
    squish_factor = [cal_params["x_max"] / 255, cal_params["y_max"] / 255]
    print(joy_pos,cal_params,squish_factor)

# create serial message for arduino
# format = "x,y" where x and y are ints in range [-255,255]
def create_msg():
    # format so joystick up = +255, and is the first coordinate
    return str(-int(joy_pos[1]/squish_factor[1])) + "," + str(int(joy_pos[0]/squish_factor[0]))

# function to send a message to the arduino and wait for a response
def send_msg(arduino, kill=False):
    msg = create_msg() if not kill else "0,0"
    if DEBUG_MODE:
        # print the message to console for debug purposes
        print("Sending msg: " + msg)
    else:
        # send the message over serial to the arduino
        arduino.write(msg.encode())
        # wait for arduino to answer
        while arduino.inWaiting()==0: pass
        if  arduino.inWaiting()>0: 
            response=str(arduino.readline())
            # print received data to console
            print(response)
            # remove data after reading
            arduino.flushInput()

def main():
    global calibrating, joy_pos
    setup_gamepad()
    # start serial comm with arduino
    print('Running. Hold X to calibrate, and press B to exit.')
    if DEBUG_MODE:
        print("Currently in DEBUG MODE: Messages will not be sent to Arduino.")
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
                        if event.code == button_codes['X']:
                            # press and hold to stay in calibration mode
                            calibrating = (event.value == 1)
                        elif event.code == button_codes['B'] and event.value == 1: # only on press
                            raise KeyboardInterrupt
                    # analog = control
                    elif event.type == ecodes.EV_ABS: # only use left joystick
                        # print info to console
                        absevent = categorize(event)
                        print(ecodes.bytype[absevent.event.type][absevent.event.code], absevent.event.value)
                        #print(absevent.event.type, absevent.event.code, absevent.event.value)
                        if absevent.event.type == 3: #only use left joystick
                            # update current joystick position
                            if absevent.event.code == 0: # ABS_X
                                joy_pos[0] = absevent.event.value
                            else: # absevent.event.type == 1: # ABS_Y
                                joy_pos[1] = absevent.event.value
                            # check for calibration mode
                            if calibrating:
                                calibrate()
                            else:
                                send_msg(arduino)
    
            except KeyboardInterrupt: # Ctrl-C or kill button (B) pressed
                print("KeyboardInterrupt has been caught.")
                # tell the arduino to STOP before closing the program
                send_msg(arduino,True)

if __name__ == "__main__":
    main()