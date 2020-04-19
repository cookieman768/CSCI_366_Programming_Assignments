//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <math.h>
#include "BitArray2D.hpp"
#include "common.hpp"

BitArray2D::BitArray2D(unsigned int rows, unsigned int columns) {
    if(rows == 0 && columns == 0) {
        throw BitArray2DException("You cannot have a 0 by 0 board!");
    }
    int size = rows * columns;
    array = new char[size];
    for(int i = 0; i < size; i++) {
        array[i] = '0';
        // cout << array[i];
    }
}


BitArray2D::~BitArray2D() {

}


bool BitArray2D::get(unsigned int row, unsigned int column){
   // check array bounds
   if(row < 0 || row >= BOARD_SIZE) {
       throw BitArray2DException("Row is out of bounds.");
   } else if(column < 0 || column >= BOARD_SIZE) {
       throw BitArray2DException("Column is out of bounds.");
   } else {
       // get the element
       return get_bit_elem(array, columns, row, column);
   }
}



void BitArray2D::set(unsigned int row, unsigned int column){
   // check array bounds
    if(row < 0 || row >= BOARD_SIZE) {
        throw BitArray2DException("Row is out of bounds.");
    } else if(column < 0 || column >= BOARD_SIZE) {
        throw BitArray2DException("Column is out of bounds.");
    } else {
        // set the element
        set_bit_elem(array, columns, row, column);
    }
}
