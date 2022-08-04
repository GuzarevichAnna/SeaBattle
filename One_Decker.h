#ifndef SEABATTLE_ONE_DECKER_H
#define SEABATTLE_ONE_DECKER_H

#include "Ship.h"

class One_Decker : public Ship {

    One_Decker(int start_x, int start_y);

private:
    Deck* deck1;


};


#endif //SEABATTLE_ONE_DECKER_H
