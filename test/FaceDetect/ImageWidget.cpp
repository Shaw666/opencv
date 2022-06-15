#include "ImageWidget.h"
#include <QPainter>


ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{
    id_name_map_.emplace(35,("肖焕鹏"));
}

ImageWidget::~ImageWidget()
{
}
void ImageWidget::refreshImage(const QImage &image)
{
    m_pImage = image.copy();
    update();
}

void ImageWidget::refreshImage(const QImage &image, const std::map<int, cv::Rect>& id_rect_map)
{
    id_rect_map_ = id_rect_map;
    m_pImage = image.copy();
    update();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0,0,0));
    painter.drawRect(this->rect());
    if (m_pImage.isNull())
        return;
    m_pixmap = QPixmap::fromImage(m_pImage);
    QRect rectContent = this->rect();
    QPixmap pm = m_pixmap.scaled(rectContent.width(), rectContent.height());
    painter.drawPixmap(0, 0, pm);
    painter.restore();
    for(auto&& var : id_rect_map_){
        auto it_name = id_name_map_.find(var.first);
        if(it_name != id_name_map_.end()){
            QString name = QString::fromStdString(it_name->second);
            cv::Rect rect = var.second;
            painter.drawText(rect.x + 10, rect.y - 20, name);
        }
    }


}