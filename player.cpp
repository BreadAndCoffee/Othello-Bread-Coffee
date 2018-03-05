#include "player.hpp"
#include <vector>
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Board *board = new Board();
Side side;

Player::Player(Side my_side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    // Record the size and initialize the player's 
    // record of the board
    side = my_side; 
    //Board *board = new Board(); 
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    // true if game is finished, automatically exits
    // not sure if this is necessary 
    if(board->isDone())
    {
        exit(0);
    }

    Side oppo_side;
    if(side == BLACK)
    {
        oppo_side = WHITE;
    }
    else
    {
        oppo_side = BLACK;
    }

    // update board based on opponent move
    // opponentsMove param in this function, side declared in constructor
    board->doMove(opponentsMove, oppo_side);

    // if there are no valid moves, return nullptr
    if(!board->hasMoves(side))
    {
        return nullptr;
    } 

    // initialize a vector to store all possible moves 
    vector<Move*> possible_moves;  

    // iterate through positions to check if the move is valid
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Move *move = new Move(i, j); 
            // if the move is valid, add it to possible_moves
            if(board->checkMove(move, side))
            {
                possible_moves.push_back(move); 
            }
        }
    }

    Move *final_move = possible_moves[0];
    board->doMove(final_move, side);
    // return the first element (essentially random)
    return final_move; 
}
