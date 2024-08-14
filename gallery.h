#ifndef GALLERY_H
#define GALLERY_H

#include <QString>
#include <QDebug>
#include <memory>

using namespace std;

/*
 * Helper class
 * wraps the path of an image
 */
struct ImageSpec
{
    QString m_location;
    bool m_valid;

    ImageSpec(const QString location) : m_location(location) {
        //qDebug() << "ImageSpec" << location;
        m_valid = location != QString("");
    }

    ImageSpec(const ImageSpec &spec) : m_location(spec.m_location), m_valid(spec.m_valid) {
        //qDebug() << "ImageSpec(copy)";
    }

    virtual ~ImageSpec() {
        //qDebug() << "~ImageSpec";
    }
};

/*
 * A gallery is basically a list of ImageSpecs and
 * a currently active index into this list
 */
class Gallery
{
    vector<shared_ptr<ImageSpec>> m_images;
    int m_index = -1;

public:
    Gallery();
    Gallery(const Gallery&) = default;

    void initFromDirectory(QString directory);
    ImageSpec currentImage() const;
    ImageSpec image(int offsetFromCurrent = 0)  const;
    void step(int direction);
};

#endif // GALLERY_H
