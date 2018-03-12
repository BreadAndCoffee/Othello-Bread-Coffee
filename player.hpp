#ifndef __PLAYER_H__
#define __PLAYER_H__
#define DEPTH 3
#define INFINITY 100000000


#include <iostream>
#include <vector>
#include "common.hpp"
#include "board.hpp"

class Player {

public:
    Player(Side my_side);
    ~Player();
    vector<Move*> get_possible_moves(Board *board, Side s);
    float minimax(Board *board, int depth, bool maximizing_player); 
    Move *doMove(Move *opponentsMove, int msLeft);
    Move* opp_move(Board *copy); 
    int bestMove(std::vector<Move*> possible_moves);
    int get_score(Board* copy, Side s);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board * board;
    Side side;
    int array[8][8] = {
            {50,  30,  30,  30,  30,   30,  30,  50}, 
            {30, -50, -10, -10, -10,  -10, -50,  30}, 
            {30, -10,   5,  5,   5,    5,  -10,  30}, 
            {30, -10,   5,  5,   5,    5,  -10,  30}, 
            {30, -10,   5,  5,   5,    5,  -10,  30}, 
            {30, -10,   5,  5,   5,    5,  -10,  30}, 
            {30, -50, -10, -10, -10,  -10, -50,  30}, 
            {50,  30,  30,  30,  30,   30,  30,  50}
            };
};

#endif
