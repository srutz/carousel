
#ifndef PIXMAPLABEL_H
#define PIXMAPLABEL_H

#include <QLabel>
#include <QString>


class PixmapLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString pixmapPath READ pixmapPath WRITE setPixmapPath NOTIFY pixmapPathChanged)
    QString m_pixmapPath;

public:
    explicit PixmapLabel(QWidget *parent = nullptr);

    QString pixmapPath() const;
    void setPixmapPath(const QString &path);

signals:
    void pixmapPathChanged(const QString &path);
};

#endif // PIXMAPLABEL_H
