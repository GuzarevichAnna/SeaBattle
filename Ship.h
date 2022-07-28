#ifndef SEABATTLE_SHIP_H
#define SEABATTLE_SHIP_H

#include "Deck.h"
#include <QPoint>
#include <vector>


class Ship {

public:
    //Ship(int start_x, int start_y, int direction);
    Ship(std::vector<std::vector<int>> loc){
        location = loc;
    }
    //virtual void Locate() = 0;

    //virtual void Destroy() = 0;

    std::vector<std::vector<int>> location;

protected:
    int size;

};


#endif //SEABATTLE_SHIP_H
