#include "Field.h"

Field::Field(std::vector<Ship*> ships) {
    int x_coord;
    int y_coord;

    matrix.resize(10);
    for(int i = 0; i< 10; i++){
        matrix[i].resize(10);
    }

    for(std::vector row : matrix){
        for(Cell cell : row){
            cell = EMPTY;
        }
    }

    for(Ship* ship : ships){
        for(std::vector cell : ship->location){
            x_coord = cell[0];
            y_coord = cell[1];
            matrix[x_coord][y_coord] = SHIP;
        }
    }
}
