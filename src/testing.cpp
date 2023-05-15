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
    Mat img = imread("../samples/s.png", IMREAD_COLOR);

    if(img.empty())
    {
        std::cout << "Could not read the image ! " << std::endl;
        return 1;
    }

    // cerate an image holder to get the green channel
    Mat channel;

    // Extract green plane
    extractChannel(img, channel, 2);

    // Resize image for display
    // resize(channel, channel, Size(), 0.20, 0.20);
    
    // Get a named window
    // cv::namedWindow("Green Color plane");

    imwrite("out.jpg", channel);

    // show the green plane (gray image)
    // imshow("Green Color plane", channel);
    
    int k = cv::waitKey(0);
    return 0;
}