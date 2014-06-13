#ifndef ADDITIONAL_CARD_H
#define ADDITIONAL_CARD_H

#include "..\cardLib_code\cardLib.h"

enum Inst{
	IFCOND = 0,
	MOVE,
	SETTLE,
	END
};
enum SubOperand{
	HAND = 50,
	HAND_CNT,
	KEEPER,
	KEEPER_CNT,
	NOP
};
enum Operand {
	PRESENT_PLAYER = 60,
	LAST_PLAYER,
	NEXT_PLAYER,
	ALL_PLAYER,
	DECK,
	DROPPED_DECK,
	RULE
};
class AdditionalCard : public Card {

};



#endif