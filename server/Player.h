
#ifndef PLAYER_H_
#define PLAYER_H_


#include "CheckerBoard.h"


class Player {

protected:
	piece_color color;


	CheckerBoard *table;
public:

	Player(piece_color color, CheckerBoard *table);

	piece_color Color();

	vector<Piece> * getPieces();

	vector<Piece> * getPieces(CheckerBoard &t, piece_color color);

};

#endif /* PLAYER_H_ */
