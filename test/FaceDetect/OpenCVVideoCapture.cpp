#include "OpenCVVideoCapture.h"
#include <iostream>

using namespace std;
using namespace cv;

OpenCVVideoCapture::OpenCVVideoCapture(const std::string& file_name)
    :file_name_(file_name), video_capture_thread_(nullptr)
{
    video_capture_.reset(new cv::VideoCapture(file_name));
}

OpenCVVideoCapture::~OpenCVVideoCapture()
{
}

int OpenCVVideoCapture::startCapture()
{
    if(video_capture_thread_)
        return -1;

    capture_exit_ = false;
    bool ret = video_capture_->open(0);

    if (!ret || !video_capture_->isOpened()) {
        cerr << "打开相机失败" << endl;
        return -1;
    }

    video_capture_thread_ = new std::thread([this]() {
        while (!capture_exit_) {
            Mat img;
            video_capture_->read(img);
            Mat img_small;
            cv::resize(img, img_small, Size(img.cols / 2, img.rows / 2));
            frame_queue_.push(std::move(img_small));
            this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    });
    return 0;
}

void OpenCVVideoCapture::stopCapture()
{
    if(video_capture_thread_ && video_capture_thread_->joinable()) {
        capture_exit_ = true;
        video_capture_thread_->join();
    }
}

void OpenCVVideoCapture::readFrame(cv::Mat& frame)
{
    if(frame_queue_.empty())
        return;

    frame = frame_queue_.front();
}