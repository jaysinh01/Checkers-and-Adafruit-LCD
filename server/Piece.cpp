

#include "Piece.h"

//initialize the class
Piece::Piece(int x, int y, piece_color color) {
	this->x = x;
	this->y = y;
	this->color = color;
	this->king = false;
}

//get position of the current piece
position Piece::getPosition(){
	position p;
	p.first = this->X();
	p.second = this->Y();
	return p;
}

//get color of the current peiece
piece_color Piece::Color(){
	return this->color;
}

//get column of the current peice
int Piece::X(){
	return this->x;
}

//get row of the current piece
int Piece::Y(){
	return this->y;
}

//change the position of the piece
void Piece::setPosition(position p){
	this->x = p.first;
	this->y = p.second;
}

//find if the piece is there
bool Piece::isAtPosition(position p){
	if(X() == p.first && Y() == p.second)
		return true;
	return false;
}

bool Piece::isKing(){
	return this->king;
}




