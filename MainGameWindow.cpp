
#include "MainGameWindow.h"
#include <QDebug>

MainGameWindow::MainGameWindow(Ship** mas_ships_user, Ship** mas_ships_robot) {
    field_user = new Field(mas_ships_user);
    field_robot = new Field(mas_ships_robot);
    corner_user = new QPoint(70, 70);
    corner_robot = new QPoint(600, 70);
    isUsersTurn = true;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerAlarm()));
    control = false;
}

void MainGameWindow::paintEvent(QPaintEvent *) {
    PaintEmptyFields();
    PaintShips();
}

void MainGameWindow::mousePressEvent(QMouseEvent *event) {
    if(EventFilter(event)) return;

    srand(time(NULL));
    if (event->button() == Qt::LeftButton) {

        if (field_rect_robot->contains(event->pos())) {
            Coordinates shoot = Coordinates::TranslateCoordinates(event->pos(), *corner_robot, deck_size);
            if (field_robot->matrix[shoot.getX()][shoot.getY()] > 0) {
                field_robot->matrix[shoot.getX()][shoot.getY()] = -2;
            } else if (field_robot->matrix[shoot.getX()][shoot.getY()] == 0) {
                field_robot->matrix[shoot.getX()][shoot.getY()] = -1;
                isUsersTurn = false;
            }
            repaint();
        }

        while(!isUsersTurn) {
            control = true;
            Sleep(3000);
            timer->start(100);

            int x, y;
            x = rand() % 10;
            y = rand() % 10;
            if (field_user->matrix[x][y] > 0) {
                field_user->matrix[x][y] = -2;
            } else if (field_user->matrix[x][y] == 0) {
                field_user->matrix[x][y] = -1;
                isUsersTurn = true;
            }

            repaint();
        }
    }
}

void MainGameWindow::PaintEmptyFields() {

    QPainter painter(this);
    QPen blackPen(QColorConstants::Svg::black);
    painter.setPen(blackPen);

    QSize field_size(length, length);

    field_rect_user = new QRect(*corner_user, field_size);
    painter.drawRect(*field_rect_user);

    for (int i = 1; i < 10; i++) {
        painter.drawLine(*corner_user + QPoint(deck_size * i, 0), *corner_user + QPoint(deck_size * i, length));
        painter.drawLine(*corner_user + QPoint(0, deck_size * i), *corner_user + QPoint(length, deck_size * i));
    }

    field_rect_robot = new QRect(*corner_robot, field_size);
    painter.drawRect(*field_rect_robot);

    for (int i = 1; i < 10; i++) {
        painter.drawLine(*corner_robot + QPoint(deck_size * i, 0), *corner_robot + QPoint(deck_size * i, length));
        painter.drawLine(*corner_robot + QPoint(0, deck_size * i), *corner_robot + QPoint(length, deck_size * i));
    }
}

void MainGameWindow::PaintShips() {
    QPainter painter(this);
    QPen bluePen(QColorConstants::Svg::blue, 2);
    QPen darkBluePen(QColorConstants::Svg::darkblue);
    QPen redPen(QBrush(QColorConstants::Svg::red), 2);
    QBrush blueBrush(QColorConstants::Svg::lightskyblue);
    QBrush darkBlueBrush(QColorConstants::Svg::darkblue);

    for (int column = 0; column < 10; column++) {
        for (int row = 0; row < 10; row++) {
            if (field_user->matrix[column][row] > 0) {

                painter.setPen(bluePen);
                painter.setBrush(blueBrush);
                painter.drawRect(corner_user->x() + deck_size * column, corner_user->y() + deck_size * row,
                                 deck_size,
                                 deck_size);

            } else if (field_user->matrix[column][row] == -2){

            painter.setPen(bluePen);
            painter.setBrush(blueBrush);
            painter.drawRect(corner_user->x() + deck_size * column, corner_user->y() + deck_size * row,
                             deck_size,
                             deck_size);

            painter.setPen(redPen);
            painter.drawLine(QPoint(corner_user->x() + deck_size * column + 2, corner_user->y() + deck_size * row + 2),
                             QPoint(corner_user->x() + deck_size * (column + 1) - 2,
                                    corner_user->y() + deck_size * (row + 1) - 2));
            painter.drawLine(
                    QPoint(corner_user->x() + deck_size * (column + 1) - 2, corner_user->y() + deck_size * row + 2),
                    QPoint(corner_user->x() + deck_size * (column) + 2,
                           corner_user->y() + deck_size * (row + 1) - 2));
        } else if (field_user->matrix[column][row] == -1){

                    painter.setPen(darkBluePen);
                    painter.setBrush(darkBlueBrush);
                    painter.drawEllipse(QPoint(corner_user->x() + deck_size * column + deck_size * 0.5,
                                               corner_user->y() + deck_size * row + deck_size * 0.5), 4, 4);
            }
        }
    }


    for (int column = 0; column < 10; column++) {
        for (int row = 0; row < 10; row++) {

             if (field_robot->matrix[column][row] == -2){


                painter.setPen(redPen);
                painter.drawLine(QPoint(corner_robot->x() + deck_size * column + 2, corner_robot->y() + deck_size * row + 2),
                                 QPoint(corner_robot->x() + deck_size * (column + 1) - 2,
                                        corner_robot->y() + deck_size * (row + 1) - 2));
                painter.drawLine(
                        QPoint(corner_robot->x() + deck_size * (column + 1) - 2, corner_robot->y() + deck_size * row + 2),
                        QPoint(corner_robot->x() + deck_size * (column) + 2,
                               corner_robot->y() + deck_size * (row + 1) - 2));
            } else if (field_robot->matrix[column][row] == -1){

                painter.setPen(darkBluePen);
                painter.setBrush(darkBlueBrush);
                painter.drawEllipse(QPoint(corner_robot->x() + deck_size * column + deck_size * 0.5,
                                           corner_robot->y() + deck_size * row + deck_size * 0.5), 4, 4);
            }
        }
    }
}

void MainGameWindow::TimerAlarm()
{
    control = false;
    timer->stop();
}

bool MainGameWindow::EventFilter(QEvent *event)
{
    if (control && event->type() == QEvent::MouseButtonPress) {
            return true;
        }
    return false;
}