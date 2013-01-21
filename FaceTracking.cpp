#include <iostream>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cv.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib> 
#include <string.h> 
#include <list>
#include <sstream>
#include <time.h>
#include <vector>
#include <fstream>
#include <windows.h>
#include <tchar.h>
#include "Serial.h"

using namespace std;
using namespace cv;

#ifdef _EiC
#define WIN32
#endif

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;
 
int midFaceX=0; //current face center position x coordinate
int midFaceY=0; //current face center position y coordinate

int ImgWidth=320; //image width
int ImgHeight=240; // image height
void WINAPI Sleep(__in DWORD dwMilliseconds);

string CreateStreamData(int pan, int tilt, bool laser); //creates string to be sent to arduino via serial COM

void detectFaces( IplImage *img );
 
int main( int argc, char** argv )
{
    CvCapture *capture;
    IplImage  *frame;
    int       key=0;
    char      *filename = "haarcascade_frontalface_alt.xml";

  
// ******* initialize parameters ******
	int servoPanPosition=90; // position can be from 0 to 180
	int servoTiltPosition=90; // position can be from 0 to 180
	bool laser=false;
	int Sensitivity=2;  // 
	int midScreenX=(ImgWidth/2);
	int midScreenY=(ImgHeight/2);
	int midScreenWindow=12;
	int stepSize=2;  // how fast the servos should move
	bool sendcommand=false;  //flag

	 CSerial serial;

	// Attempt to open the serial port  - change to port where arduino is connected
	serial.Open(_T("COM6"),0,0,false);   
 
    cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 ); // load classifier
    storage = cvCreateMemStorage( 0 );
    capture = cvCaptureFromCAM( 1 );
    assert( cascade && storage && capture );
 
    cvNamedWindow( "video", 1 ); //display video
 
    while( key != 'q' ) {
        frame = cvQueryFrame( capture );
 
        if( !frame ) break;
 
 
        detectFaces( frame );   // Detect face


		cout<<"MidFaceY: "<<midFaceY<<" midScreenY: "<<midScreenY<<std::endl;  //display face position

   // *** define imaginary horizontal-vertical line to find out whether the face is above/below horizontal line or right/left vertical line.
		midScreenX=120; 
		midScreenY=100;
		if(midFaceX!=0 && midFaceY!=0)
		{
			//Find out if the Y component of the face is below the 	middle of screen.
			if(midFaceY<(midScreenY-midScreenWindow))
			{
				if(servoTiltPosition<=175)
				{ 
					servoTiltPosition+=stepSize;
					sendcommand=true;
				}
			}
			 if(midFaceY>(midScreenY+midScreenWindow))
			{
				if(servoTiltPosition>=5)
				{
					servoTiltPosition-=stepSize;
					sendcommand=true;
				}

			}

			//Find out if the X component of the face is to the left of the middle of the screen.
			 if(midFaceX < (midScreenX - midScreenWindow))
			{
				if(servoPanPosition >= 5)
				{
					servoPanPosition -= stepSize;
					sendcommand=true;
				}

		    }
  		//Find out if the X component of the face is to the right of the middle of the screen.
    if(midFaceX > (midScreenX + midScreenWindow))
			{
				if(servoPanPosition <= 175)
				{
					servoPanPosition +=stepSize; 
					sendcommand=true;
				}

		    }

			 if(sendcommand==true) //build string command with pan-tilt degrees, and send to arduino
			 {
				 std::string mydata=CreateStreamData(servoPanPosition,servoTiltPosition,laser);
				 serial.Write(mydata.c_str());
				 Sleep(10);
				 sendcommand=false;
			 }

		}//End initial check
		 midFaceX=0; //reset face coordinates
		 midFaceY=0;

 
        key = cvWaitKey( 10 );
		if(key=='z')
		{
			laser=true;
			cout<<"ON"<<endl;
		}else if(key=='x')
		{
			laser=false;
			cout<<"OFF"<<endl;
		}
	}
 
	serial.Close();
    /* free memory */
    cvReleaseCapture( &capture );
    cvDestroyWindow( "video" );
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseMemStorage( &storage );
 
    return 0;
}
 
void detectFaces( IplImage *img )
{
    int i;


    /* detect faces */
    CvSeq *faces = cvHaarDetectObjects(img,cascade,storage,1.2,2,CV_HAAR_DO_CANNY_PRUNING,cvSize(50,50));
 
    /* for each face found, draw a red box */
    for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );
        cvRectangle( img,
                     cvPoint( r->x, r->y ),
                     cvPoint( r->x + r->width, r->y + r->height ),
                     CV_RGB( 255, 0, 0 ), 1, 8, 0 );


		midFaceX=(r->x+r->width)/2;
		midFaceY=(r->y+r->height)/2;
		
		
    }
 

    /* display video */
    cvShowImage( "video", img );
}

string CreateStreamData(int pan, int tilt, bool laser)
{
			std::stringstream ss;
			if(laser)
				ss <<pan<<";"<<tilt<<";"<<"on;\n";
			else
				ss <<pan<<";"<<tilt<<";"<<"off;\n";

			std::string data=ss.str();
			return data;

}

