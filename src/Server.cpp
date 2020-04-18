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


#include "common.hpp"
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
    return file->gcount();
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board) {
    if (board_size != BOARD_SIZE) {
        throw ServerException("Error! Wrong board size.");
    } else if (p1_setup_board.length() < 1 || p2_setup_board.length() < 1) {
        throw ServerException("Error! Invalid board provided.");
    } else {
        this->board_size = board_size;
        //this->p1_setup_board = ifstream(p1_setup_board);
        //this->p2_setup_board = ifstream(p2_setup_board);
    }
}


Server::~Server() {
}

/**
    * Sets up a BitArray2D pointer with bits filled in based on a setup_board_name
    * @param setup_board_name - the name of the setup board file
    * @return an internally allocated pointer to a BitArray2D object
    */
BitArray2D *Server::scan_setup_board(string setup_board_name){
    ifstream in = ifstream(setup_board_name);
    string line;
    vector<string> lines(board_size, "");
    int i = 0;
    while(getline(in, line)) {
        lines.at(i) = line;
        i++;
    }
    int rows = lines.size();
    int columns = lines.at(0).size();
    in.close();

    BitArray2D *out;
    out = new BitArray2D(rows, columns);

    return out;
}


int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    // Determine if out of bounds
    if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0) {
        return OUT_OF_BOUNDS;
    }

        // Check if player is valid
    else if (player > MAX_PLAYERS || player < 1) {
        throw ServerException("Error! Invalid player number.");
    } else {
        // Check player number and select the other player's board.
        string boardName;
        if (player == 1) {
            boardName = "player_2.setup_board.txt";
        } else {
            boardName = "player_1.setup_board.txt";
        }

        ifstream boardFile;
        boardFile.open(boardName);

        // Iterate through the whole file.
        string current;
        vector<string> lines(board_size, "");
        int i = 0;
        while (getline(boardFile, current)) {
            lines[i] = current;
            i++;
        }

        // Close file, we read the whole file.
        boardFile.close();

        // Get character at (x, y)
        char tile = lines[x].at(y);

        // Check to see if a ship has been hit or not.
        if (tile == 'C' || tile == 'B' || tile == 'R' || tile == 'S' || tile == 'D') {
            return HIT;
        } else {
            return MISS;
        }
    }
}


int Server::process_shot(unsigned int player) {
    if (player == 1 || player == 2) {
        // Generate shot file name from player number.
        string shotFileName = "player_" + to_string(player) + ".shot.json";
        // Initialize variables.
        int x, y, result = 0;
        ifstream shotFile;
        shotFile.open(shotFileName);
        if (!shotFile) {
            return NO_SHOT_FILE;
        } else {
            // Deserialize JSON
            cereal::JSONInputArchive archive(shotFile);
            // Set X and Y from JSON
            archive(y, x);
            // Check if the coordinates are out of bounds
            cout << "X: " << x << " Y: " << y << " Size: " << BOARD_SIZE << endl;
            /*if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0) {
                cout << "x: " << x << " Size: " << BOARD_SIZE;
                result = OUT_OF_BOUNDS;
            } else {
                result = evaluate_shot(player, x, y);
            }*/

            result = evaluate_shot(player, x, y);
            // Close shot JSON since we're done with it.
            shotFile.close();
            string resultFileName = "player_" + to_string(player) + ".result.json";
            ofstream resultFile;
            resultFile.open(resultFileName);
            {
                cereal::JSONOutputArchive outputArchive(resultFile);
                // Serialize the output to JSON
                outputArchive(CEREAL_NVP(result));
            }
            // Delete the shot file
            remove(shotFileName.c_str());
            return SHOT_FILE_PROCESSED;
        }

    } else {
        throw ServerException("Error! Incorrect player number.");
    }
}