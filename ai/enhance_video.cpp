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
// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
 
#include <iostream>
#include <iomanip>

#include <opencv2/dnn_superres.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
 
using namespace std;
using namespace cv;
using namespace dnn_superres;

//#define QUICK_CHECK
 
int main(int argc, char *argv[])
{
  string input_path;
  string output_path;
  string algorithm;
  int scale;
  string path;

  //input_path = "./aitakatta.mp4";
  //output_path = "./aitakatta_hq.mp4";
 
   
  // Make sure model and scale match algorithm.
  //algorithm = "edsr";
  //algorithm = "espcn";
  //algorithm = "lapsrn";
  algorithm = "fsrcnn";

  //path = "EDSR_x4.pb";
  //path = "ESPCN_x4.pb"; 
  //path = "ESPCN_x2.pb";
  //path = "LapSRN_x8.pb";
  //path = "LapSRN_x4.pb";  
  path = "FSRCNN_x4.pb";

  scale = 4;

  input_path = "./yuhi.mp4";
  output_path = "./yuhi_" + algorithm + to_string(scale) + ".mp4";

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

  int counter, totalFrames;
  counter = 0;
  
  totalFrames = input_video.get(CAP_PROP_FRAME_COUNT);
  cout << "Total frames: " << totalFrames << endl;

  for(;;)
  {
     Mat frame;
     Mat output_frame;

     counter++;

     cout << "[\033[1;32m Frame \033[0m] " << to_string(counter)
          << setw(9) << " (" <<  (100 * counter / totalFrames) << " "
          << setw(3) << " % done)" << endl;
      
     input_video >> frame;

     // End loop
     if ( frame.empty() )
          break;

     sr.upsample(frame, output_frame);
     output_video << output_frame;

    // Quick check for testing
    #ifdef QUICK_CHECK
    if ( counter == 10)
          break;
    #endif
  }

  input_video.release();
  output_video.release();     

  return 0;
}
