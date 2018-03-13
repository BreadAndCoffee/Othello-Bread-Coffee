
Summary of Contributions by Function:

Ishani Karmarkar: 
	- get_score()
	- get_possible_moves()

Alice Jin: 
	- minimax()
	- best_move()

Collaborative Effort: 
	- do_move()
	- best_move()
	- debugging 
	- heuristics 

Explanation of Contributions:

We went to office hours together for two nights. We also worked in our room (we are roomates) for a few nights together. We outlined our code for the functions (pseudocode) together and split up the implementation of each function. A lot of functions we also worked on together. We certainly debugged most of our functions together. 

Improvements Made Since Last Week:
	- Fixed implementation of heuristic to evaluate the entire the entire board as opposed to individual moves 
	- Tuned heuristic to weight the corners and edges more heavily 
	- Implemented a working minimax algorithm 
	
	Minimax looks ahead and our tuned heuristic performs very well, so it should be able to beat players of 
	at least ConstantPlayer skill (making it tournament-worthy)

Ideas That Were Tried But Did Not Work Out: 
	- Interior mobility 
	- Minimax over individual moves heuristic (evaluated individual leaves independently of the rest of the board)

	Interior mobility was not effective because it sometimes weighted quantity of moves over quality of moves, and after trying to tune it many times, we realized that our othellobot just did much better without the interior mobility heuristic. Minimax over individual moves did not work (or really make sense) because it
	optimized the score of a particular move without looking at the score of the whole board, which sometimes caused bad moves to be made. 