#ifndef SEABATTLE_MAINGAMEWINDOW_H
#define SEABATTLE_MAINGAMEWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QMouseEvent>
#include <QRect>
#include <QBrush>
#include "Field.h"
#include "Coordinates.h"
#include <Windows.h>
#include <QTimer>
#include <QPolygon>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>


class MainGameWindow : public QWidget {

    Q_OBJECT

public:
    //MainGameWindow(Ship** mas_ships_user_, Ship** mas_ships_robot_);
    MainGameWindow();

    void SetUpWindow(Ship** mas_ships_user_, Ship** mas_ships_robot_);

    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void PaintEmptyFields();
    void PaintShips();
    void PaintPosRobot();
    void PaintPosUser();
    void PaintTriangle();
    void PaintShotUsersShips();
    void PaintShotRobotsShips();
    bool isGameOver(Ship** mas_ships);


    bool EventFilter(QEvent *event);

private slots:
    void TimerAlarm();
    void onNewGame();

signals:
    void openPosWindow();

private:
    const int length = 400;
    const int deck_size = length/10;
    
    QPoint* corner_user;
    QPoint* corner_robot;

    Field* field_user;
    Field* field_robot;

    QRect* field_rect_user;
    QRect* field_rect_robot;

    Ship* mas_ships_user[10];
    Ship* mas_ships_robot[10];

    QPushButton * newGameButton;

    bool isUsersTurn;
    bool control;
    bool isShootingInProcess;
    bool gameOver;

    QTimer* timer;

    int suggestedX = 1;
    int suggestedY = 2;

    Coordinates suggestedCoord;

    QLabel* gameOverLabel;
};


#endif //SEABATTLE_MAINGAMEWINDOW_H
