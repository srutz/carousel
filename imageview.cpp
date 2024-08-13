#include "imageview.h"

#include <QPainter>

ImageView::ImageView(QWidget *parent)
    : QWidget{parent}
    , m_label(QString())
{
    m_errorPixmap.load(":/images/images/block_640.png");
    showErrorImage();
}


void ImageView::paintEvent(QPaintEvent *event)
{
    //qDebug() << "ImageView::paintEvent" << size();
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    auto size = this->size();
    //painter.fillRect(QRect(0, 0, size.width(), size.height()), QColor(200, 160, 140));

    // draw the svg
    if (!m_pixmap.isNull()) {
        auto pixmapSize = m_pixmap.size();
        auto pixmapAspectRatio = static_cast<double>(pixmapSize.width()) / pixmapSize.height();
        double widgetAspectRatio = static_cast<double>(size.width()) / size.height();

        QRectF targetRect;
        if (widgetAspectRatio > pixmapAspectRatio) {
            double scaledWidth = pixmapAspectRatio * size.height();
            targetRect = QRectF((size.width() - scaledWidth) / 2, 0, scaledWidth, size.height());
        } else {
            qreal scaledHeight = size.width() / pixmapAspectRatio;
            targetRect = QRectF(0, (size.height() - scaledHeight) / 2, size.width(), scaledHeight);
        }
        // draw the pixmap
        painter.drawPixmap(targetRect, m_pixmap, m_pixmap.rect());
    }


    // draw some text
    /*
    painter.fillRect(QRect(10, 0, width() - 20, 32), QColor(0, 0, 0, 200));
    painter.setFont(QFont("Sans", 12, QFont::Light));
    painter.setPen(QColor(255, 255, 255, 255));
    painter.drawText(20, 18, m_label);
    */
}


QString ImageView::label()
{
    return m_label;
}


void ImageView::setLabel(const QString &label)
{
    this->m_label = label;
}


void ImageView::showPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
    update();
}


void ImageView::showErrorImage()
{
    showPixmap(m_errorPixmap);
    m_label = "";
}


void ImageView::showFile(QString path)
{
    QPixmap pixmap;
    bool loaded = pixmap.load(path);
    if (!loaded) {
        showErrorImage();
    }
    m_label = path;
    showPixmap(pixmap);
}
