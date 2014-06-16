#include "player.h"

extern int cntPlayer;
Player::Player(std::string name) : playerName(name) {
	cntConsumedcard = 0;
	cntPlayer++;
}

bool Player::consumeHand(const Card& targetCard) {
	//找到卡牌,确认有效性
    std::vector<const Card*>::iterator i = hand.begin();
	for(; i != hand.end(); i++) {
		if ((**i) == targetCard) {
			hand.erase(i);
		}
		return true;
	}
	return false;
}
bool Player::removeKeeper(const Card& targetCard) {
    std::vector<const Card*>::iterator i = keeper.begin();
	for(; i != keeper.end(); i++) {
		if ((**i) == targetCard) {
			keeper.erase(i);
			return true;
		}
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
    std::vector<const Card*>::iterator i;
	for (i = hand.begin(); i != hand.end(); i++) {
		if ((**i) == removedhand) {
			hand.erase(i);
			return true;
		}
	}
	return false;
}
void Player::infoPlayer(std::ostream& os) {
	os << "当前玩家信息： " << std::endl;
	os << "玩家姓名：" << this->getNameref() << std::endl;
	os << "玩家手牌：" ;
	for (unsigned int i = 0; i < this->hand.size(); i++) {
		os << hand[i]->getNum() << "\t";
	}
	os << "\n所有物信息：";
	for (unsigned int i = 0; i < this->keeper.size(); i++) {
		os << keeper[i]->getNum() << "\t" ;
	}
	os << "\n";
}
