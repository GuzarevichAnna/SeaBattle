#ifndef SEABATTLE_SHIP_H
#define SEABATTLE_SHIP_H

#include "Deck.h"

class Ship {
public:
    Ship(Deck** mas_decks_, int size_){
        mas_decks = mas_decks_;
        //id = id_;
        size = size_;
        state = SAFE_SHIP;
    }

    Deck** mas_decks;
    //int id;
    int size;

    enum State_Ship{
        SAFE_SHIP = 0,
        SHOT_SHIP = 1
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
