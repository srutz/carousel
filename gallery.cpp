#include "gallery.h"

#include <QDir>
#include <QElapsedTimer>

Gallery::Gallery() {}


void Gallery::initFromDirectory(QString directory)
{
    QStringList imageFilters;
    imageFilters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif" << "*.tiff" << "*.svg";
    auto dir = QString(".") == directory ? QDir::home() : QDir(directory);
    QFileInfoList fileInfoList = dir.entryInfoList(imageFilters, QDir::Files);

    m_images.clear();
    for (const QFileInfo &fileInfo : fileInfoList) {
        qDebug() << "loading: " << fileInfo.absoluteFilePath();
        m_images.push_back(make_shared<ImageSpec>(ImageSpec(fileInfo.absoluteFilePath())));
    }
    this->m_index = m_images.size() > 0 ? 0 : -1;
}


ImageSpec Gallery::currentImage() const
{
    int count = m_images.size();
    if (m_index < 0 || m_index >= count) {
        return ImageSpec("");
    }
    auto& i = m_images.at(m_index);
    return *i;
}

ImageSpec Gallery::image(int offsetFromImage) const
{
    QElapsedTimer timer;
    timer.start();
    if (m_images.size() == 0) {
        return ImageSpec("");
    }
    int i = m_index + offsetFromImage;
    int count = m_images.size(); // attention signed vs unsigned
    while (i < 0) {
        i += count;
    }
    while (i >= count) {
        i -= count;
    }
    //qDebug() << "offsetFromImage " << m_index << offsetFromImage << i << " ... " << timer.elapsed();
    return *m_images.at(i);
}


void Gallery::step(int direction)
{
    if (m_images.size() == 0) {
        return;
    }
    m_index = m_index + direction;
    int count = m_images.size();  // int and auto make a difference here (signed vs unsigned)
    while(m_index >= count) {
        m_index -= count;
    }
    while(m_index < 0) {
        m_index += count;
    }
}
