

#ifndef AI_H_
#define AI_H_

#include "Player.h"
#include "Piece.h"
#include <iostream>
#include <vector>
#include "CheckerBoard.h"

using namespace std;

//calss AIMOVE keeps the 
class AImove{
public:
	//stores the previous and ending
	GameMove m;
	
	//contains the picure of the current board
	CheckerBoard tableGame;
	//initializer
	AImove(position from, position to, AImove * parent, CheckerBoard oTable);

};

//initialize AI class with Player class inheritant
// To implement a seperate class for human and AI to ease with organization
// of classes. http://www.cplusplus.com/doc/tutorial/inheritance/
class AI : public Player {
private:
	//depth of the min max. For 4gb ram 3 is the max
	const int maxDepth = 3;

public:
	//initializer
	AI(piece_color color, CheckerBoard *table);
	// the function called by the CherckerAI to make AI return its next move
	GameMove * play();
	//find the possible moves for a given piece
	std::vector<AImove*> possibleMovesForPiece(Piece &p, AImove *parent);
	//check if the position is valid
	bool checkPositionValid(position &p);
	//heauristics function
	int  evaluate(AImove * board);
	//returning bool indicating a cutoff
	bool alphabeta(int b, int a);
	// the main AI :)
	int minmax(AImove * board, int layer, int a, int b, bool max);
};

#endif /* AI_H_ */
