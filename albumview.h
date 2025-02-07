#ifndef ALBUMVIEW_H
#define ALBUMVIEW_H

#include <memory>
#include <QWidget>
#include <QTableView>
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
    QTableView *table;
    void layoutImages();

public:
    explicit AlbumView(QWidget *parent = nullptr);
    void setGallery(shared_ptr<Gallery>);

    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // ALBUMVIEW_H
