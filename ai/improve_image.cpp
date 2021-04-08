/*
Original script from:
https://towardsdatascience.com/deep-learning-based-super-resolution-with-opencv-4fd736678066?gi=a959e9522203

Compile using the cmake option : -DOPENCV_GENERATE_PKGCONFIG=ON
Then, the LD flags are given by: pkg-config --cflags --libs opencv4

For example, on my Ubuntu:
g++ test.cpp -I/usr/local/include/opencv4 -L/usr/local/lib -lopencv_gapi -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lop
encv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_li
ne_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optfl
ow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_xfeatures2d -
lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_video -lopencv_dnn -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_xphoto -lopen
cv_photo -lopencv_imgproc -lopencv_core

*/

#include <opencv2/dnn_superres.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;

int main(int argc, char *argv[])
{
    //Create the module's object
    DnnSuperResImpl sr;

    //Set the image you would like to upscale
    string img_path = "image.png";
    Mat img = cv::imread(img_path);

    //Read the desired model
    //string path = "FSRCNN_x2.pb";
    //string path = "ESPCN_x4.pb";
    string path = "EDSR_x4.pb"; // best
    sr.readModel(path);

    //Set the desired model and scale to get correct pre- and post-processing
    //Needs to match option defined above (model and enhancement factor)
    //sr.setModel("fsrcnn", 2);
    //sr.setModel("espcn", 4);
    sr.setModel("edsr", 4);

    //Upscale
    Mat img_new;
    sr.upsample(img, img_new);
    cv::imwrite( "upscaled.png", img_new);

    return 0;
}
