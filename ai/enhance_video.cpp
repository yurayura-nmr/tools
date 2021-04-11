// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
 
/*
To make this work, need to build OpenCV with ffmpeg.
Otherwise, the code will fail silently (just not opening the video).
 
Does not need special -D ... options, just make sure these packages are installed before running cmake:

libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavresample-dev

To restore audio stream:
1. copy audio from original file
   ffmpeg -i yuhi.mp4 -vn -acodec copy yuhi.aac
2. merge them after upscaling
   ffmpeg -i yuhi_hq.mp4 -i yuhi.aac -map 0:0 -map 1:0 -c:v copy -c:a copy yuhi_hq_test1.mp4
*/

#include <iostream>
#include <opencv2/dnn_superres.hpp>
 
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
 
using namespace std;
using namespace cv;
using namespace dnn_superres;
 
int main(int argc, char *argv[])
{
  string input_path;
  string output_path;
  string algorithm;
  int scale;
  string path;

  input_path = "./yuhi.mp4";
  output_path = "./yuhi_hq.mp4";
 
  // Make sure model and scale match algorithm.
  //algorithm = "edsr";
  algorithm = "espcn";
  //path = "ESPCN_x4.pb"; 
  path = "ESPCN_x2.pb";
  //scale = 4;
  scale = 2;

  VideoCapture input_video(input_path);
  int ex = static_cast<int>(input_video.get(CAP_PROP_FOURCC));
  Size S = Size((int) input_video.get(CAP_PROP_FRAME_WIDTH) * scale,
                (int) input_video.get(CAP_PROP_FRAME_HEIGHT) * scale);

  VideoWriter output_video;
  output_video.open(output_path, ex, input_video.get(CAP_PROP_FPS), S, true);

  if (!input_video.isOpened())
   {
       std::cerr << "Could not open the video." << std::endl;
       return -1;
   }

  DnnSuperResImpl sr;
  sr.readModel(path);
  sr.setModel(algorithm, scale);

  int counter;
  counter = 0;
  for(;;)
  {
     Mat frame;
     counter++;
     cout << "[ Frame ] " << counter << endl;

     Mat output_frame;
       
     input_video >> frame;
     // Skip if frame empty or to end the loop
     if ( frame.empty() )
          break;
     sr.upsample(frame, output_frame);
     output_video << output_frame;
     // Output won't work in Windows Ubuntu
     //namedWindow("Upsampled video", WINDOW_AUTOSIZE);
     //imshow("Upsampled video", output_frame);
     //namedWindow("Original video", WINDOW_AUTOSIZE);
     //imshow("Original video", frame);
  }
  input_video.release();
  output_video.release();     
 
  return 0;
}
