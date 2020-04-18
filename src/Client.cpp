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
#include "Client.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size) {
    if (player < 1 || player > 2) {
        // Throw exception if there are more than two players.
        throw ClientWrongPlayerNumberException();
    } else {
        // Set some private and public variables.
        this->player = player;
        this->board_size = board_size;
        this->board_name = "player_" + to_string(player) + ".action_board.json";
        // Generate a vector of 0's the size of the board
        vector<int> vec(this->board_size, 0);
        // Generate the full 2D array representation.
        vector<vector<int>> board(this->board_size, vec);
        ofstream outFile;
        outFile.open(this->board_name, ofstream::out);
        if (outFile) {
            {
                cereal::JSONOutputArchive archive(outFile);
                // Serialize to JSON
                archive(CEREAL_NVP(board));
            }
            // Close file since we've finished accessing it.
            outFile.close();
            initialized = true;
        } else {
            cout << "Couldn't find the file " << this->board_name << endl;
        }
    }
}


void Client::fire(unsigned int x, unsigned int y) {
    // Generate file name string.
    string shotFileName = "player_" + to_string(player) + ".shot.json";
    ofstream shotFile;
    // Open file
    shotFile.open(shotFileName, ofstream::out);
    cereal::JSONOutputArchive archive(shotFile);
    // Serialize to JSON
    archive(CEREAL_NVP(x), CEREAL_NVP(y));
    shotFile.flush();
}


bool Client::result_available() {
    // Generate file name string.
    string resultFileName = "player_" + to_string(player) + ".result.json";
    ifstream resultFile;
    // Open file
    resultFile.open(resultFileName);
    if (resultFile) {
        return true;
    }
    return false;
}


int Client::get_result() {
    if (result_available()) {
        int result = 0;
        // Generate file name string
        string resultFileName = "player_" + to_string(player) + ".result.json";
        ifstream resultFile;
        // Open file
        resultFile.open(resultFileName);
        // Deserialize JSON
        cereal::JSONInputArchive archive(resultFile);
        archive(result);
        // Close file since we're done with it.
        resultFile.close();
        // Remove result file
        remove(resultFileName.c_str());
        if (result == HIT || result == MISS || result == OUT_OF_BOUNDS) {
            return result;
        } else {
            throw ClientException("Result number is invalid!");
        }
    }
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    // Generate 2D representation of the board.
    vector<int> vec(this->board_size, 0);
    vector<vector<int>> board(this->board_size, vec);

    ifstream inFile;
    // Open file
    inFile.open(this->board_name);
    if (inFile) {
        cereal::JSONInputArchive archive(inFile);
        archive(board);
    }

    // Update the board at (x, y) from the result variable.
    board[x][y] = result;

    ofstream outFile;
    // Open file
    outFile.open(this->board_name);
    if (outFile) {
        cereal::JSONOutputArchive archive(outFile);
        archive(CEREAL_NVP(board));
    }
}


string Client::render_action_board() {
    // Generate the 2D representation of the board.
    vector<int> vec(this->board_size, 0);
    vector<vector<int>> board(this->board_size, vec);

    ifstream inFile;
    // Open file
    inFile.open(this->board_name);
    if (inFile) {
        cereal::JSONInputArchive archive(inFile);
        archive(board);
    }

    string result;

    for (int j = 0; j < board_size; j++) {
        for (int k = 0; k < board_size; k++) {
            result += to_string(board[j][k]);
        }
        result += "\n";
    }

    return result;
}