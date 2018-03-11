#ifndef __PLAYER_H__
#define __PLAYER_H__
#define DEPTH 3

#include <iostream>
#include <vector>
#include "common.hpp"
#include "board.hpp"

class Player {

public:
    Player(Side my_side);
    ~Player();
    vector<Move*> get_possible_moves(Board *board, Side s); 
    Move *doMove(Move *opponentsMove, int msLeft);
    Move* opp_move(Board *copy); 
    int bestMove(std::vector<Move*> possible_moves);
    int get_score(Move* move);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board * board;
    Side side;
};

#endif
