#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
   
#include <iostream>

   
using namespace std;
using namespace cv;
   
int main()
{
   // Load Face cascade (.xml file)
       CascadeClassifier face_cascade;
       face_cascade.load( "haarcascade_frontalface_alt.xml" );
       if(!face_cascade.load("haarcascade_frontalface_alt.xml"))
       {
         cerr<<"Error Loading XML file"<<endl;
         return 0;
       }
 
 VideoCapture capture(0);  
    if ( !capture.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }
    namedWindow("window", 1);  
    while (true)
     { 
       Mat image; 
       bool bSuccess = capture.read(image); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
 
      // Detect faces
      std::vector<Rect> faces;
      face_cascade.detectMultiScale( image, faces, 1.1, 2, 0, Size(30, 30) );
   
      // Draw circles on the detected faces
      for( int i = 0; i < faces.size(); i++ )
      {
        //Take the center of the face
        Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        //Draw an ellipse around
        ellipse( image, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
      }
       
  imshow( "Detected Face", image );
  if (waitKey(1) == 27) //wait for 'esc' key press for 1ms. If 'esc' key is pressed, break loop
        {
                cout << "esc key is pressed by user" << endl;
                break; 
        }
   }  
                
     return 0;
}