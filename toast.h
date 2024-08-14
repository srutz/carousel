#ifndef TOAST_H
#define TOAST_H

#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>


/*
 * A popup message shown briefly and then disappearing
 *
 * The widget will deallocate itself after its been hidden
 */

class Toast : public QWidget {
public:
    Toast(QWidget* parent = nullptr) : QWidget(parent) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_ShowWithoutActivating);

        QVBoxLayout* layout = new QVBoxLayout(this);
        label = new QLabel(this);
        label->setStyleSheet("QLabel { color : white; background-color: #444444; padding: 32px; border-radius: 5px; }");
        layout->addWidget(label);
        setLayout(layout);
    }

    ~Toast() {
        //qDebug() << "~Toast";
    }

    void showMessage(const QString& message, int timeout = 2000) {
        label->setText(message);
        adjustSize();

        if (parentWidget() && parentWidget()->window()) {
            auto parent = parentWidget()->window();
            int x = parent->geometry().center().x() - (width() / 2);
            int y = parent->geometry().y() + 16;
            move(x, y);
        }
        QTimer::singleShot(0, this, [=]() {
            show();
        });

        QTimer::singleShot(timeout, this, [=] () {
            this->dismissAndFree();
        });
    }

    void mousePressEvent(QMouseEvent *event) override {
        this->dismissAndFree();
    }

    void dismissAndFree() {
        this->close();
        this->deleteLater();
    }

private:
    QLabel* label;
};


#endif // TOAST_H
