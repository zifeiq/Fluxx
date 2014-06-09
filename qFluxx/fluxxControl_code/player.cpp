#include "player.h"

extern int cntPlayer;
Player::Player(std::string name = "FluxxPlayer" + cntPlayer) : playerName(name) {
}

bool Player::consumeHand(const Card& targetCard) {
	//找到卡牌,确认有效性
	std::vector<const Card*>::const_iterator i = hand.begin();
	for(; i != hand.end(); i++) {
		if ((**i) == targetCard) {
			hand.erase(i);
		}
		return true;
	}
	return false;
}
bool Player::removeKeeper(const Card& targetCard) {
	std::vector<const Card*>::const_iterator i = keeper.begin();
	for(; i != keeper.end(); i++) {
		if ((**i) == targetCard) {
			hand.erase(i);
		}
		return true;
	}
	return false;
}
void Player::addHand(const Card& newhand) {
	hand.push_back(&newhand);
}
void Player::addKeeper(const Card& newkeeper) {
	keeper.push_back(&newkeeper);
}
bool Player::removeHand(const Card& removedhand) {
	std::vector<const Card*>::const_iterator i;
	for (i = hand.begin(); i != hand.end(); i++) {
		if ((**i) == removedhand) {
			hand.erase(i);
			return true;
		}
	}
	return false;
}