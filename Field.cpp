#include "Field.h"
#include <QDebug>

Field::Field(Ship **mas_ships) {

    int x_coord;
    int y_coord;


    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = 0;
        }

    }
    for (int i = 0; i < 10; i++) {
        Ship *cur_ship = mas_ships[i];
        for (int j = 0; j < cur_ship->size; j++) {
            Deck *cur_deck = cur_ship->mas_decks[j];
            matrix[cur_deck->coord.getX()][cur_deck->coord.getY()] = cur_ship->id * 10 + cur_deck->id;
        }
    }
}
