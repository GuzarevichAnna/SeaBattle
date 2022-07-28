
#ifndef SEABATTLE_COORDINATES_H
#define SEABATTLE_COORDINATES_H

class Coordinates{

public:
    Coordinates(int x_, int y_){
        x = x_;
        y = y_;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

private:
    int x;
    int y;
};

#endif //SEABATTLE_COORDINATES_H
