
#include "MainGameWindow.h"

MainGameWindow::MainGameWindow(std::vector<Ship *> ships_user, std::vector<Ship *> ships_robot) {
    field_user = new Field(ships_user);
    field_robot = new Field(ships_robot);
    corner_user = new QPoint(70, 70);
    corner_robot = new QPoint(600, 70);
}

void MainGameWindow::paintEvent(QPaintEvent *) {
    PaintEmptyFields();
    PaintShips();
}


void MainGameWindow::PaintEmptyFields() {

    QPainter painter(this);
    QPen blackPen(QColorConstants::Svg::black);
    painter.setPen(blackPen);

    QSize field_size(length, length);

    QRect rect_user(*corner_user, field_size);
    painter.drawRect(rect_user);

    for (int i = 1; i < 10; i++) {
        painter.drawLine(*corner_user + QPoint(deck_size * i, 0), *corner_user + QPoint(deck_size * i, length));
        painter.drawLine(*corner_user + QPoint(0, deck_size * i), *corner_user + QPoint(length, deck_size * i));
    }

    QRect rect_robot(*corner_robot, field_size);
    painter.drawRect(rect_robot);

    for (int i = 1; i < 10; i++) {
        painter.drawLine(*corner_robot + QPoint(deck_size * i, 0), *corner_robot + QPoint(deck_size * i, length));
        painter.drawLine(*corner_robot + QPoint(0, deck_size * i), *corner_robot + QPoint(length, deck_size * i));
    }
}


void MainGameWindow::PaintShips() {
    QPainter painter(this);
    QPen blackPen(QColorConstants::Svg::black);
    QPen redPen(QBrush(QColorConstants::Svg::red), 2);
    QBrush blueBrush(QColorConstants::Svg::lightskyblue);
    QBrush blackBrush(QColorConstants::Svg::black);
    //painter.setBrush(blueBrush);
    //painter.setPen(blackPen);

    for (int column = 0; column < field_user->matrix.size(); column++) {
        for (int row = 0; row < field_user->matrix.size(); row++) {
            switch (field_user->matrix[column][row]) {

                case Field::SHIP:
                    painter.setPen(blackPen);
                    painter.setBrush(blueBrush);
                    painter.drawRect(corner_user->x() + deck_size * column, corner_user->y() + deck_size * row,
                                     deck_size,
                                     deck_size);
                    break;

                case Field::SHOT_SHIP:
                    painter.setPen(blackPen);
                    painter.setBrush(blueBrush);
                    painter.drawRect(corner_user->x() + deck_size * column, corner_user->y() + deck_size * row,
                                     deck_size,
                                     deck_size);

                    painter.setPen(redPen);
                    painter.drawLine(QPoint(corner_user->x() + deck_size * column+2, corner_user->y() + deck_size * row+2),
                                     QPoint(corner_user->x() + deck_size * (column + 1)-2,
                                            corner_user->y() + deck_size * (row + 1)-2));
                    painter.drawLine(
                            QPoint(corner_user->x() + deck_size * (column + 1)-2, corner_user->y() + deck_size * row+2),
                            QPoint(corner_user->x() + deck_size * (column)+2,
                                   corner_user->y() + deck_size * (row + 1)-2));
                    break;

                case Field::MISS:
                    painter.setPen(blackPen);
                    painter.setBrush(blackBrush);
                    painter.drawEllipse(QPoint(corner_user->x() + deck_size * column + deck_size * 0.5,
                                               corner_user->y() + deck_size * row + deck_size * 0.5), 5, 5);
                    break;
            }
        }
    }

    for (int column = 0; column < field_robot->matrix.size(); column++) {
        for (int row = 0; row < field_robot->matrix.size(); row++) {
            if (field_robot->matrix[column][row] == Field::SHIP) {
                painter.drawRect(corner_robot->x() + deck_size * column, corner_robot->y() + deck_size * row, deck_size,
                                 deck_size);
            }
        }
    }
}

void MainGameWindow::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        if (event->x() > corner_user->x() && event->x() < corner_user->x() + length && event->y() > corner_user->y() &&
            event->y() < corner_user->y() + length) {
            Coordinates shoot = TranslateCoordinates_user(event->pos());
            if (field_user->matrix[shoot.getX()][shoot.getY()] == Field::SHIP) {
                field_user->matrix[shoot.getX()][shoot.getY()] = Field::SHOT_SHIP;
            } else if(field_user->matrix[shoot.getX()][shoot.getY()] == Field::EMPTY){
                field_user->matrix[shoot.getX()][shoot.getY()] = Field::MISS;
            }
        }
    }
    update();

}

Coordinates MainGameWindow::TranslateCoordinates_user(QPoint point) {
    point -= *corner_user;
    int x = point.x() / deck_size;
    int y = point.y() / deck_size;
    return Coordinates(x, y);
}



