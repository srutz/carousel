#ifndef GALLERYVIEW_H
#define GALLERYVIEW_H

#include "imageview.h"
#include "gallery.h"
#include <memory>
#include <vector>
#include <QLabel>
#include <QWidget>

using namespace std;

/*
 * Shows an image-gallery. this means one image of the gallery
 * is shown and others are accessible via buttons and can be
 * navigated to
 */
class GalleryView : public QWidget
{
    Q_OBJECT

    vector<unique_ptr<ImageView>> m_imageViews;
    QWidget *m_leftNavigation;
    QWidget *m_rightNavigation;
    QWidget *m_imageViewContainer;
    Gallery m_gallery;
    QLabel *m_statusLabel;
    bool m_transitioning = false;

    void setImageSpec(const ImageSpec &spec, ImageView *imageView, bool appylTitle = false);
    void layoutImages();
    void rehashImage();

public:
    explicit GalleryView(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    QPixmap getImage() const;
    QString getImagePath() const;
    void step(int direction);
    void setGallery(const Gallery& gallery);
    void setTitle(QString s);


signals:
};

#endif // GALLERYVIEW_H
