#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qdebug.h>
#include <QDir>
#include <iostream>
#include <fstream>
#include "opencv2/face.hpp"
#include <QFileInfo>

using namespace std;
using namespace cv;

constexpr auto face_cascade_name = "C:\\AI\\opencv\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //detect();
    findYou();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::detect()
{

    //-- 1. Load the cascades
    if (!face_cascade.load(face_cascade_name)) {
        printf("--(!)Error loading face cascade\n");
        return;
    };

    QDir dir("C:\\Users\\xhp\\Pictures\\Saved Pictures");

    QFileInfoList  files = dir.entryInfoList(QStringList() << "*.jpg");

    int j = 0;

    for (auto&& filePath : files) {
        qDebug() << filePath.absoluteFilePath();
        Mat frame = imread(filePath.absoluteFilePath().toLocal8Bit().constData());

        std::vector<Rect> faces;
        Mat frame_gray;

        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

        equalizeHist(frame_gray, frame_gray);

        //-- Detect faces
        face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, CASCADE_DO_ROUGH_SEARCH, Size(70, 70), Size(200, 200));

        for (size_t i = 0; i < faces.size(); i++) {
            //Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
            //ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
            // 1 给图片画上人脸框
            rectangle(frame, faces[i], Scalar(255, 0, 0), 2, LINE_8, 0);
            // 1

            Mat faceROI = frame_gray(faces[i]);
            Mat MyFace;

            if (faceROI.cols > 100) {
                cv::resize(faceROI, MyFace, Size(92, 112));
                QString str = QString("%1/myfaces/face%2.jpg").arg(QCoreApplication::applicationDirPath()).arg(j++);
                bool ret = imwrite(str.toStdString().c_str(), MyFace);
                qDebug() << "save face image : " << str << ret;
            }

            // Mat faceROI = frame_gray(faces[i]);
            // std::vector<Rect> eyes;

            // //-- In each face, detect eyes
            // eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 1, CV_HAAR_DO_ROUGH_SEARCH, Size(3, 3));

            // for (size_t j = 0; j < eyes.size(); j++) {
            //     Rect rect(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y, eyes[j].width, eyes[j].height);

            //     //Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
            //     //int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
            //     //circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
            //     rectangle(frame, rect, Scalar(0, 255, 0), 2, 8, 0);
            // }
        }
    }

    //cvtColor(frame, frame, COLOR_BGR2RGB);
    //QImage qim((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step,
    //  QImage::Format_RGB888);
    //ui->label->setPixmap(QPixmap::fromImage(qim).scaled(400, 300));
}



//使用CSV文件去读图像和标签，主要使用stringstream和getline方法
void MainWindow::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator)
{
    std::ifstream file(filename.c_str(), ifstream::in);

    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(cv::Error::StsBadArg, error_message);
    }

    string line, path, classlabel;

    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);

        if (!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}


void MainWindow::findYou()
{
    //读取你的CSV文件路径.
	//string fn_csv = string(argv[1]);
	string fn_csv = "C:\\AI\\opencv\\atdatabase\\at.txt";
 
	// 2个容器来存放图像数据和对应的标签
	vector<Mat> images;
	vector<int> labels;

    read_csv(fn_csv, images, labels);
    if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(cv::Error::StsBadArg, error_message);
	}
    // 下面的几行代码仅仅是从你的数据集中移除最后一张图片
	//[gm:自然这里需要根据自己的需要修改，他这里简化了很多问题]
    int testIndex = 353;
	Mat testSample = images[testIndex];
	int testLabel = labels[testIndex];
	images.pop_back();
	labels.pop_back();
    std::string modelFilePath = QCoreApplication::applicationDirPath().toStdString() + "/MyFacePCAModel.xml";
    Ptr<cv::face::FaceRecognizer> model = cv::face::EigenFaceRecognizer::create();
    if (!QFileInfo::exists(modelFilePath.c_str())) {
        model->train(images, labels);
        model->save(modelFilePath);
    }
    else {
        model->read(modelFilePath);
    }
	//Ptr<cv::face::FaceRecognizer> model1 = cv::face::FisherFaceRecognizer::create();
	//model1->train(images, labels);
	//model1->save(QCoreApplication::applicationDirPath().toStdString() +"/MyFaceFisherModel.xml");
 //
	//Ptr<cv::face::FaceRecognizer> model2 = cv::face::LBPHFaceRecognizer::create();
	//model2->train(images, labels);
	//model2->save(QCoreApplication::applicationDirPath().toStdString() +"/MyFaceLBPHModel.xml");
 
	// 下面对测试图像进行预测，predictedLabel是预测标签结果
	int predictedLabel = model->predict(testSample);
	//int predictedLabel1 = model1->predict(testSample);
	//int predictedLabel2 = model2->predict(testSample);
 
	// 还有一种调用方式，可以获取结果同时得到阈值:
	//      int predictedLabel = -1;
	//      double confidence = 0.0;
	//      model->predict(testSample, predictedLabel, confidence);
	
	string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
	//string result_message1 = format("Predicted class = %d / Actual class = %d.", predictedLabel1, testLabel);
	//string result_message2 = format("Predicted class = %d / Actual class = %d.", predictedLabel2, testLabel);
	qDebug() << result_message.c_str() << endl;
    //qDebug() << result_message1.c_str() << endl;
    //qDebug()<< result_message2.c_str() << endl;
}
