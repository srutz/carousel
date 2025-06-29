#ifndef ALBUMVIEW_H
#define ALBUMVIEW_H

#include <memory>
#include <QWidget>
 #include <QScrollArea>
#include "gallery.h"

using namespace std;

/*
 * Shows an image-gallery. this means one image of the gallery
 * is shown and others are accessible via buttons and can be
 * navigated to
 */
class AlbumView : public QWidget
{
    Q_OBJECT
    shared_ptr<Gallery> m_gallery;
    QScrollArea* scrollArea;
    QWidget* content;
    bool m_sizesComputed = false;
    void layoutImages();

    static QSize computeImageSize(const ImageSpec &spec);

public:
    explicit AlbumView(QWidget *parent = nullptr);
    void setGallery(shared_ptr<Gallery>);

    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // ALBUMVIEW_H
