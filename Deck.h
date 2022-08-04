#ifndef SEABATTLE_DECK_H
#define SEABATTLE_DECK_H

#include "Coordinates.h"

class Deck {
public:

    Deck(Coordinates coord_, int id_) : coord(coord_) {
        id = id_;
        state = SAFE_DECK;
    }

    int id;
    Coordinates coord;

private:
    enum State{
        SAFE_DECK = 0,
        SHOT_DECK = 1
    };
    State state;


};


#endif //SEABATTLE_DECK_H
