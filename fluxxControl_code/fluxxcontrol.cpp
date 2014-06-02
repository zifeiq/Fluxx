#include "..\fluxxControl_code\fluxxcontrol.h"

extern enum gameState;
extern enum msgType;

bool msgBox::parseMsg(const msgType msgtype, int& senderNum, std::string name, std::vector<const Card&>& relatedCard);
bool msgBox::creatMsg(const int msgtype, std::string name, int playerNum, std::vector<const Card&>& relatedCard, const int additionalMsg)

fluxxControl::fluxxControl():cards(CardLib::getLib()) {
	for(int i = 0; i < cards.getCardNum(); i++)  {
		droppeddeck.push_back(cards.getCard(i));
	}//初始化牌堆
	//从GUI获取主机玩家信息，创建第一个玩家
	clientNum = 1;
	presentState = WaitforPlayers;
	//等待其他玩家加入
	while (presentState == WaitforPlayers) {
		//等待玩家加入阶段，有新的玩家加入，收消息，转换消息，广播消息，告诉该玩家其他玩家信息
		//msgbufMsgtype = REGISTER;
		if(msgBox.acceptNewClient()) {
			msgBox.recvMsg(clientNum);
			if(msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)){
				players.push_back(Player(msgbufString));
			}
			//msgbufMsgtype = ACK;
			msgBox.creatMsg(msgbufMsgtype, msgbufString, msgbufString, clientNum, &msgbufCards, msgbufAdditional);
			msgBox.sendMsg(clientNum);
			//msgbufMsgtype = ADDPLAYER;
			//broadcast new player info
			for(int i = 0; i < players.size()-1, i++) {
				msgBox.creatMsg(msgbufMsgtype, msgbufString, msgbufString, i, &msgbufCards, msgbufAdditional);
				msgBox.sendMsg(i);
				//msgbufMsgtype = ACK
				msgBox.recvMsg(i);
				if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
					//wrong message
				}
				//msgbufMsgtype = ADDPLAYER;
			}
			for(int i = 0; i< players.size()-1, i++) {
				msgbufString = players[i].getName();
				msgBox.creatMsg(msgbufMsgtype, msgbufString, clientNum, &msgbufCards, msgbufAdditional);
				msgBox.sendMsg(clientNum);
				//msgbufmSGTYPE = ACK
				msgBox.recvMsg(clientNum);
				if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
					//WRONG MESSAGE
				}
				//msgbufMsgtype = ADDPLAYER;
			}
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
		//msgbufMsgtype = GAMESTART
		clientNum = distance(players.begin(), i);
		msgBox.creatMsg(msgbufMsgtype, msgbufString, clientNum, &msgbufCards, msgbufAdditional);
		msgBox.sendMsg(clientNum);
		//msgbufMsgtype = ACK
		msgBox.recvMsg(clientNum);
		if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
				//WRONG MESSAGE
		}
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
			msgbufCards.push_back(deck.back());
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
	clientNum = getclientnum(p);
}
void fluxxControl::playCard(ruleMsg msg) {
	while(presentPlayer.getConsumedcard() < msg) {
		//出牌，通知回合开始，期待出牌信息
		clientNum = getclientnum();
		dealCard(rule.draw());
		//msgbufMsgtype = ROUNDBEGIN;
		msgBox.creatMsg(msgbufMsgtype, msgbufString, clientNum, &msgbufCards, msgbufAdditional);
		msgBox.sendMsg(clientNum);
		//msgbufMsgtype = ACK;
		msgBox.recvMsg();
		if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
					//WRONG MESSAGE
		}
		//msgbufMsgtype = PLAY
		msgBox.recvMsg(clientNum);
		if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
					//WRONG MESSAGE
		}
		else {
			//RIGHT MESSAGE
			//结算，通知信息有效，广播结算结果
			if(presentPlayer.comsumeHand()){
				//手牌有效,广播出牌信息
				for( int i = (1+clientNum)%4; i != clientNum; i++) {
					//msgbufMsgtype = CARDPLAYING;
					msgBox.creatMsg(msgbufMsgtype, msgbufString, clientNum, &msgbufCards, msgbufAdditional);
					msgBox.sendMsg(i);
					//msgbufMsgtype = ACK;
					msgBox.recvMsg(i);
					if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
						//WRONG MESSAGE
					}
				}
				settleCard(msgbufCards[0]);
				presentPlayer.setConsumedcard(presentPlayer.getConsumedcard()+1);
			}
			else ｛
				//手牌无效
			}
		}
	}
}
void fluxxControl::dropCard(ruleMsg msg) {
	//弃牌，发送弃牌命令
	//如果要弃的牌数大于1，等待传递弃牌信息
	//广播弃牌信息
	//msgbufMsgtype = DROPCARD;
	msgbufAdditional = presentPlayer.getHandcnt()-msg;//可能是负数！！！！
	msgBox.creatMsg(msgbufMsgtype, msgbufString, clientNum, &msgbufCards, msgbufAdditional);
	msgBox.sendMsg(clientNum);
	//msgbufMsgtype = ACK;
	msgBox.recvMsg();
	if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
				//WRONG MESSAGE
	}
	if (msgbufAdditional > 0) {
		//msgbufMsgtype = DROPCARD
		msgBox.recvMsg();
		if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
			//WRONG MESSAGE
		}
		//广播弃牌信息
		for(int i = 0; i < msgbufCards.size(); i++) {
			if(presentPlayer.removeHand(msgbufCards[i])) {
				droppeddeck.push_back(msgbufCards[i]);
			}
			else {
				//弃牌错误
			}
		}
		//msgbufMsgtype = ACK
		msgBox.creatMsg(msgbufMsgtype, msgbufString, clientNum, &msgbufCards, msgbufAdditional);
		msgBox.sendMsg(clientNum);
		for(int i = (clientNum+1)%4; i != clientNum; i++) {
			//msgbufmsgtype = OTHERDROPCARD
			msgBox.creatMsg(msgbufMsgtype, msgbufString, clientNum, &msgbufCards, msgbufAdditional);
			msgBox.sendMsg(i);
			//msgbufMsgtype = ACK;
			msgBox.recvMsg(i);
			if(!msgBox.parseMsg(msgbufMsgtype, clientNum, &msgbufString, &msgbufCards)) {
				//WRONG MESSAGE
			}
		}
	}
}
void fluxxControl::settleCard(const Card& targetCard) {

}