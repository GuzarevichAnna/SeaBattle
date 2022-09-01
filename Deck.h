#ifndef SEABATTLE_DECK_H
#define SEABATTLE_DECK_H

#include "Coordinates.h"

class Deck {
public:

    Deck(Coordinates coord_) : coord(coord_) {
        state = SAFE_DECK;
    }

    enum State_Deck {
        SAFE_DECK = 0,
        SHOT_DECK = 1
    };

    State_Deck GetState() {
        return state;
    }

    void SetState(State_Deck state_) {
        state = state_;
    }

    Coordinates coord;

private:
    State_Deck state;
};


#endif //SEABATTLE_DECK_H
