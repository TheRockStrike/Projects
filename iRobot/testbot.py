import create
import time

ROOMBA_PORT = "/dev/ttyUSB0"
robot = create.Create(ROOMBA_PORT)
robot.printSensors()  # debug output
robot.toSafeMode()
robot.go(10, 0)
time.sleep(2.0)
robot.go(0, 0)
robot.close()
