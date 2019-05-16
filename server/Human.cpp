
#include "Human.h"
#include "Piece.h"
#include <iostream>
using namespace std;

//human class inherited from player. There is no diffrence between the two however there is a diffrence between
//player and AI.
// To implement a seperate class for human and AI to ease with organization
// of classes. http://www.cplusplus.com/doc/tutorial/inheritance/
Human::Human(piece_color color, CheckerBoard *table) : Player(color, table){

}
