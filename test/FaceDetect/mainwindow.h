#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include "ImageWidget.h"
#include "opencv2/face.hpp"
#include <thread>
#include <chrono>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

struct Detect_Frame_Data {
    cv::Mat frame;
    std::map<int, cv::Rect> id_rect_map;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    cv::CascadeClassifier face_cascade; //定义人脸分类器
    cv::Ptr<cv::face::FaceRecognizer> model_;
    std::unique_ptr<cv::VideoCapture> video_capture_;
    std::queue<Detect_Frame_Data> frame_bgr_queue_;
    std::mutex* pMutex_;
    ImageWidget *m_pImageWidget;
    cv::VideoCapture video_capture;
    std::thread* video_capture_thread;
    bool capture_exit_;
    bool save_roi_once_;
    std::chrono::steady_clock last_compare_roi_ts;

private:
    void detectFrame(cv::Mat &frame_bgr);
    void detectImages();
    void read_csv(const std::string &filename, std::vector<cv::Mat> &images, std::vector<int> &labels, char separator = ';');
    void loadModule();
    int FindYou(cv::Mat &testSample, int testLabel);
    void VideoCapture();
};
#endif // MAINWINDOW_H
