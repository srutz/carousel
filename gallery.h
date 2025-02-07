#ifndef GALLERY_H
#define GALLERY_H

#include <QString>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
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
    qint64 m_size = 0;
    qint64 m_lastModified = 0; // msec since epoch
    QString m_filename;
    QString m_directory;

    ImageSpec(const QString location) : m_location(location) {
        //qDebug() << "ImageSpec" << location;
        m_valid = location != QString("");
        if (m_valid) {
            QFile f(location);
            m_size = f.size();
            m_lastModified = f.fileTime(QFileDevice::FileModificationTime).toMSecsSinceEpoch();
            m_filename = f.fileName();
            m_directory = QFileInfo(location).absoluteDir().absolutePath();
        }
    }

    //ImageSpec(const ImageSpec &spec) : m_location(spec.m_location), m_valid(spec.m_valid) {
    //   //qDebug() << "ImageSpec(copy)";
    //}
    // default copy constructor is fine

    

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
    vector<shared_ptr<ImageSpec>> specs() const;
};

#endif // GALLERY_H
