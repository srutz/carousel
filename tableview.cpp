#include "tableview.h"
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


TableView::TableView(QWidget *parent)
    : QWidget{parent}
{
    auto outerLayout = new QVBoxLayout(this);
    this->setStyleSheet("background-color: #ffffff;");
    this->setLayout(outerLayout);
    //outerLayout->setContentsMargins(QMargins(12, 12, 12, 12));
    outerLayout->setContentsMargins(QMargins(4, 4, 4, 4));
    outerLayout->setSpacing(4);
    
    table = new QTableView(this);
    table->setStyleSheet("QTableView { border: none; }");
    outerLayout->addWidget(table, 1);
    //QTimer::singleShot(0, [=,this]() { layoutImages(); });
}


void TableView::layoutImages()
{
}


void TableView::resizeEvent(QResizeEvent * event)
{
    layoutImages();
}

void TableView::paintEvent(QPaintEvent *event)
{
}

void TableView::setGallery(shared_ptr<Gallery> gallery)
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



