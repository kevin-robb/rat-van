#!/usr/bin/env python
import serial,time, sys
from evdev import InputDevice, list_devices, ecodes, categorize
from math import acos, sqrt, pi

# only prints to console instead of sending to arduino and waiting for a response.
DEBUG_MODE = False
MAX_POS = 255

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
    squish_factor = [cal_params["x_max"] / MAX_POS, cal_params["y_max"] / MAX_POS]
    print(joy_pos,cal_params,squish_factor)

# create serial message for arduino
def create_msg():
    # get cartesian coordinates
    x = joy_pos[0]/squish_factor[0]
    y = -(joy_pos[1]/squish_factor[1])
    # zero region
    if abs(x) < 0.25 * MAX_POS and abs(y) < 0.25 * MAX_POS:
        return str("0,0")
    # calculate stuff
    yaw = acos(abs(x)/sqrt(x**2 + y**2))
    turn_coeff = -1 + yaw/(pi/4)
    turn = turn_coeff * abs(abs(y) - abs(x))
    move = max(abs(y), abs(x))
    # set motor speeds depending on quadrant of joystick
    if (x >= 0 and y >= 0) or (x < 0 and y < 0):
        left = move
        right = turn
    else:
        left = turn
        right = move
    # check if we're going backwards
    if y < 0:
        left = -left
        right = -right
    # create the message to send
    return str(int(left*MAX_POS)) + "," + str(int(right * MAX_POS))

# function to send a message to the arduino
def send_msg(arduino, kill=False):
    msg = create_msg() if not kill else "0,0"
    if DEBUG_MODE:
        # print the message to console for debug purposes
        print("Sending msg: " + msg)
    else:
        # send the message over serial to the arduino
        arduino.write(msg.encode())

def main():
    global calibrating, joy_pos, cal_params
    setup_gamepad()
    # just set the known values of my joystick to skip calibrating each time
    cal_params = {"x_max":32767,"x_min":-32768,"y_max":32767,"y_min":-32768}
    # call calibrate to set the squish factor
    calibrate()
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