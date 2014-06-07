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
	int getHandcnt(){ return hand.size(); }
	void addKeeper(const Card&);
	bool removeKeeper(const Card&);
	std::vector<const Card&>& getkeeper() { return keeper; }
	int getConsumedcard() { return cntConsumedcard; }
	void setConsumedcard(int i) { cntConsumedcard = i; }
	std::string getName() { return playerName; }
private:
	std::string playerName;
	int cntConsumedcard;
	std::vector<const Card&> hand;
	std::vector<const Card&> keeper;
};
#endif