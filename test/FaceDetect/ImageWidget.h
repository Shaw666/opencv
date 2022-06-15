#pragma once
#include <QWidget>
#include <qpixmap.h>
#include <QImage>
#include "opencv2/opencv.hpp"
#include <map>

#pragma execution_character_set("utf-8")

class ImageWidget : public QWidget
{
private:
    Q_OBJECT
public:
    ImageWidget(QWidget *parent);
    ~ImageWidget();
    void refreshImage(const QImage& image);
    void refreshImage(const QImage &image, const std::map<int, cv::Rect>& id_rect_map);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QPixmap m_pixmap;
    QImage m_pImage;
    std::map<int, std::string> id_name_map_;
    std::map<int, cv::Rect> id_rect_map_;
};


