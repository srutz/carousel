#include "galleryview.h"
#include <memory>
#include <QApplication>
#include <QStyle>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileInfo>
#include <QTimer>




GalleryView::GalleryView(QWidget *parent)
    : QWidget{parent}
    , m_gallery(Gallery())

{
    m_imageViewContainer = new QWidget(this);
    for (int i = 0; i < 3; i++) {
        this->m_imageViews.push_back(make_unique<ImageView>(m_imageViewContainer));
    }
    this->m_leftNavigation = new QWidget(this);
    this->m_rightNavigation = new QWidget(this);
    auto setupNavigation = [=](QWidget *widget, const QIcon icon, auto onClick) {
        auto layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(layout);
        auto button = new QPushButton(widget);
        const int ICON_SIZE = 48;
        button->setIcon(icon);
        button->setFixedSize(ICON_SIZE, ICON_SIZE);
        button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
        button->setStyleSheet("QPushButton { border: none; }");
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, widget, [=]() {
            onClick();
        });
    };
    setupNavigation(m_leftNavigation, QIcon(":/images/images/back_48.png"), [=]() { step(-1); });
    setupNavigation(m_rightNavigation, QIcon(":/images/images/forward_48.png"), [=]() { step(1); });
    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    m_statusLabel->setStyleSheet("QLabel { color : #efefef; }");


    auto horizontalWrapper = new QWidget(this);
    auto horizontalLayout = new QHBoxLayout(this);
    horizontalWrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    horizontalWrapper->setLayout(horizontalLayout);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->addWidget(m_leftNavigation);
    horizontalLayout->addWidget(m_imageViewContainer, 1);
    horizontalLayout->addWidget(m_rightNavigation);

    auto outerLayout = new QVBoxLayout(this);
    this->setLayout(outerLayout);
    outerLayout->setContentsMargins(0, 4, 0, 4);
    outerLayout->setSpacing(4);
    outerLayout->addWidget(horizontalWrapper);
    outerLayout->addWidget(m_statusLabel);
    QTimer::singleShot(0, [=]() { layoutImages(); });
}


void GalleryView::layoutImages()
{
    auto size = m_imageViewContainer->size();
    auto x = -size.width();
    auto y = 0;

    for (auto &view : m_imageViews) {
        view->setFixedSize(size.width(), size.height());
        view->setPosition(QPoint(x, y));
        view->show();
        x += size.width();
    }
}


void GalleryView::resizeEvent(QResizeEvent * event)
{
    layoutImages();
}


void GalleryView::paintEvent(QPaintEvent *event)
{
}


void GalleryView::setGallery(const Gallery& gallery)
{
    this->m_gallery = gallery;
    rehashImage();
}


QPixmap GalleryView::getImage() const
{
    auto spec = m_gallery.currentImage();
    return spec.m_valid ? m_imageViews.at(1)->getImage() : QPixmap(0, 0);
}


QString GalleryView::getImagePath() const
{
    auto spec = m_gallery.currentImage();
    return spec.m_valid ? spec.m_location : "";
}


void GalleryView::step(int direction)
{
    if (m_transitioning) {
        return;
    }
    auto size = m_imageViewContainer->size();
    auto duration = 250;
    if (direction > 0) {
        m_transitioning = true;
        m_imageViews[0]->setPositionA(QPoint(0, 0), duration);
        m_imageViews[1]->setPositionA(QPoint(size.width(), 0), duration, [=] {
            m_gallery.step(direction);
            rehashImage();
            layoutImages();
            m_transitioning = false;
        });
    } else if (direction < 0) {
        m_transitioning = true;
        m_imageViews[2]->setPositionA(QPoint(0, 0), duration);
        m_imageViews[1]->setPositionA(QPoint(-size.width(), 0), duration, [=] {
            m_gallery.step(direction);
            rehashImage();
            layoutImages();
            m_transitioning = false;
        });
    }
}

void GalleryView::rehashImage()
{
    auto currentImage = m_gallery.image(0);
    auto prevImage = m_gallery.image(-1);
    auto nextImage = m_gallery.image(1);
    setImageSpec(nextImage, m_imageViews.at(0).get());
    setImageSpec(currentImage, m_imageViews.at(1).get(), true);
    setImageSpec(prevImage, m_imageViews.at(2).get());
}


void GalleryView::setImageSpec(const ImageSpec &spec, ImageView *imageView, bool applyTitle)
{
    if (spec.m_location == QString("")) {
        imageView->showErrorImage();
        if (applyTitle) {
            m_statusLabel->setText(QString("Image cannot be displayed"));
            setTitle("");
        }
    } else {
        imageView->showFile(spec.m_location);
        if (applyTitle) {
            m_statusLabel->setText(spec.m_location);
            QFileInfo fi(spec.m_location);
            setTitle(fi.baseName());
        }
    }
}


void GalleryView::setTitle(QString s)
{
    auto window = QApplication::activeWindow();
    if (window) {
        window->setWindowTitle(s);
    } else {
        // we are too fast, app isnt build yet
        QTimer::singleShot(100, [=]() {
            auto window = QApplication::activeWindow();
            if (window) {
                window->setWindowTitle(s);
            }
        });
    }
}


