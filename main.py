import machine
import time
from Adafruit_BNO055.BNO055 import *
# Pyboard hardware I2C
i2c = machine.I2C(0,scl=machine.Pin(15),sda=machine.Pin(14))
# ESP8266 soft I2C
# i2c = machine.I2C(-1, scl=machine.Pin(2), sda=machine.Pin(0))
imu = BNO055(i2c)
calibrated = False
while True:
    time.sleep(1)
    if not calibrated:
        calibrated = imu.calibrated()
        print('Calibration required: sys {} gyro {} accel {} mag {}'.format(*imu.cal_status()))
    print('Temperature {}°C'.format(imu.temperature()))
    print('Mag       x {:5.0f}    y {:5.0f}     z {:5.0f}'.format(*imu.mag()))
    print('Gyro      x {:5.0f}    y {:5.0f}     z {:5.0f}'.format(*imu.gyro()))
    print('Accel     x {:5.1f}    y {:5.1f}     z {:5.1f}'.format(*imu.accel()))
    print('Lin acc.  x {:5.1f}    y {:5.1f}     z {:5.1f}'.format(*imu.lin_acc()))
    print('Gravity   x {:5.1f}    y {:5.1f}     z {:5.1f}'.format(*imu.gravity()))
    print('Heading     {:4.0f} roll {:4.0f} pitch {:4.0f}'.format(*imu.euler()))



# 
# from machine import Pin, I2C
# from time import sleep
# 
# i2c = I2C(0,scl=Pin(14),sda=Pin(15),freq=400000)
# 
# led = Pin(22, Pin.OUT)
# led.value(1)
# 
# while True:
#     led.toggle()
#     sleep(0.5)