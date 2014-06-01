#include "player.h"

extern int cntPlayer;
Player::Player(std::string name = "FluxxPlayer" + cntPlayer) : playerName(name) {
}

bool Player::useCard(const Card& targetCard) {
	//找到卡牌,确认有效性
	std::vector<const Card&>::const_iterator i = hand.begin();
	for(; i != hand.end(); i++) {
		if ((*i) == targetCard) {
			hand.erase(i);
		}
		return true;
	}
	return false;
}