#ifndef SEABATTLE_FIELD_H
#define SEABATTLE_FIELD_H

#include "Ship.h"

class Field {

public:
    Field(Ship **mas_ships);

    int matrix[10][10];
};


#endif //SEABATTLE_FIELD_H
