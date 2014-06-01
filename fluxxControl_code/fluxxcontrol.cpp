#include "..\fluxxControl_code\fluxxcontrol.h"

extern enum gameState;

fluxxControl::fluxxControl():cards(CardLib::getLib()) {
	for(int i = 0; i < cards.getCardNum(); i++)  {
		droppeddeck.push_back(cards.getCard(i));
	}//初始化牌堆
	//从GUI获取主机玩家信息，创建第一个玩家
	int clientNum = 1;
	presentState = WaitforPlayers;
	//等待其他玩家加入
	while (presentState == WaitforPlayers) {
		if(msgBox.acceptNewClient()) {
			players.push_back(Player(msgBox.recvMsg(clientNum)));
			clientNum++;
		}
		if(clientNum == 4) {
			//限制玩家数量上限为4
			presentState = PlayersTurns;
		}
	}
	srand(int(time(NULL)));
	//洗牌
	shuffleCard();
	//分发初始手牌
	for(std::vector<Player>::iterator i = players.begin(); i != players.end(); i++) {
		setpresentPlayer(*i);
		dealCard(rule.draw());
	}
	//随机选择开始的玩家
	presentPlayer = *(players.begin()+rand()%players.size());
}
void fluxxControl::shuffleCard() {
	deck = droppeddeck;
	droppeddeck.clear();
	for(int i = 0; i < deck.size(); i++) {
		const Card& tempCard = deck[i];
		int temp = rand()%deck.size();
		deck[i] = deck[temp];
		deck[temp] = tempCard;
	}
}
void fluxxControl::dealCard(ruleMsg msg) {
	for(int i = 0; i < msg; i++) {
		if (!deck.empty()) {
			presentPlayer.addHand(deck.back());
			deck.pop_back();
		}
		else {
			shuffleCard();
			i--;
		}
	}
}
void fluxxControl::setpresentPlayer(Player& p) {
	presentPlayer = p;
}
void fluxxControl::playCard(ruleMsg msg) {
	while(presentPlayer.getConsumedcard() < msg) {
		const Card& consumedCard;
		if (presentPlayer.consumeHand(consumedCard)) {
			settleCard(consumedCard);
			presentPlayer.setConsumedcard(presentPlayer.getConsumedcard()+1));
		}
	}