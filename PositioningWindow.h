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
#include "Coordinates.h"
#include <algorithm>

class PositioningWindow : public QWidget {
Q_OBJECT

public:
    PositioningWindow();

private:
    void PaintEmptyField();

    void PaintRects();

    void MoveRects(QRect *rect);

    QRect CalculateSuggestedPos(QRect *rect);

    bool CheckPos(const QRect &rect);

    QPoint GetStartPos(int i);

    void EditMatrix(QRect* rect, int value);

    void GenerateShipsForRobot(Ship** mas_ships);

    bool CheckLocRobot(Coordinates* suggested_loc, int length);

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;


private slots:

    void onFinish();
    void onAuto();
    void newGame();

private:
    const int length = 400;
    const int deck_size = length / 10;
    QRect *field_rect;
    QRect *mas_rect[10];
    int matrix[10][10];
    int matrix_robot[10][10];

    QPushButton *finishButton;
    QPushButton *autoButton;
    MainGameWindow *gameWindow;

    QPoint previous_point;
    QPoint cur_point;

    int which_ship = -1;
    bool isInField = false;
    bool isPositioningInProcess = false;
};

#endif //SEABATTLE_POSITIONINGWINDOW_H
