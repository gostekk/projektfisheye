#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <unistd.h>

using namespace cv;
using namespace std;

//void ConvertFromFishEye(

int main(int argc, char** argv)
{
    int xd, yd;
    double vStrenght=0.8; //no clue
    float D, DO, foc=4.0/12.0, r, xf, yf, cosfi, sinfi;
    if(argc < 2){
        return -1;
    }
    cout << "Filename = " << argv[1] << endl;
    VideoCapture inputVideo(argv[1]);

    if(!inputVideo.isOpened()){
        return -1;
    }
    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));
    cout << ex;
    Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
            (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
    Size S2 = Size((int) S.width/2, (int) S.height/2);
    cout << "Width = " << S.width << " Height = " << S.height << " Total frames = " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    cout << "Width/2 = " << S2.width << " Height/2 = " << S2.height << endl;

    DO = foc*sqrt(S2.width*S2.width + S2.height*S2.height);
    D = foc*sqrt(S.width*S.width + S.height*S.height);
    cout << "D = " << D << " DO = " << DO << endl;

    VideoWriter outputVideo;
   //CV_FOURCC('M','P','4','2');
    outputVideo.open("out.avi", CV_FOURCC('X','2','6','4'), inputVideo.get(CV_CAP_PROP_FRAME_COUNT), S, true);
    if(!outputVideo.isOpened()){
        return -1;
    }
    Mat frame, post,img;
    for(;;){
        inputVideo >> frame;
        post = frame;
        //imwrite("test.png", frame);
        if(frame.empty()) break;
        for(int i=0; i <= int(frame.rows/2);i++){
            for(int j=0; j <= int(frame.cols/2);j++){
                //cout << "i = " << i << " j = " << j << endl;
                xf = (float)(i-S2.width/2);
                yf = (float)(j-S2.height/2);
                r = sqrt(xf*xf + yf*yf) * vStrenght;
                cosfi = xf / r;
                sinfi = yf / r;

                xf = cosfi * 2 * D * tan(0.5 * atan2(r, DO));
                yf = sinfi * 2 * D * tan(0.5 * atan2(r, DO));
                //cout << "xf = " << xf << " yf = " << yf << endl;
                //usleep(20000);
                xd = (int)xf;
                yd = (int)yf;
                xd += frame.rows / 2;
                yd += frame.cols / 2;

                if((xd>0)&&(xd<frame.rows)&&(yd>0)&&(yd<frame.cols)){
                    Vec3b color = frame.at<Vec3b>(xd,yd);
                    //cout << "xd = " << xd << " yd = " << yd << endl;
                    //cout << "i = " << i << " j = " << j << endl;
                    post.at<Vec3b>(i,j) = color;
                    //cout << color << endl;
                    //outputVideo.write(post);
                }

            }
        }
        //imwrite("test.png", post);
        //cout << "i = " << i << " j = " << j << endl;
        outputVideo << post;

        //cout << "Added frame" << endl;
        //cout << "abc = " << frame.data << endl;

    }
    return 0;
}
