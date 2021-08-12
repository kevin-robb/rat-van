#!/usr/bin/python
import sys
import time
from evdev import InputDevice, list_devices, ecodes, categorize

print("Press Ctrl-C to Quit")
time.sleep(1)

devices = [InputDevice(path) for path in list_devices()]
for dev in devices:
    print("Input Device Found: " + dev.name)

if not devices:
    print('No Joysticks Found. Quitting.')
    exit()

# create gamepad object
gp = InputDevice('/dev/input/event3')
# print device info
print(gp)

# display codes
for event in gp.read_loop():
    # buttons
    if event.type == ecodes.EV_KEY:
        print(event)
    # analog
    elif event.type == ecodes.EV_ABS:
        absevent = categorize(event)
        print(ecodes.bytype[absevent.event.type][absevent.event.code], absevent.event.value)
