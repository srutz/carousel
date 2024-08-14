#include "gallery.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "toast.h"
#include "galleryview.h"

#include <QShortcut>
#include <QFileDialog>
#include <QClipboard>

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
    connect(ui->actionCopyToClipboard, &QAction::triggered, this, [=]() {
        auto image = galleryView->getImage();
        if (image.size().width() > 0) {
            auto clipboard = QApplication::clipboard();
            clipboard->setPixmap(image);
            auto toast = new Toast(this);
            toast->showMessage(
                QString("Image of size %1 x %2 copied to clipboard")
                    .arg(image.size().width()).arg(image.size().height()), 3000);
        }
    });
    connect(ui->actionCopyFilepathToClipboard , &QAction::triggered, this, [=]() {
        auto imagePath = galleryView->getImagePath();
        if (imagePath.length() > 0) {
            auto clipboard = QApplication::clipboard();
            clipboard->setText(imagePath);
            auto toast = new Toast(this);
            toast->showMessage(QString("Filepath copied to clipboard"), 3000);
        }
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
    {
        QShortcut *shortcut= new QShortcut(QKeySequence("CTRL+C"), this);
        connect(shortcut, &QShortcut::activated, ui->actionCopyToClipboard, &QAction::trigger);
    }

    auto layout = new QVBoxLayout(this);
    ui->content->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(galleryView);

    Gallery gallery;
    gallery.initFromDirectory(".");
    //gallery.initFromDirectory("/home/sr/Pictures/Screenshots");
    //gallery.initFromDirectory("/home/sr/tmp3");
    galleryView->setGallery(gallery);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(ui->actionChooseDirectory);
    menu.addSeparator();
    menu.addAction(ui->actionCopyToClipboard);
    menu.addAction(ui->actionCopyFilepathToClipboard);
    menu.exec(event->globalPos());
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
