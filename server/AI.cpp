
#include "AI.h"
#include <iostream>
#include <queue>
#include <limits.h>
#include <algorithm>
#include <limits>
#include <cstdlib>

using namespace std;
//keep track of the number of states evaluated
int evaluatedstates = 0;
//initialize AI class with Player class inheritant
// To implement a seperate class for human and AI to ease with organization
// of classes. http://www.cplusplus.com/doc/tutorial/inheritance/
AI::AI(piece_color color, CheckerBoard *table) : Player(color, table){
}

//just make sure the position is within the board
bool AI::checkPositionValid(position &p){
	if(p.first >= 0 && p.first <= 7 &&
	   p.second >= 0 && p.second <= 7)
		return true;
	return false;
}

// the function called by the CherckerAI to make AI return its next move
GameMove * AI::play(){
	//get the best move by calling other fuctions
    // initializing the GameMove class
	GameMove * bestMove = new GameMove();
    // specifying initial limits for Alpha-Beta pruning
	int a = INT_MIN;
	int b = INT_MAX;
    //get all the WhitePieces from the current board
	vector<Piece> *current_pieces = getPieces();
    //iterate thorugh all the WHITE pieces to find all the possible moves
    //and calling min max
	for (auto i : *current_pieces){
		//calls a function which will find possible move for each piece
        //and and create a class which has a picture of the board
		vector<AImove *> possible_moves = possibleMovesForPiece(i, NULL);
        //for every move call minmax and find the best move
		for (auto j : possible_moves){
			//call minmax to figure the score for this move
            //by going deeper
           
			int worth = minmax(j, maxDepth, a, b, true);
            //if the score of this piece is greater than the previous one replace it
			if (worth > a){
				a = worth;
				bestMove->start = j->m.start;
				bestMove->end   = j->m.end;
			}

		}

	}
    //print the amount of board alpha-beta evaluated
	cout << "Alpha-Beta # states evaluated = ";
	cout << evaluatedstates << endl;
	evaluatedstates = 0;
    //return the best move
	return bestMove;
}


//the following function willl evaluate alpha and beta cutoff
//in sense if the current move is worse than the previous then
// minmax should stop looking further
bool AI::alphabeta(int b, int a){
    if (b <= a)
        return true;
    return false;
    
}

//evaluation function which evaluates heurictits for a given board
//this function needs to be chenged to better AI
//however due to time contstraint we have limited the factors taken into consideration
int AI::evaluate(AImove * board){
	//create an alias so we dont have to use board->tablegame
	CheckerBoard &newtable = board->tableGame;
    //grab all the pieces for the specified color meaning the players who is playing
	vector<Piece> *pieces_to_evluate = getPieces(newtable, board->tableGame.color_up);
    //initialize score
	int score = 0;
    //if the score to be counted is for white pieces
	if (board->tableGame.color_up == WHITE){
		for(auto i : *pieces_to_evluate){
			int row = i.X();
            // pawn == row + 5 and king == row + 7
			i.isKing() ? score += row + 7 : score += row + 5;
		}
    //if the score to be counted is for white pieces
	}else{
		for (auto j : *pieces_to_evluate){
            //flip the rows so greater number of rows correlates closer to opponent
			int row = 7 - j.X();
            // pawn == row + 5 and king == row + 7
			(j.isKing() == true) ? score += row + 7 : score += row + 5;		
		}
	}
    // get the number of pieces of white peieces so killing of a piece can also be taken into
    // the consideration
    int size = 1;
    if (board->tableGame.color_up == WHITE){
        size = board->tableGame.BlackPieces.size();
    }else{
	   size = board->tableGame.WhitePieces.size();
	}
	score = score / size;

	return score;

}

// Min max to keep finding moves and scoring deeper and deeper levels into the game and
// calling alpha-beta to stop eariler to significantly incearse the effieciency
int AI::minmax (AImove * board, int layer, int a, int b, bool max){
    //if at the bottom level call the evaluation function and evaluate
	if (layer == 0){
		++evaluatedstates;
		return evaluate(board);
	}
    //if at a leafnode where the score needs to be maximized (AI)
	if (max){
		// check the color and make it equal to WhitePieces
		board->tableGame.color_up = WHITE;
		// get the pieces and find possible move for everyboard->
		// here
		CheckerBoard &newtable = board->tableGame;
        //iterating thrhough every piece -> finding possible move -> calling minmax to go to next leaf node
		vector<Piece> *current_pieces = getPieces(newtable, WHITE);
        vector<Piece> &reference_pieces = *current_pieces;
		for (auto i : reference_pieces){
			vector<AImove *> possible_moves = possibleMovesForPiece(i, board);
			for (auto j : possible_moves){
				//alterate the leaf to calulate the min value
				 a = std::max(a, minmax(j, layer - 1, a, b, false));
	            //if the current is less then the preivous then break the loop
				bool cutoff = alphabeta(b, a);
				if (cutoff)
               		return b;
			}
		}
		return a;

	}
    //if at a leafnode where the score needs to be maximized (Human)
    else{
        // check the color and make it equal to BLACK
		board->tableGame.color_up = BLACK;
        //same concept as above except this time the value of BETA will get changed as this
        //claulates the min value
		CheckerBoard &newtable = board->tableGame;
		vector<Piece> *current_pieces = getPieces(newtable, BLACK);
		vector<Piece> &reference_pieces = *current_pieces;
		for (auto i : reference_pieces){

			vector<AImove *> possible_moves = possibleMovesForPiece(i, board);
			for (auto j : possible_moves){
				b = std::min(b, minmax(j, layer - 1, a, b, true));
			bool cutoff = alphabeta(b, a);
			if (cutoff)
                return a;	            
			}
		}
		return b;
	}
}
