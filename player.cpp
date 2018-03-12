#include "player.hpp"
#include <vector>


#define CORNERS 10
#define EDGES 5

using namespace std;

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */

Player::Player(Side my_side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    board = new Board();

    // Record the size and initialize the player's 
    // record of the board
    side = my_side; 
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

vector<Move*> Player::get_possible_moves(Board *board, Side s)
{
    // true if game is finished, automatically exits
    // not sure if this is necessary 
    if(board->isDone())
    {
        exit(0);
    }

    // initialize a vector to store all possible moves 
    vector<Move*> possible_moves;  

    // if there are no valid moves, return nullptr
    if(!board->hasMoves(s))
    {
        possible_moves.push_back(nullptr); 
        return possible_moves; 
    } 

    // iterate through positions to check if the move is valid
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Move *move = new Move(i, j); 
            // if the move is valid, add it to possible_moves
            if(board->checkMove(move, s))
            {
                possible_moves.push_back(move); 
            }
            else
            {
                delete move;
            }
        }
    }
    return possible_moves; 
}

/*
 * @brief returns the score of a given move 
 * 
 * @param the move to find the score for 
 * 
 * @return the score of that move
 * 
 */
int Player::get_score(Board *copy, Move* move, Side s, int original){
    // Outline:
    // make copy of the board 
    // get the original score
    // make a move on that copy
    // get the new score 
    // delete the copy of the board 
    // return the score

    int old_score = original;
    int x = move->getX(); 
    int y = move->getY(); 
    // Copy->doMove(move, s); 
    int new_score = copy->count(s); 
    delete copy; 

    // Check corners 
    if((x == 0 || x == 7) && ((y == 0) || y == 7))
    {
        new_score += CORNERS; 
    }

    // Check adjacent 
    else if((x == 1 || x == 6) && (y == 6 || y ==1))
    {
        new_score -= CORNERS; 
    }

    // Check neighbors
    else if((x == 0 && y == 1) || (x == 0 && y == 6) 
        || (x == 1 && y == 0) || (x == 1 && y == 7)
        || (x == 6 && y == 0) || (x == 6 && y == 7)
        || (x == 7 && y == 1) || (x == 7 && y == 6))
    {
        new_score -= EDGES; 
    }

    // Check the edges (excluding near corner)
    else if (x == 0 || x == 7 || y == 0 || y == 7)
    {
        new_score += EDGES; 
    } 

    return (new_score - old_score);
}

/*
 * @brief returns the index of the best move from the input vector 
 * possible_moves
 *
 * @param the list of possible moves from which to find the best move
 *
 * @return an integer corresponding to the index of the best move
 * 
 */
int Player::bestMove(vector<Move*> possible_moves){
    //Calls get_score for every move in possible_moves
    //Finds the highest
    //Return the index of the highest value 

    // index of move with highest value; initialize to 0
    if (possible_moves.size() < 0)
    {
        return -1;
    }
    int high_index = 0; 
    // highest score of all moves in vector
    // initialized to score of possible_moves[0]
    /*
    int high_score = get_score(possible_moves[0]);
    */
    int high_score = (int)(minimax(board, possible_moves[0], DEPTH, true));

    // iterate over rest of vector
    for (unsigned int i = 1; i < possible_moves.size(); i++)
    {
        int my_score = (int)(minimax(board, possible_moves[i], DEPTH, true));
        // if move has higher score than current highest score
        if(my_score > high_score)
        {
            // update score
            high_score = my_score; 
            // update index
            high_index = i;
        }
    }
    return high_index;
}

float Player::minimax(Board *board, Move *node, int depth, bool maximizing_player)
{
    float bestValue;
    Board *copy = board->copy();

    Side oppo_side;
    if(side == BLACK)
    {
        oppo_side = WHITE;
    }
    else
    {
        oppo_side = BLACK;
    }

    if (maximizing_player)
    {
        int original = copy->count(oppo_side); 
        copy->doMove(node, side); 
        vector<Move*> children = get_possible_moves(board, oppo_side);
        if (depth == 0 || children[0] == nullptr)
        {
            return get_score(copy, node, side, original);
        }
        bestValue = -INFINITY;
        for (unsigned int i = 0; i < children.size(); i++)
        {
            // Board *copy_c = copy->copy();
            // copy_c->doMove(children[i], oppo_side);
            float recur_result = minimax(copy, children[i], depth-1, false);
            // return maximum
            if (recur_result > bestValue)
            {
                bestValue = recur_result;
            }
            //bestValue = (bestValue < recur_result) ?recur_result:bestValue;
        }
        return bestValue;
    }
    else
    {
        int original = copy->count(oppo_side); 
        copy->doMove(node, side); 
        vector<Move*> children = get_possible_moves(board, side);
        if (depth == 0 || children[0] == nullptr)
        {
            return get_score(copy, node, oppo_side, original);
        }
        bestValue = INFINITY;
        for (unsigned int i = 0; i < children.size(); i++)
        {
            // Board *copy_c = copy->copy();
            // copy_c->doMove(children[i], side);
            float recur_result = minimax(copy, children[i], depth-1, true);
            // return minimum
            //bestValue = !(recur_result < bestValue) ?bestValue:recur_result;
            if (recur_result < bestValue)
            {
                bestValue = recur_result;
            }
        }
        return bestValue;
    }
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

    vector<Move*> possible_moves = get_possible_moves(board, side); 

    int index = bestMove(possible_moves);
    if(index == -1)
    {
        exit(0);
    }

    Move *final_move = new Move(possible_moves[index]->getX(), 
        possible_moves[index]->getY());

    board->doMove(final_move, side);

    for(unsigned int i = 0; i < possible_moves.size(); i++)
    {
        delete possible_moves[i]; 
    }
    // return the first element (essentially random)
    return final_move; 
}