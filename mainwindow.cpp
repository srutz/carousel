#include "gallery.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QShortcut>
#include <QFileDialog>

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
    connect(ui->actionChooseDirectory, &QAction::triggered, this, [=]() {
        chooseDirectory();
    });
    connect(ui->actionPreviousImage, &QAction::triggered, this, [=]() {
        galleryView->step(-1);
    });
    {
        QShortcut *shortcut= new QShortcut(QKeySequence("Ctrl+Q"), this);
        connect(shortcut, &QShortcut::activated, this, [] () { QApplication::exit(); });
    }
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
    gallery.initFromDirectory(".");
    galleryView->setGallery(gallery);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::chooseDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        "Select Directory",
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (dir.isEmpty()) {
        qDebug() << "No directory selected.";
        return;
    }

    auto galleryView = this->findChild<GalleryView*>();
    Gallery gallery;
    gallery.initFromDirectory(dir);
    galleryView->setGallery(gallery);
}
