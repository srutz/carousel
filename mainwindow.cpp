#include "gallery.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStatusBar(nullptr);
    auto galleryView = new GalleryView(this);

    connect(ui->actionExit, &QAction::triggered, this, [=]() {
        QApplication::exit();
    });
    connect(ui->actionPreviousImage, &QAction::triggered, this, [=]() {
        galleryView->step(-1);
    });
    {
        QShortcut *shortcut= new QShortcut(QKeySequence("Left"), this);
        connect(shortcut, &QShortcut::activated, ui->actionPreviousImage, &QAction::trigger);
    }
    connect(ui->actionNextImage, &QAction::triggered, this, [=]() {
        galleryView->step(1);
    });
    {
        QShortcut *shortcut= new QShortcut(QKeySequence("Right"), this);
        connect(shortcut, &QShortcut::activated, ui->actionNextImage, &QAction::trigger);
    }

    auto layout = new QVBoxLayout(this);
    ui->content->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(galleryView);

    Gallery gallery;
    gallery.initFromDirectory("/home/sr/Pictures/Screenshots");
    galleryView->setGallery(gallery);
}

MainWindow::~MainWindow()
{
    delete ui;
}
