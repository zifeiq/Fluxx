#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "..\cardLib_code\cardLib.h"

static int cntPlayer = 0;
typedef int cardNum;

class Player {
public:
	Player(std::string);
	bool useCard(const Card&);
private:
	std::string playerName;
	std::vector<const Card&> hand;
	std::vector<const Card&> keeper;
};
#endif