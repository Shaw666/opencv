#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

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

    cv::CascadeClassifier face_cascade;   //定义人脸分类器
private:
    void detect();
    void read_csv(const std::string& filename, std::vector<cv::Mat>& images, std::vector<int>& labels, char separator = ';');
    void findYou();
};
#endif // MAINWINDOW_H
