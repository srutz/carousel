#ifndef GALLERYVIEW_H
#define GALLERYVIEW_H

#include "imageview.h"
#include "gallery.h"
#include <QLabel>
#include <QWidget>

class GalleryView : public QWidget
{
    Q_OBJECT

    ImageView *m_imageView;
    QWidget *m_leftNavigation;
    QWidget *m_rightNavigation;
    Gallery m_gallery;
    QLabel *m_statusLabel;

public:
    explicit GalleryView(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

    void step(int direction);
    void rehashImage();
    void setGallery(const Gallery& gallery);
    void setTitle(QString s);

signals:
};

#endif // GALLERYVIEW_H
