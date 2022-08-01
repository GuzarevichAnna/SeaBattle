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

class PositioningWindow : public QWidget
{
    Q_OBJECT

public:
    PositioningWindow();

private:
    void paintEvent(QPaintEvent *event) override;
    void PaintEmptyField();
    void PaintRects();
    void MoveRects(QRect* rect);
    QRect CalculateSuggestedPos(QRect* rect);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    enum Rects{
        FOUR_DECKER_RECT,
        THREE_DECKER_RECT_1,
        THREE_DECKER_RECT_2,
        TWO_DECKER_RECT_1,
        TWO_DECKER_RECT_2,
        TWO_DECKER_RECT_3,
        ONE_DECKER_RECT_1,
        ONE_DECKER_RECT_2,
        ONE_DECKER_RECT_3,
        ONE_DECKER_RECT_4,
        NONE
    };

protected slots:
    void onFinish();

private:
    const int length = 400;
    const int deck_size = length/10;
    QRect* field_rect;
    QPushButton* finishButton;
    MainGameWindow* gameWindow;

    QRect* four_decker_rect;
    QRect* three_decker_rect_1;
    QRect* three_decker_rect_2;
    QRect* two_decker_rect_1;
    QRect* two_decker_rect_2;
    QRect* two_decker_rect_3;
    QRect* one_decker_rect_1;
    QRect* one_decker_rect_2;
    QRect* one_decker_rect_3;
    QRect* one_decker_rect_4;

    QPoint previous_point;
    QPoint cur_point;

    int which_ship = NONE;

    bool isInField = false;
};

#endif //SEABATTLE_POSITIONINGWINDOW_H
