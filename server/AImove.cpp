#include "AI.h"
#include "CheckerBoard.h"



 
//Create a GameMove for AI
AImove::AImove(position from, position to, AImove * parent, CheckerBoard newboard){

	m.start = from;
	m.end = to;
	this->tableGame = newboard;
}


//finds all the possible moves for a piece
vector<AImove *> AI::possibleMovesForPiece(Piece &p, AImove *parent){
	//initilize list to all the possible moves
	vector<AImove *> possible_moves;
	//initilaize table
	CheckerBoard * table;
	//go to parent level to get table
	if(parent == NULL)
		table = this->table;
	else
		table = &(parent->tableGame);

	position start;
	start.first = p.X();
	start.second = p.Y();
	
	int changeinposition = (p.Color() == table->getPlayerUp())?1:-1;
	//if piece is a king
	if(p.isKing()){
		//four cases
		for(int i = 0; i < 4; i++){
			//generate four possible directions
			// do scoring diffrently
			int changeinX, changeinY;
			if(i == 0){
				//one right one up
				changeinX = 1; changeinY = 1;
			}else if(i == 1){
				//one right one down
				changeinX = 1; changeinY = -1;
			}else if(i == 2){
				//one left one up
				changeinX = -1; changeinY = 1;
			}else if(i == 3){
				//one right one down
				changeinX = -1; changeinY = -1;
			}

			bool canMove = true;
			bool wasLastPieceEnemy = false;
			position ending_pos = p.getPosition();
			//continue moving until the piece cannot
			while(canMove){
				ending_pos.first += changeinX;
				ending_pos.second += changeinY;
				//check if it is in the board
				if(ending_pos.first < 0 || ending_pos.first > 7 ||
  				   ending_pos.second < 0 || ending_pos.second > 7){
  				   	canMove = false;
					continue;
				}
				Piece *collision = table->getPieceAt(ending_pos);
				//if the peiece at the final does't exist 
				if(collision == NULL){
					//if the piece had just skipped an enemy
					if(wasLastPieceEnemy){
						CheckerBoard newboard = *table;
						//kill and move on
						newboard.executeMove(start, ending_pos, p.Color());
						//add to the list
						AImove *new_move = new AImove(start, ending_pos, parent, newboard);
						possible_moves.push_back(new_move);
						canMove = false;
					}else{
						//if the piece did not jump an emeny,simply execute and add to the list
						CheckerBoard newboard = *table;
						newboard.executeMove(start, ending_pos, p.Color());
						AImove *new_move = new AImove(start, ending_pos, parent, newboard);
						possible_moves.push_back(new_move);
					}
				//if the collision is with a diffrent color then that is an enemy
				// capture!!!!!!!!!!
				}else if(collision->Color() != this->color &&
					 wasLastPieceEnemy != true){
					wasLastPieceEnemy = true;
				//if the collision is with the same piece then the move is not valid
				}else if(collision->Color() == this->color){
					canMove = false;
				}else{
					canMove = false;
				}
			}

		}
		//if it is not king
	}else{
		//all the cases defined
		position one_left = start;
		position one_right = start;
		one_left.second += changeinposition;
		one_right.second += changeinposition;
		one_left.first -= 1;
		one_right.first += 1;
		//if the left is empty
		if(table->getPieceAt(one_left) == NULL){
			if(this->checkPositionValid(one_left)){
				if(one_left.second == 7 || one_left.second == 0){
					CheckerBoard newboard = *table;
					newboard.executeMove(start, one_left, p.Color());
					AImove * new_move = new AImove(start, one_left, parent, newboard);
					possible_moves.push_back(new_move);
				}else{
					CheckerBoard newboard = *table;
					newboard.executeMove(start, one_left, p.Color());
					AImove *  new_move = new AImove(start, one_left, parent, newboard);
					possible_moves.push_back(new_move);
				}
			}
		//there is an enemy there
		}else if(table->getPieceAt(one_left)->Color() != this->color ){
			//check next case!
			position possible1C2 = one_left;
			possible1C2.second += changeinposition;
			possible1C2.first -= 1;
			if(table->getPieceAt(possible1C2) == NULL){
				if(this->checkPositionValid(possible1C2)){
					CheckerBoard newboard = *table;
					newboard.executeMove(start, possible1C2, p.Color());
					AImove *new_move = new AImove(start, possible1C2, parent, newboard);
					possible_moves.push_back(new_move);
				}
			}
		}

		if(table->getPieceAt(one_right) == NULL){
			if(this->checkPositionValid(one_right)){
				if(one_right.second == 7 || one_right.second == 0){
					
					CheckerBoard newboard = *table;
					newboard.executeMove(start, one_right, p.Color());
					AImove *new_move = new AImove(start, one_right,parent, newboard);
					possible_moves.push_back(new_move);
				}else{
					
					CheckerBoard newboard = *table;
					newboard.executeMove(start, one_right, p.Color());
					AImove *new_move = new AImove(start, one_right, parent, newboard);
					possible_moves.push_back(new_move);
				}
			}
		}else if(table->getPieceAt(one_right)->Color() != this->color ){
			//check next case!
			position possible2C2 = one_right;
			possible2C2.second += changeinposition;
			possible2C2.first += 1;
			if(table->getPieceAt(possible2C2) == NULL){
				if(this->checkPositionValid(possible2C2)){
					
					CheckerBoard newboard = *table;
					newboard.executeMove(start, possible2C2, p.Color());
					AImove * new_move = new AImove(start, possible2C2, parent, newboard);
					possible_moves.push_back(new_move);
				}
			}
		}
	}
	return possible_moves;
}

	