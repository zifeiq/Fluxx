#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <iostream>
#include "..\cardLib_code\cardLib.h"

static int cntPlayer = 0;
typedef int cardNum;

class Player {
public:
	Player(){};
	Player(std::string);
	int getHandcnt(){ return hand.size(); }
	int getcntAddcard() { return cntAddcard; }
	int getKeepercnt() { return keeper.size(); }
	int getConsumedcard() { return cntConsumedcard; }
	void addHand(const Card&);
	void addKeeper(const Card&);
	void setcntAddcard(int i) { cntAddcard = i; }
	void setConsumedcard(int i) { cntConsumedcard = i; }
	void infoPlayer();
	bool removeHand(const Card&);
	bool consumeHand(const Card&);
	bool removeKeeper(const Card&);
	std::string getName() { return playerName; }
	std::string getNameref() { return playerName; }
	std::vector<const Card*>& getkeeper() { return keeper; }
	std::vector<const Card*>& gethand() { return hand; }
protected:
	std::string playerName;
	int cntAddcard;
	int cntConsumedcard;
	std::vector<const Card*> hand;
	std::vector<const Card*> keeper;
};
#endif