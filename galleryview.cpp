#include "galleryview.h"

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
    this->m_imageView = new ImageView(this);
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
    horizontalLayout->addWidget(m_imageView, 1);
    horizontalLayout->addWidget(m_rightNavigation);

    auto outerLayout = new QVBoxLayout(this);
    this->setLayout(outerLayout);
    outerLayout->setContentsMargins(0, 4, 0, 4);
    outerLayout->setSpacing(4);
    outerLayout->addWidget(horizontalWrapper);
    outerLayout->addWidget(m_statusLabel);
}


void GalleryView::paintEvent(QPaintEvent *event)
{
}


void GalleryView::setGallery(const Gallery& gallery)
{
    this->m_gallery = gallery;
    rehashImage();
}



void GalleryView::step(int direction)
{
    m_gallery.step(direction);
    rehashImage();
}

void GalleryView::rehashImage()
{
    auto currentImage = m_gallery.currentImage();
    if (currentImage.m_location == QString("")) {
        m_imageView->showErrorImage();
        m_statusLabel->setText(QString("Image cannot be displayed"));
        setTitle("");
    } else {
        m_imageView->showFile(currentImage.m_location);
        m_statusLabel->setText(currentImage.m_location);
        QFileInfo fi(currentImage.m_location);
        setTitle(fi.baseName());
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

