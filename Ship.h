#ifndef SEABATTLE_SHIP_H
#define SEABATTLE_SHIP_H

#include "Deck.h"

class Ship {
public:
    Ship(Deck** mas_decks_, int size_, bool isShipHorizontal_ = true){
        mas_decks = mas_decks_;
        size = size_;
        state = SAFE_SHIP;
        isShipHorizontal = isShipHorizontal_;
    }

    Deck** mas_decks;
    int size;
    bool isShipHorizontal;

    enum State_Ship{
        SAFE_SHIP = 0,
        SHOT_SHIP = 1,
        WOUNDED_SHIP = 2
    };

    State_Ship GetState(){
        return state;
    }

    void SetState(State_Ship state_){
        state = state_;
    }


private:

    State_Ship state;
};

#endif //SEABATTLE_SHIP_H
