#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H


#include <QWidget>
#include <QPaintEvent>

class ImageView : public QWidget
{
    Q_OBJECT

    QPixmap m_pixmap;
    QPixmap m_errorPixmap;
    QString m_label;

public:
    explicit ImageView(QWidget *parent = nullptr);

    void showPixmap(QPixmap);
    void showErrorImage();
    void showFile(QString path);
    void paintEvent(QPaintEvent *event) override;

    QString label();
    void setLabel(const QString &l);

signals:
};

#endif // IMAGEVIEW_H
