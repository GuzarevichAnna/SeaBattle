#ifndef SEABATTLE_FIELD_H
#define SEABATTLE_FIELD_H

#include <vector>
#include "One_Decker.h"
#include "Ship.h"

class Field {

public:
    Field(Ship** mas_ships);
    ~Field() = default;

//    enum Cell{
//        EMPTY = 0,
//        SHIP = 1,
//        SHOT_SHIP = 2,
//        MISS = 3
//    };


    int matrix[10][10];
};


#endif //SEABATTLE_FIELD_H
