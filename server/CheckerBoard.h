

#include <vector>
#include "Piece.h"
#include <utility>
#include <stdlib.h>
#include "Utils.h"

#ifndef CheckerBoard_H_
#define CheckerBoard_H_
using namespace std;
//GamemMove class stores initial and ending position
class GameMove{
public:
	position start;
	position end;
};

//MAIN class holding the whole picure of the game
class CheckerBoard {
private:
	bool removePiece(Piece *toRemove);
	void removeKilled(position &initial, position &final, piece_color pcolor);
public:
	vector<Piece> WhitePieces, BlackPieces;
	piece_color color_up; //check if white pieces are on the upper part

	CheckerBoard();
	CheckerBoard(piece_color colorup);
	//CheckerBoard(CheckerBoard &table);
	~CheckerBoard();
	void populate(piece_color colorup);
	void checkPiecePromotion(Piece *p, position &to, piece_color pcolor);
	piece_color getPlayerUp();
	Piece* getPiece(position p, piece_color color);
	int executeMove(position &from, position &to, piece_color pcolor);
	int isMoveValidKing(position &initial, position &final, piece_color pcolor);
	int isMoveValid(position &initial, position &final, piece_color pcolor);
	Piece* getPieceAt(position p);
};

#endif /* CheckerBoard_H_ */
