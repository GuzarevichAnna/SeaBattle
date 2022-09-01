
#include "Field.h"

Field::Field(Ship **mas_ships) {

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < 10; i++) {
        Ship *cur_ship = mas_ships[i];
        for (int j = 0; j < cur_ship->size; j++) {
            Deck *cur_deck = cur_ship->mas_decks[j];
            matrix[cur_deck->coord.getX()][cur_deck->coord.getY()] = (i + 1) * 10 + (j + 1);
        }
    }
}
