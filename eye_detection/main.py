import os
import platform

if __name__ == "__main__":
    if(platform.system() == 'Windows'):
        os.system("python detect_blinks.py "
            "--shape-predictor shape_predictor_68_face_landmarks.dat")
    else:
        os.system("python3 detect_blinks.py "
            "--shape-predictor shape_predictor_68_face_landmarks.dat")