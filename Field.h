#ifndef SEABATTLE_FIELD_H
#define SEABATTLE_FIELD_H

#include <vector>
#include "One_Decker.h"
#include "Ship.h"

class Field {

public:
    Field(std::vector<Ship*> ships);
    ~Field() = default;

    enum Cell{
        EMPTY = 0,
        SHIP = 1,
        SHOT_SHIP = 2,
        MISS = 3
    };


    std::vector<std::vector<Cell>> matrix;
};


#endif //SEABATTLE_FIELD_H
