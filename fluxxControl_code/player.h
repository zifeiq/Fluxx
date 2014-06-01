#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "..\cardLib_code\cardLib.h"

static int cntPlayer = 0;
typedef int cardNum;

class Player {
public:
	Player(){};
	Player(std::string);
	void addHand(const Card&);
	bool consumeHand(const Card&);
	void addKeeper(const Card&);
	bool removeKeeper(const Card&);
	inline int getConsumedcard() { return cntConsumedcard; }
	inline void setConsumedcard(int i) { cntConsumedcard = i; }
private:
	std::string playerName;
	int cntConsumedcard;
	std::vector<const Card&> hand;
	std::vector<const Card&> keeper;
};
#endif