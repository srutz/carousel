#include "pixmaplabel.h"

PixmapLabel::PixmapLabel(QWidget *parent)
    : QLabel{parent}
{
}

QString PixmapLabel::pixmapPath() const
{
    return m_pixmapPath;
}

void PixmapLabel::setPixmapPath(const QString &path)
{
    if (m_pixmapPath != path) {
        m_pixmapPath = path;
        emit pixmapPathChanged(m_pixmapPath);
    }
}
