#ifndef GALLERY_H
#define GALLERY_H

#include <QString>
#include <QDebug>
#include <memory>

using namespace std;


struct ImageSpec
{
    QString m_location;

    ImageSpec(const QString location) : m_location(location) {
        //qDebug() << "ImageSpec" << location;
    }

    ImageSpec(const ImageSpec &spec) : m_location(spec.m_location) {
        //qDebug() << "ImageSpec(copy)";
    }

    virtual ~ImageSpec() {
        //qDebug() << "~ImageSpec";
    }
};

class Gallery
{
    vector<shared_ptr<ImageSpec>> m_images;
    int m_index = -1;

public:
    Gallery();
    Gallery(const Gallery&) = default;

    void initFromDirectory(QString directory);
    ImageSpec currentImage() const;
    void step(int direction);
};

#endif // GALLERY_H
