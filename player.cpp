#include "player.hpp"
#include <vector>

#define CORNERS 200
#define EDGES 100
#define PIECES 5

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

    // Record the side and initialize the player's 
    // record of the board
    side = my_side; 

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if ((x == 0 || x == 7) && (y == 0 || y == 7))
            {
                array[x][y] = CORNERS;
            }
            // Check adjacent 
            else if((x == 1 || x == 6) && (y == 6 || y ==1))
            {
                array[x][y] = -CORNERS; 
            }

            // Check neighbors
            else if((x == 0 && y == 1) || (x == 0 && y == 6) 
                || (x == 1 && y == 0) || (x == 1 && y == 7)
                || (x == 6 && y == 0) || (x == 6 && y == 7)
                || (x == 7 && y == 1) || (x == 7 && y == 6))
            {
                array[x][y] = -EDGES; 
            }

            // Check the edges (excluding near corner)
            else if (x == 0 || x == 7 || y == 0 || y == 7)
            {
                array[x][y] = EDGES; 
            } 
            else
            {
                array[x][y] = PIECES;
            }
        }
    }
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

/*
 * @brief gets all possible moves for a a particular side 
 * given the board and side
 * 
 * @param Board *curr_board the board in which to look for moves 
 * @param Side s            the side making the move(s)
 * 
 * @return a vector<Move*> containing all the possible moves 
 */
vector<Move*> Player::get_possible_moves(Board *curr_board, Side s)
{
    // initialize a vector to store all possible moves 
    vector<Move*> possible_moves;  

    // iterate through positions to check if the move is valid
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Move *move = new Move(i, j); 
            // if the move is valid, add it to possible_moves
            if(curr_board->checkMove(move, s))
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
int Player::get_score(Board *copy, Side s){
    int val = 0; 
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(copy->get(s, i, j))
            {
                val += array[i][j];
            }
        }
    }
    return val; 
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
int Player::bestMove(vector<Move*> possible_moves)
{
    int high_index = 0;

    Board *copy = board->copy();
    copy->doMove(possible_moves[0], side);
    int high_score = (int)(minimax(copy, DEPTH, false));
    delete copy; 

    // iterate over rest of vector
    for (unsigned int i = 1; i < possible_moves.size(); i++)
    {
        Board *copy = board->copy();
        copy->doMove(possible_moves[i], side);
        int my_score = (int)(minimax(copy, DEPTH, false));
        // if move has higher score than current highest score
        if(my_score > high_score)
        {
            // update score
            high_score = my_score; 
            // update index
            high_index = i;
        }
        delete copy;
    }
    return high_index;
}

/*
 * @brief finds the best possible score by searching a decision tree of moves
 *
 * @param Board *curr_board        the current state of the board
 * @param int depth                the depth to which to search in the tree
 * @param bool maximizing_player   true if minimaxing for our team, else false
 * 
 * @return the float value of the best possible score
 *
 */
float Player::minimax(Board *curr_board, int depth, bool maximizing_player)
{
    float bestValue;
    Board *copy = curr_board->copy();

    // find opposite side 
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
        // all possible moves our player can make
        vector<Move*> children = get_possible_moves(copy, side);
        // base case, return heuristic score
        if (depth <= 0 || (children.empty() && (get_possible_moves(copy, oppo_side)).empty()))
        {
            return get_score(copy, side);
        }
        // no possible moves for us, directly call recursive
        else if (children.empty())
        {
            return minimax(copy, depth-1, false);
        }
        bestValue = -INFINITY;
        for (unsigned int i = 0; i < children.size(); i++)
        {
            // make copy and do move
            Board *copy_c = copy->copy();
            copy_c->doMove(children[i], side);
            float recur_result = minimax(copy_c, depth-1, false);
            // return maximum
            if (recur_result > bestValue)
            {
                bestValue = recur_result;
            }
            delete copy_c;
        }
        for (unsigned int i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
        delete copy;
        return bestValue;
    }
    else
    {
        // all possible moves opponent can make
        vector<Move*> children = get_possible_moves(copy, oppo_side);
        if ((depth <= 0 || children.empty()) && (get_possible_moves(copy, side)).empty())
        {
            return get_score(copy, oppo_side);
        }
        else if (children.empty())
        {
            return minimax(copy, depth-1, true);
        }
        bestValue = INFINITY;
        for (unsigned int i = 0; i < children.size(); i++)
        {
            Board *copy_c = copy->copy();
            copy_c->doMove(children[i], oppo_side);
            float recur_result = minimax(copy_c, depth-1, true);
            // return minimum
            if (recur_result < bestValue)
            {
                bestValue = recur_result;
            }
            delete copy_c;
        }
        for (unsigned int i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
        delete copy;
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
        return nullptr;
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
    if (possible_moves.size() == 0)
    {
        return nullptr;
    }

    int index = bestMove(possible_moves);

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