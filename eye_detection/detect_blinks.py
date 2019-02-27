# import the necessary packages
from scipy.spatial import distance as dist
from imutils.video import FileVideoStream
from imutils.video import VideoStream
from imutils import face_utils
import numpy as np
import argparse
import imutils
import time
import dlib
import cv2
import serial


def eye_aspect_ratio(eye):
    A = dist.euclidean(eye[1], eye[5])
    B = dist.euclidean(eye[2], eye[4])

    C = dist.euclidean(eye[0], eye[3])

    ear = (A + B) / (2 * C)

    return ear

try:
    # Set up serial communication to communicate with arduino
    com_port = serial.Serial("COM3", 9600)
    # Delay to make sure no message is lost
    time.sleep(2)
except:
    print("No device connected")

ap = argparse.ArgumentParser()
ap.add_argument("-p", "--shape-predictor", required=True,
                help="path to facial landmark predictor")
ap.add_argument("-v", "--video", type=str, default="",
                help="path to input video file")
args = vars(ap.parse_args())

# Threshhold for what EAR values indicate open/closed eye
EYE_AR_THRESH = 0.2
# Number of frames eye is closed that counts as a blink
EYE_AR_CONSEC_FRAMES = 3
# Time of blink required to trigger alarm
EYE_AR_TIME_THRESH = 0.2

# Used to keep track of time during a blink
CLOSE_START = 0
CLOSE_DURATION = 0
# Used to count how many frames eye has currently been closed
COUNTER = 0
# Total number of blinks so far
TOTAL = 0

print("[INFO] loading facial landmark predictor...")
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(args["shape_predictor"])

(l_start, l_end) =  face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(r_start, r_end) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]

print("[INFO] starting video stream thread...")
vs = FileVideoStream(args["video"]).start()
fileStream = True

# FROM WEB CAM
vs = VideoStream(src=0).start()

# FROM RASP_PI
# vs = VideoStream(usePiCamera=True).start()

# IF 49 OR 52 UNCOMMENTED, UNCOMMENT THIS
fileStream = False

time.sleep(1)

while True:
    if fileStream and not vs.more():
        break

    # Get frame from video stream
    frame = vs.read()
    frame = imutils.resize(frame, width=450)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Find faces
    rects = detector(gray, 0)

    # Iterate over faces in frame
    for rect in rects:
        shape = predictor(gray, rect)
        shape = face_utils.shape_to_np(shape)

        # Find eyes in face and compute EAR
        left_eye = shape[l_start:l_end]
        right_eye = shape[r_start:r_end]

        left_ear = eye_aspect_ratio(left_eye)
        right_ear = eye_aspect_ratio(right_eye)

        # Average EAR over both eyes
        ear_mean = (left_ear + right_ear) / 2

        # Draw eyes in frame
        left_eye_hull = cv2.convexHull(left_eye)
        right_eye_hull = cv2.convexHull(right_eye)

        cv2.drawContours(frame, [left_eye_hull], -1, (0, 255, 0), 1)
        cv2.drawContours(frame, [right_eye_hull], -1, (0, 255, 0), 1)

        # Check if EAR below threshhold, i.e. if eyes are closed
        if ear_mean < EYE_AR_THRESH:
            if CLOSE_START == 0:
                CLOSE_START = time.time()
            COUNTER += 1

        # When eyes no longer closed, check if they were closed long enough for a blink
        else:
            CLOSE_DURATION = time.time()-CLOSE_START
            # If blink, then send data over serial port to arduino
            # if COUNTER >= EYE_AR_CONSEC_FRAMES:
            if CLOSE_DURATION >= EYE_AR_TIME_THRESH:


                TOTAL += 1
                print(">> blink")
                data = "Sleep\n".encode('utf-8')
                if 'com_port' in locals(): com_port.write(data)

            # Resest
            CLOSE_START = 0

            # Reset counter
            COUNTER = 0

        # Display number of blinks and current EAR on frame
        cv2.putText(frame, "Blinks: {}".format(TOTAL), (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        cv2.putText(frame, "EAR: {:.2f}".format(ear_mean), (300, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        cv2.putText(frame, "Blink duration: {:.2f}".format(CLOSE_DURATION), (10, 60),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF

    if key == ord("q"):
        break

com_port.close()
cv2.destroyAllWindows()
vs.stop()
