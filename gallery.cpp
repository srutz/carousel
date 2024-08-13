#include "gallery.h"

#include <QDir>

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
    if (m_index < 0 || m_index >= m_images.size()) {
        return ImageSpec("");
    }
    auto& i = m_images.at(m_index);
    return i->m_location;
}


void Gallery::step(int direction)
{
    if (m_images.size() == 0) {
        return;
    }
    m_index = (m_index + direction) % m_images.size();
    while(m_index < 0) {
        m_index += m_images.size();
    }
}
