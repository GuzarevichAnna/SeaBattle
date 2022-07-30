#ifndef SEABATTLE_POSITIONINGWINDOW_H
#define SEABATTLE_POSITIONINGWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QMouseEvent>
#include <QRect>
#include <QDrag>
#include <QPushButton>
#include <QVBoxLayout>
#include "MainGameWindow.h"

class PositioningWindow : public QWidget
{
    Q_OBJECT

public:
    PositioningWindow();

private:
    void paintEvent(QPaintEvent *event) override;
    void PaintEmptyField();

protected slots:
    void onFinish();

private:
    const int length = 400;
    const int deck_size = length/10;
    QPoint* corner;
    QPushButton* finishButton;
    MainGameWindow* gameWindow;

};

#endif //SEABATTLE_POSITIONINGWINDOW_H
