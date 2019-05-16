

#ifndef PIECE_H_
#define PIECE_H_
#include <utility>
#include "Utils.h"
using namespace std;
typedef pair<int,int> position;

class Piece{
	// we wanted the checkerboard to access private members of Piece
	//however not viceversa
	//http://www.cplusplus.com/doc/tutorial/inheritance/
	friend class CheckerBoard;
private:

	int x, y;
	piece_color color;
	bool king;
public:
	Piece(int x, int y, piece_color color);

	position getPosition();

	piece_color Color();
	
	int X();

	int Y();

	void setPosition(position p);

	bool isAtPosition(position p);

	bool isKing();
};

#endif /* PIECE_H_ */
