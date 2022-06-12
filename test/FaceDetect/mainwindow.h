#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"
#include "opencv2/face.hpp"
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include "ImageWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

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
    std::queue<cv::Mat> frame_bgr_queue_;
    std::mutex* pMutex_;
    ImageWidget *m_pImageWidget;
private:
    void detectFrame(cv::Mat &frame_bgr);
    void detectImages();
    void read_csv(const std::string &filename, std::vector<cv::Mat> &images, std::vector<int> &labels, char separator = ';');
    void loadModule();
    void FindYou(cv::Mat &testSample, int testLabel);
    void VideoCapture();
};
#endif // MAINWINDOW_H
