

#include "CheckerBoard.h"
#include <iostream>
using namespace std;



CheckerBoard::CheckerBoard() {
	// Auto-generated constructor stub (Eclipse IDE)


}

CheckerBoard::~CheckerBoard() {
	// Auto-generated destructor stub (Eclipse IDE)
}

CheckerBoard::CheckerBoard(piece_color colorup){
	//the code does not depend on the input however we had to diffrentiate between the
	// auto genereated constructor and avoid errors it originally gave
	//populate the table
	int row = 0;
	int column = 0;
	//alternate between odd and even
	bool odd = true; 
	while (row != 3){
		//each row has four pieces
		for (int i = 0; i < 4; i++){
			//if odd, add white piece in the odd column else in the even column
			if (odd)
				WhitePieces.push_back(Piece(column+1, row, WHITE));
			else
				WhitePieces.push_back(Piece(column, row, WHITE));
			//every other column
			column += 2;			
		}
		//increasr the row
		row++;
		column = 0;
		//toggle between even and odd
		odd ? odd = false : odd = true;
	}
	//fill the bottom half
	row = 5;
	odd = false;
	while (row != 8){
		//each row == 4 pieces
		for (int i = 0; i < 4; i++){
			//if odd, add black piece in the odd column else in the even column
			if (odd)
				BlackPieces.push_back(Piece(column+1, row, BLACK));
			else
				BlackPieces.push_back(Piece(column, row, BLACK));
			column += 2;			
		}
		//increase the row
		row++;
		//put column back to zero
		column = 0;
		//toggle
		odd ? odd = false : odd = true;
	}
	
}

//return piece at specified position
Piece* CheckerBoard::getPieceAt(position location){
	//search thorugh whitepieces first
	for(int i = 0; i < this->WhitePieces.size(); i++){
		if(WhitePieces[i].isAtPosition(location))
			return &(WhitePieces[i]);
	}
	//search thorugh balckpieces
	for(int i = 0; i < this->BlackPieces.size(); i++){
		if(BlackPieces[i].isAtPosition(location))
			return &(BlackPieces[i]);
	}
	//reutnr NULL if nothing
	return NULL;
}

// coudl've used the one above however this will avoid one for loop
// if we are sure of the color
Piece* CheckerBoard::getPiece(position pos, piece_color color){
	//if the color is black go thorugh black pieces
	if(color == BLACK){
		for(int i = 0; i < this->BlackPieces.size(); i++){
			if(BlackPieces[i].isAtPosition(pos))
				return &(BlackPieces[i]);
		}
	//if the color is black go thorugh white pieces
	}else if(color == WHITE){
		for(int i = 0; i < this->WhitePieces.size(); i++){
			if(WhitePieces[i].isAtPosition(pos))
				return &(WhitePieces[i]);
		}
	}
	return NULL;
}

// if the piece is at edge then it qualifies for a promotion to KING
void CheckerBoard::checkPiecePromotion(Piece *p, position &end, piece_color color){
	//if balck piece at the top of the board then it is king
	if(color == BLACK && end.second == 0){
		p->king = true;
	// if white piece at the bottom of the baord then it is king
	}else if(color == WHITE && end.second == 7){
		p->king = true;
	}
}

// update the table with changes caused by the move from start to end
int CheckerBoard::executeMove(position &start, position &end, piece_color color){
	// check if there is a piece to be removed or not or if the move is invalid
	int points = this->isMoveValid(start, end, color);
	// if valid move
	if(points >= 0){
		//get the piece and see if it is promoted to king
		Piece * p = this->getPiece(start,color);
		checkPiecePromotion(p, end, color);
		//if there needs someone to be killed
		if(points > 0){
			//call the function to remove the killed piece
			removeKilled(start,end,color);
			//change the positojn of the piece to the latest one
			p->setPosition(end);
			return 1;
		}else{
			//set the position of the piece to the latest one
			p->setPosition(end);
			return 0;
		}
		//invalid move
	}else{
		return -1;
	}
}

//check if the move is valid or not for king
int CheckerBoard::isMoveValidKing(position &initial, position &final, piece_color pcolor){
	//grab information about the piece
	Piece *current = getPiece(initial, pcolor);
	//create alias of the variables 
	int &x2 = final.first;
	int &y2 = final.second;
	int &x1 = initial.first;
	int &y1 = initial.second;
	
	if (pcolor == WHITE){
		//case 1 if the piece moved backwards 2 and right 2
		if (x1 + 2 == x2 && y1 - 2 == y2){
			position temp;
			temp.first = x1 + 1;
			temp.second = y1 - 1;
			Piece *check = getPiece(temp, BLACK);
			if (check ==  NULL)
				return -1;
			return 1;
		//case 2 if the piece moved backwards 2 and left 2
		}else if(x1 - 2 == x2 && y1 - 2 == y2){
			position temp;
			temp.first = x1 - 1;
			temp.second = y1 - 1;
			Piece *check = getPiece(temp, BLACK);
			if (check ==  NULL)
				return -1;
			return 1;
		}
		//if the piece only moved one block
		else if ((x1 + 1 == x2 || x1 - 1 == x2) && y1 -	 1 == y2){
					//if the end is within the board then it is a valid move
					if (x2 > -1 && x2 < 8 && y2 > -1 && y2 < 8)
						return 0;
					else
						return -1;
			}
		//if the piece is black
		}else if (pcolor == BLACK){
			//case 1: two right and two below
			if (x1 + 2 == x2 && y1 + 2 == y2){
				position temp;
				temp.first = x1 + 1;
				temp.second = y1 + 1;
				Piece *check = getPiece(temp, WHITE);
				if (check ==  NULL)
					return -1;
				return 1;
			}
			//case 2: two left and two below
			else if (x1 - 2 == x2 && y1 + 2 == y2){
				position temp;
				temp.first = x1 - 1;
				temp.second = y1 + 1;
				Piece *check = getPiece(temp, WHITE);
				if (check ==  NULL)
					return -1;
				return 1;
			}
			//if the piece only moved one block
			else if ((x1 + 1 == x2 || x1 - 1 == x2) && y1 + 1 == y2){
			//if the end is within the board then it is a valid move
				if (x2 > -1 && x2 < 8 && y2 > -1 && y2 < 8)
					return 0;
				else
					return -1;
	}
}
}

