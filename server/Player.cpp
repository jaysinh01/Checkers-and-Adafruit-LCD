
#include "Player.h"

#include "CheckerBoard.h"

#include <iostream>

using namespace std;

//initialzing the plaer class
Player::Player(piece_color color, CheckerBoard *table) {
	this->table = table;
	this->color = color;
}

//return color of the current player
piece_color Player::Color(){
	return this->color;
}

//return all the pieces of the current player
vector<Piece> * Player::getPieces(){
	if(this->color == BLACK){
        
		return &(table->BlackPieces);
    }
    
	return &(table->WhitePieces);
}

//grab the peoces of desired board and player
vector<Piece> * Player::getPieces(CheckerBoard &board, piece_color color){
	if(color == BLACK){
        //cout << "you got BlackPieces" << endl;
		return &(board.BlackPieces);
    }
    //cout << "you got WhitePieces" << endl;
	return &(board.WhitePieces);
}
