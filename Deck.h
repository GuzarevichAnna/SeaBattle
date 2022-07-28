#ifndef SEABATTLE_DECK_H
#define SEABATTLE_DECK_H


class Deck {
public:

    Deck(int x, int y){
        coord_x = x;
        coord_y = y;
        state= SAFE;
    }

    enum State{
        SAFE = 0,
        SHOT = 1
    };

private:
    State state;
    int coord_x;
    int coord_y;
};


#endif //SEABATTLE_DECK_H
