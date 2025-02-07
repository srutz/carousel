#include "albumview.h"
#include <memory>
#include <QApplication>
#include <QStyle>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileInfo>
#include <QTimer>
#include <QLabel>
#include <QHeaderView>
#include <QTableWidget>
#include "imagespectablemodel.h"


AlbumView::AlbumView(QWidget *parent)
    : QWidget{parent}
{
    auto outerLayout = new QVBoxLayout(this);
    this->setStyleSheet("background-color: #ffffff;");
    this->setLayout(outerLayout);
    outerLayout->setContentsMargins(QMargins(12, 12, 12, 12));
    outerLayout->setSpacing(4);
    
    table = new QTableView(this);
    outerLayout->addWidget(table, 1);
    //QTimer::singleShot(0, [=,this]() { layoutImages(); });
}


void AlbumView::layoutImages()
{
}


void AlbumView::resizeEvent(QResizeEvent * event)
{
    layoutImages();
}

void AlbumView::paintEvent(QPaintEvent *event)
{
}

void AlbumView::setGallery(shared_ptr<Gallery> gallery)
{
    this->m_gallery = gallery;

    // Create a table model
    auto model = new ImageSpecTableModel(this);
    model->setImageSpecs(gallery->specs());
    table->setModel(model);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    layoutImages();
}



