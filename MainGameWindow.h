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


class MainGameWindow : public QMainWindow {

    Q_OBJECT

public:
    MainGameWindow(Ship** mas_ships_user, Ship** mas_ships_robot);
    ~MainGameWindow()= default;

    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;


    void PaintEmptyFields();
    void PaintShips();

private:
    const int length = 400;
    const int deck_size = length/10;
    
    QPoint* corner_user;
    QPoint* corner_robot;

    Field* field_user;
    Field* field_robot;

    QRect* field_rect_user;
    QRect* field_rect_robot;
};


#endif //SEABATTLE_MAINGAMEWINDOW_H
