#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // read the image from samples folder
    // Mat img = imread("../samples/starry_night.jpg", IMREAD_COLOR);
    VideoCapture capture("https://www.bogotobogo.com/python/OpenCV_Python/images/mean_shift_tracking/slow_traffic_small.mp4");

    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    Mat frame, roi, hsv_roi, mask;
    // take first frame of the video
    capture >> frame;
    // setup initial location of window
    Rect track_window(300, 200, 100, 50); // simply hardcoded the values
    // set up the ROI for tracking
    roi = frame(track_window);
    cvtColor(roi, hsv_roi, COLOR_BGR2HSV);
    inRange(hsv_roi, Scalar(0, 60, 32), Scalar(180, 255, 255), mask);
    float range_[] = {0, 180};
    const float* range[] = {range_};
    Mat roi_hist;
    int histSize[] = {180};
    int channels[] = {0};
    calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, histSize, range);
    normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);
    // Setup the termination criteria, either 10 iteration or move by at least 1 pt
    TermCriteria term_crit(TermCriteria::EPS | TermCriteria::COUNT, 10, 1);
    int counter = 0;
    VideoWriter vid;
    while(true){
        Mat hsv, dst;
        capture >> frame;
        if (frame.empty())
            break;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        calcBackProject(&hsv, 1, channels, roi_hist, dst, range);
        // apply camshift to get the new location
        RotatedRect rot_rect = CamShift(dst, track_window, term_crit);
        // Draw it on image
        Point2f points[4];
        rot_rect.points(points);
        for (int i = 0; i < 4; i++)
            line(frame, points[i], points[(i+1)%4], 255, 2);
        // imshow("img2", frame);
        imwrite(("../images/out_"+ to_string(counter)+".jpg"), frame);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
        counter++;
    }
    
    int k = waitKey(0);
    return 0;
}