
#ifndef HUMAN_H_
#define HUMAN_H_

#include "Player.h"

// To implement a seperate class for human and AI to ease with organization
// of classes. http://www.cplusplus.com/doc/tutorial/inheritance/
class Human : public Player {

public:

	Human(piece_color color, CheckerBoard *table);

};

#endif /* HUMAN_H_ */
