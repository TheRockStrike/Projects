import socket
import time
import errno
import create
import time

HOST = '127.0.0.1'    # The remote host
PORT = 12345              # The same port as used by the server

TARGET_OBJ = "person"
ADJUST = 100.0

ROOMBA_PORT = "/dev/ttyUSB0"
robot = create.Create(ROOMBA_PORT, startingMode=create.FULL_MODE)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
received = s.recv(1024)
timestamp = 0.0
s.setblocking(False)

while True:
    try:
        received = s.recv(1024)  # Read in what was sent
    except socket.error, e:
        err = e.args[0]
        if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
            # If time since last packet is too long...
            if (time.time() - timestamp > 2.0):
                # We know that the object of interest is no longer in view, so reset and find it.
                robot.go(0, LR * 25) # Rotate in the direction it was last seen (to avoid turning the "long" way)
            else:  # We lost view of it but it might be just out of frame.
                # Look at the last packet to see where it went.
                if (x_cord > 0.75):  # Last frame in which it was seen, it was on the right side. So it probably is to the right
                    robot.go(0, -20)
                    LR = -1
                elif (x_cord < 0.25):  # It was on the left side.
                    robot.go(0, 20)
                    LR = 1
                correction = dist_from_center * ADJUST
                robot.go(0, correction)
            continue

    if (received.decode(encoding='UTF-8', errors='strict') == "Close"):  # If the close flag was sent
        break  # End
    else:  # Pipe has not closed, so continue normal operation
        lb_center_left = robot.senseFunc(create.LIGHTBUMP_CENTER_LEFT)
        lb_center_right = robot.senseFunc(create.LIGHTBUMP_CENTER_RIGHT)
        print "Sensors: L:", lb_center_left(), "R:", lb_center_right()
        if ((lb_center_left() + lb_center_right()) / 2 > 50): # Too close to something
            robot.stop()
        else:
            decoded_info = received.decode(encoding='UTF-8', errors='strict').split(',')
            object_id = int(decoded_info[1])
            classification = decoded_info[2]

            if (classification == TARGET_OBJ):
                timestamp = float(decoded_info[0])
                x_cord = float(decoded_info[3])
                dist_from_center = 0.5 - x_cord
                print "Distance from center:", dist_from_center
                correction = dist_from_center * ADJUST
                print "Correction:", correction
                robot.go(40, correction)

robot.close()
s.close()
