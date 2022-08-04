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
#include <QDebug>
#include "Ship.h"

class PositioningWindow : public QWidget
{
    Q_OBJECT

public:
    PositioningWindow();

private:
    void PaintEmptyField();
    void PaintRects();
    void MoveRects(QRect* rect);
    QRect CalculateSuggestedPos(QRect* rect);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


private slots:
    void onFinish();

private:
    const int length = 400;
    const int deck_size = length/10;
    QRect* field_rect;
    QRect* mas_rect[10];

    QPushButton* finishButton;
    MainGameWindow* gameWindow;

    QPoint previous_point;
    QPoint cur_point;

    int which_ship = -1;
    bool isInField = false;
    bool isPositioningInProcess = false;
};

#endif //SEABATTLE_POSITIONINGWINDOW_H