// check if move is valid
// return :
// 1 : Piece to kill
// 0 : move is valid
// -1 : move is not valid
int CheckerBoard::isMoveValid(position &initial, position &final, piece_color pcolor){
	//grab information about the piece
	Piece *current = getPiece(initial, pcolor);
	//create alias for ease
	int &x2 = final.first;
	int &y2 = final.second;
	int &x1 = initial.first;
	int &y1 = initial.second;
	//check if the end position already has a piece
	Piece *collision = getPieceAt(final);
	if (collision != NULL)
		return -1;
	//if the piece is white and not king
	else if (pcolor == WHITE && !(current->king)){
		//case 1: two below and two right
		if (x1 + 2 == x2 && y1 + 2 == y2){
			//if it jumps two then there has to be a black piece in the middle
			position temp;
			temp.first = x1 + 1;
			temp.second = y1 + 1;
			Piece *check = getPiece(temp, BLACK);
			//if the black piece doesnt exist then not valid
			if (check ==  NULL)
				return -1;
			return 1;
		//case 2: two below and two left
		}else if(x1 - 2 == x2 && y1 + 2 == y2){
			//if it jumps two then there has to be a black piece in the middle
			position temp;
			temp.first = x1 - 1;
			temp.second = y1 + 1;
			Piece *check = getPiece(temp, BLACK);
			//if the black piece doesnt exist then not valid
			if (check ==  NULL)
				return -1;
			return 1;
		}
		//if the piece only moved one block
		else if ((x1 + 1 == x2 || x1 - 1 == x2) && y1 + 1 == y2){
			//if the end is within the board then it is a valid move
			if (x2 > -1 && x2 < 8 && y2 > -1 && y2 < 8)
				return 0;
			else
				return -1;
	}
	//if the piece is black and the piece is not king
	}else if (pcolor == BLACK && !(current->king)){
		//case 1: two right and two up
		if (x1 + 2 == x2 && y1 - 2 == y2){
			//if it jumps two then there has to be a white piece in the middle
			position temp;
			temp.first = x1 + 1;
			temp.second = y1 - 1;
			Piece *check = getPiece(temp, WHITE);
			//if the white piece doesnt exist then not valid
			if (check ==  NULL)
				return -1;
			return 1;
		}
		//case 2: two left and two up
		else if (x1 - 2 == x2 && y1 - 2 == y2){
			//if it jumps two then there has to be a white piece in the middle
			position temp;
			temp.first = x1 - 1;
			temp.second = y1 - 1;
			Piece *check = getPiece(temp, WHITE);
			//if the white piece doesnt exist then not valid
			if (check ==  NULL)
				return -1;
			return 1;
		}
		else if ((x1 + 1 == x2 || x1 - 1 == x2) && y1 - 1 == y2){
			//if the end is within the board then it is a valid move
			if (x2 > -1 && x2 < 8 && y2 > -1 && y2 < 8)
				return 0;
			else
				return -1;
	}
	}else if(current->king){
		//if the piece is king then check with the king function
		int output = isMoveValidKing(initial, final, pcolor);
		return output;
	}
	return -1;

}

//find the piece to be killed and then call the kill function to kill
void CheckerBoard::removeKilled(position &initial, position &final, piece_color pcolor){
	int &x2 = final.first;
	int &x1 = initial.first;
	int &y1 = initial.second;
	int &y2 = final.second;
	int changeinx = (x2 - x1)/abs(x2 - x1);
	int changeiny = ( y2 - y1)/abs(y2 - y1);
	position toLook = final;
	toLook.first -= changeinx;
	toLook.second -= changeiny;
	Piece *toKill = this->getPieceAt(toLook);
	if(toKill != NULL && toKill->Color() != pcolor){
		removePiece(toKill);
	}
}

//remove the killed piece
bool CheckerBoard::removePiece(Piece *toRemove){
	for(int i = 0; i < BlackPieces.size(); i++){
		if(&(BlackPieces[i]) == toRemove){
			BlackPieces.erase(BlackPieces.begin() + i);
			return true;
		}
	}
	for(int i = 0; i < WhitePieces.size(); i++){
		if(&(WhitePieces[i]) == toRemove){
			WhitePieces.erase(WhitePieces.begin() + i);
			return true;
		}
	}
	return false;
}

//get the color of the player on the top
piece_color CheckerBoard::getPlayerUp(){
	return this->color_up;
}
