#pragma once


#include <string>
#include <memory>
#include <queue>
#include "opencv2/opencv.hpp"
class OpenCVVideoCapture
{
private:

public:
    OpenCVVideoCapture(const std::string& fileName);
    ~OpenCVVideoCapture();
    int startCapture();
    void stopCapture();
    void readFrame(cv::Mat& frame);
private:
    std::string file_name_;
    std::unique_ptr<cv::VideoCapture> video_capture_;
    std::thread* video_capture_thread_;
    bool capture_exit_;
    std::queue<cv::Mat> frame_queue_;
};


