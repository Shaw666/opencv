#pragma once
#include <QWidget>
#include <qpixmap.h>
#include <QImage>

class ImageWidget : public QWidget
{
private:
    Q_OBJECT
public:
    ImageWidget(QWidget *parent);
    ~ImageWidget();
    void refreshImage(const QImage& image);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QPixmap m_pixmap;
    QImage m_pImage;
};


