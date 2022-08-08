
#ifndef SEABATTLE_COORDINATES_H
#define SEABATTLE_COORDINATES_H

#include <QPoint>

class Coordinates{

public:
    Coordinates(int x_, int y_){
        x = x_;
        y = y_;
    }

    Coordinates(){}



    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    static Coordinates TranslateCoordinates(QPoint point, const QPoint& corner, int deck_size){
            point -= corner;
            int x = point.x() / deck_size;
            int y = point.y() / deck_size;
            return Coordinates(x, y);
    }

private:
    int x;
    int y;
};

#endif //SEABATTLE_COORDINATES_H
