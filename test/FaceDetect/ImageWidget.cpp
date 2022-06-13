#include "ImageWidget.h"
#include <QPainter>


ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{
}

ImageWidget::~ImageWidget()
{
}
void ImageWidget::refreshImage(const QImage &image)
{
    m_pImage = image.copy();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0,0,0));
    painter.drawRect(this->rect());
    if (m_pImage.isNull())
        return;
    m_pixmap = QPixmap::fromImage(m_pImage);
    QRect rectContent = this->rect();
    QPixmap pm = m_pixmap.scaled(rectContent.width(), rectContent.height());

    painter.drawPixmap(0, 0, pm);
}