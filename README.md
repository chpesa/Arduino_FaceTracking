Arduino_FaceTracking
====================

Face tracking algorithm (programmed in C++) sends control commands to Arduino based Pan-Tilt camera programmed in Java.

Sample video demonstration of working algorithm: http://www.youtube.com/watch?v=rBiEUuuNTDo

Important - OpenCV 2.1 needs to be properly installed and configured before running this code.


FaceTracking.cpp 
- c++ file that contains face detection algorithm and builds a string to be sent to arduino via serial communication.

SerialCOM_DegreeContro.ino 
-  file that contains Java code to be downloaded to arduino Diecimila via Arduino software. 
-  Algorithm monitors COM port, reads and decodes string data to send pan-tilt degrees to servo motors.
-  string to be decoded is in the following format:  "pan;tilt;laser;/n"  where pan=0-180, tilt=0-180, laser=on/off

Serial.cpp & Serial.h
- Opensource library to define Serial port communication

haarcascade_frontalface_alt.xml
-  classifier file that needs to be loaded to achieve face detection using OpenCV
