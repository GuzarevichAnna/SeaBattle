#ifndef SEABATTLE_MAINGAMEWINDOW_H
#define SEABATTLE_MAINGAMEWINDOW_H

#include <QPainter>
#include <QMouseEvent>
#include <Windows.h>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "Field.h"


class MainGameWindow : public QWidget {

Q_OBJECT

public:

    MainGameWindow();

    void SetUpWindow(Ship **mas_ships_user_, Ship **mas_ships_robot_);

private:

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

    void PaintRemainingShips();

    bool isGameOver(Ship **mas_ships);

    bool EventFilter(QEvent *event);

    void LogicForRobot();

private slots:

    void TimerAlarm();

    void onNewGame();

signals:

    void openPosWindow();

private:

    const int length = 400;
    const int deck_size = length / 10;

    QPoint *corner_user;
    QPoint *corner_robot;

    Field *field_user;
    Field *field_robot;

    QRect *field_rect_user;
    QRect *field_rect_robot;

    Ship *mas_ships_user[10];
    Ship *mas_ships_robot[10];

    QPushButton *newGameButton;

    bool isUsersTurn;
    bool control;
    bool isShootingInProcess;
    bool gameOver;

    QTimer *timer;

    int suggestedX = -1;
    int suggestedY = -1;

    Coordinates suggestedCoord = Coordinates(-1, -1);

    QLabel *gameOverLabel;
};


#endif //SEABATTLE_MAINGAMEWINDOW_H
