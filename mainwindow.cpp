#include "gallery.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "toast.h"
#include "galleryview.h"
#include "albumview.h"
#include "tableview.h"

#include <memory>
#include <QShortcut>
#include <QFileDialog>
#include <QClipboard>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStatusBar(nullptr);
    auto galleryView = new GalleryView(this);
    auto albumView = new AlbumView(this);
    auto tableView = new TableView(this);

    connect(ui->actionExit, &QAction::triggered, this, [=, this]()
            { QApplication::exit(); });
    connect(ui->actionChooseDirectory, &QAction::triggered, this, [=, this]()
            { chooseDirectory(); });
    connect(ui->actionPreviousImage, &QAction::triggered, this, [=, this]()
            { galleryView->step(-1); });
    connect(ui->actionCopyToClipboard, &QAction::triggered, this, [=, this]()
            {
        auto image = galleryView->getImage();
        if (image.size().width() > 0) {
            auto clipboard = QApplication::clipboard();
            clipboard->setPixmap(image);
            auto toast = new Toast(this);
            toast->showMessage(
                QString("Image of size %1 x %2 copied to clipboard")
                    .arg(image.size().width()).arg(image.size().height()), 3000);
        } });
    connect(ui->actionCopyFilepathToClipboard, &QAction::triggered, this, [=, this]()
            {
        auto imagePath = galleryView->getImagePath();
        if (imagePath.length() > 0) {
            auto clipboard = QApplication::clipboard();
            clipboard->setText(imagePath);
            auto toast = new Toast(this);
            toast->showMessage(QString("Filepath copied to clipboard"), 3000);
        } });
    {
        QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
        connect(shortcut, &QShortcut::activated, this, []()
                { QApplication::exit(); });
    }
    {
        QShortcut *shortcut = new QShortcut(QKeySequence("Left"), this);
        connect(shortcut, &QShortcut::activated, ui->actionPreviousImage, &QAction::trigger);
    }
    connect(ui->actionNextImage, &QAction::triggered, this, [=, this]()
            { galleryView->step(1); });
    {
        QShortcut *shortcut = new QShortcut(QKeySequence("Right"), this);
        connect(shortcut, &QShortcut::activated, ui->actionNextImage, &QAction::trigger);
    }
    {
        QShortcut *shortcut = new QShortcut(QKeySequence("CTRL+C"), this);
        connect(shortcut, &QShortcut::activated, ui->actionCopyToClipboard, &QAction::trigger);
    }

    // setup tabwidget
    auto layout = new QVBoxLayout(this);
    ui->content->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    auto tabWidget = new QTabWidget(this);
    tabWidget->setTabPosition(QTabWidget::South);
    // style tab nicely for being shown on a black background
    tabWidget->setStyleSheet(R"(
        QTabWidget::pane {
            background: orange;
            padding: 0px;
        }

        QTabWidget::tab-bar {
            left: 5px;  /* move the entire tab bar slightly right */
        }

        QTabBar::tab {
            background: #2d2d2d;
            color: #808080;
            padding: 2px 15px;
            border: 1px solid #3d3d3d;
            border-bottom: none;
            border-bottom-left-radius: 4px;
            border-bottom-right-radius: 4px;
            min-width: 80px;
            margin-right: 2px;
        }

        QTabBar::tab:selected {
            background: #000000;
            color: #cccccc;
            border: 1px solid #4d4d4d;
            border-top: none;
        }

        QTabBar::tab:hover:!selected {
            background: #353535;
            color: #b0b0b0;
        }

        QTabBar::tab:disabled {
            background: #1a1a1a;
            color: #505050;
        }

        /* Optional: Style for the tab widget frame */
        QTabWidget {
            background: green;
        }

        /* Style for the content area of each tab */
        QWidget[tabContent="true"] {
            background: #1e1e1e;
            color: #e0e0e0;
        }

        /* Optional: Add a subtle gradient to selected tabs
        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #404040, stop:1 #383838);
        }
        */

        /* Optional: Add a subtle bottom border to the active tab */
        QTabBar::tab:selected {
            border-bottom: 2px solid #5a5a5a;
        }
    )");
    tabWidget->setStyleSheet("");

    layout->addWidget(tabWidget);

    tabWidget->addTab(albumView, "Album View");
    tabWidget->addTab(galleryView, "Single Image View");
    tabWidget->addTab(tableView, "Table View");

    auto gallery = make_shared<Gallery>();
    gallery->initFromDirectory(".");
    // gallery.initFromDirectory("/home/sr/Pictures/Screenshots");
    // gallery.initFromDirectory("/home/sr/tmp3");
    galleryView->setGallery(gallery);
    albumView->setGallery(gallery);
    tableView->setGallery(gallery);
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
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty())
    {
        qDebug() << "No directory selected.";
        return;
    }

    auto galleryView = this->findChild<GalleryView *>();
    auto gallery = make_shared<Gallery>();
    gallery->initFromDirectory(dir);
    galleryView->setGallery(gallery);
}
