#include "albumview.h"
#include "imagespectablemodel.h"
#include "pixmaplabel.h"

#include <memory>
#include <vector>
#include <cmath>
#include <QApplication>
#include <QStyle>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileInfo>
#include <QTimer>
#include <QLabel>
#include <QHeaderView>
#include <QTableWidget>
#include <QElapsedTimer>
#include <QImageReader>


AlbumView::AlbumView(QWidget *parent)
    : QWidget{parent}
{
    auto outerLayout = new QVBoxLayout(this);
    this->setStyleSheet("background-color: #ffffff;");
    this->setLayout(outerLayout);
    //outerLayout->setContentsMargins(QMargins(12, 12, 12, 12));
    outerLayout->setContentsMargins(QMargins(4, 4, 4, 4));
    outerLayout->setSpacing(4);
    
    content = new QWidget(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet("background-color: #f1f1f1; border: none;");
    //scrollArea->setStyleSheet("background-color: green;");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(false);
    scrollArea->setWidget(content);
    outerLayout->addWidget(scrollArea, 1);
}


void AlbumView::layoutImages()
{
    if (!m_sizesComputed) {
        qDebug() << "Computing image sizes...";
        if (!m_gallery) {
            return;
        }
        auto specs = m_gallery->specs();
        if (specs.empty()) {
            return;
        }
        for (auto &spec : specs) {
            spec->m_dimensions = AlbumView::computeImageSize(*spec);
        }
        m_sizesComputed = true;
    }

    // remove all children from content
    QElapsedTimer t;
    auto currentImageCount = content->children().size();
    auto wantedImageCount = m_gallery->specs().size();
    while (currentImageCount > wantedImageCount) {
        auto child = content->children().last();
        if (QWidget *widget = qobject_cast<PixmapLabel *>(child)) {
            widget->setParent(nullptr); // remove from layout
            delete widget; // delete the widget
        }
        currentImageCount--;
    }
    while (currentImageCount < wantedImageCount) {
        auto label = new PixmapLabel(content);
        label->setStyleSheet("background-color: red; border: 1px solid #cccccc; margin: 2px;");
        label->setFixedSize(200, 200); // default size
        currentImageCount++;
    }
    qDebug() << ">> adjustment took" << t.elapsed() << "ms";
    // layout wih masonry coordinates
    t.restart();
    auto my = 8;
    auto columnCount = 4; // number of columns
    vector<int> columns;
    columns.resize(columnCount, 0); // initialize columns with 0 height
    auto availableWidth = scrollArea->width();
    auto gap = columnCount == 1 ? 0 : 8;
    auto columnWidth = (availableWidth - 2 * my - (gap * columnCount - 1)) / columnCount;
    auto column = 0;
    for (int i = 0; i < m_gallery->specs().size(); ++i) {
        auto spec = m_gallery->specs().at(i);
        auto imageSize = spec->m_dimensions;
        auto label = qobject_cast<PixmapLabel*>(content->children().at(i));
        auto pixmapPath = spec->m_location;
        if (!pixmapPath.isEmpty() && label->pixmapPath() != pixmapPath) {
            label->setPixmapPath(pixmapPath);
            label->setPixmap(QPixmap::fromImage(QImage(pixmapPath)));
        }
        auto x = my + column * columnWidth + gap + (max(0, column - 1)) * gap;
        auto y = columns[column] + gap;
        // scale height to maintain aspect ratio
        auto aspectRatio = static_cast<double>(imageSize.width()) / imageSize.height();
        auto width = imageSize.width();
        auto height = imageSize.height();
        if (imageSize.width() > columnWidth) {
            width = columnWidth;
            height = static_cast<int>(floor(imageSize.height() * (static_cast<double>(columnWidth) / imageSize.width())));
            imageSize.setWidth(columnWidth);
        }
        columns[column] += height; // update column height
        label->setFixedSize(QSize(width, height));
        label->setScaledContents(true); // Ensure the entire image fits in the label
        label->move(x, y);
        if (!label->isVisible()) {
            label->show();
        }
        column++;
        if (column >= columnCount) {
            column = 0;
        }
        //qDebug() << "Image" << i << "at position:" << x << ", " << y << ", size=" << columnWidth << "x" << height;
    }
    // find max height
    auto maxHeight = 0;
    for (auto h : columns) {
        maxHeight = max(maxHeight, h);
    }
    content->resize(availableWidth, maxHeight + my);
    qDebug() << ">> layout took" << t.elapsed() << "ms";
}


void AlbumView::resizeEvent(QResizeEvent * event)
{
    layoutImages();
}

void AlbumView::paintEvent(QPaintEvent *event)
{
}

void AlbumView::setGallery(shared_ptr<Gallery> gallery)
{
    this->m_gallery = gallery;
    this->m_sizesComputed = false;
    layoutImages();
}

QSize AlbumView::computeImageSize(const ImageSpec &spec) {
    QImageReader reader(spec.m_location);
    if (reader.canRead()) {
        return reader.size();
    }
    return QSize(0, 0);
}



