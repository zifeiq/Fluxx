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
				//检查胜利结果
				if (checkWinner()) {
					for (int i = 0; i < players.size(); i++){
						//msgbufMsgtype = GAMEOVER;
						clientNum = i;
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
	if (msg < 0) {
		return;//没有弃牌规则，直接退出
	}
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
	//分析卡牌信息
	int firstIndex = targetCard.getfirstIndex();
	int secondIndex = targetCard.getsecondIndex();
	int thirdIndex = targetCard.getthirdIndex();
	switch (thirdIndex) {
		case 1:{
			//规则牌
			switch (secondIndex) {
				case 1: {
					//手牌上限相关
					if (rule.handlimitation() >=0 ) {
						//原来已经有了,替换
						rule.sethandlimrule(targetCard);
					}
					rule.handlimitation(firstIndex);
					break;
				}
				case 2: {
					//所有物上限相关
					if (rule.keeperlimitiation() >= 0) {
						rule,setkeeperlimrule(targetCard);
					}
					rule.keeperlimitiation(firstIndex);
					break;
				}
				case 3: {
					//摸牌相关
					if (rule.draw() >= 0){
						rule.setdrawrule(targetCard);
					}
					rule.darw(firstIndex);
					break;
				}
				case 4: {
					//行动牌相关
					break;
				}
			}
			break;
		}
		case 2: {
			//所有物
			presentPlayer.addKeeper(targetCard);
			break;
		}
		case 3: {
			//目标卡
			if (rule.isdoublegoals) {
				//双重目标情况下
				if (rule.firstgoal().getCardNum() == 0) {
					rule.firstgoal(targetCard);
				}
				else if (rule.secondgoal().getCardNum() == 0) {
					rule.secondgoal(targetCard);
				}
				else {
					if (additionalMsg == 1) {
						rule.firstgoal(targetCard);
					}
					else {
						ruleMsg.secondgoal(targetCard);
					}
				}
			}
			else {
				ruleMsg.firstgoal(targetCard);
			}
			break;
		}
		case 4: {
			//行动卡
		}
	}
}
int fluxxControl::checkWinner() {
	//遍历每个玩家的所有物，与当前的目标牌进行匹配
	//完全匹配则返回玩家编号，没有匹配返回0；
	int keeperone;
	int keepertwo;
	int keeperban;
	bool chk1 = false;
	bool chk2 = false;
	bool ban = true;
	for(int i = 1; i <= players.size(); i++) {
		if (!players[i].getkeeper().empty()) {
			std::vector<const Card&>::const_interator i;
			for (i = players[i].keeper.begin(), i != keeper.end(); i++) {
				if ((*i).getCardNum() == keeperone) {
					chk1 = true;
				}
				if ((*i).getCardNum() == keepertwo) {
					chk2 = true;
				}
				if ((*i).getCardNum() == keeperban) {
					ban = false;
				}
			}
			if (chk1 && chk2 && ban) {
				return i;
			}
		}
		chk1 = false;
		chk2 = false;
	}
	if (rule.isdoublegoals()) {
		keeperone;
		keepertwo;
		keeperban;
		chk1 = false;
		chk2 = false;
		ban = true;
		for (int i = 0; i <players.size(); i++) {
			if (!players[i].getkeeper().empty()) {
				std::vector<const Card&>::const_interator i;
				for (i = players[i].keeper.begin(), i != keeper.end(); i++) {
					if ((*i).getCardNum() == keeperone) {
						chk1 = true;
					}
					if ((*i).getCardNum() == keepertwo) {
						chk2 = true;
					}
					if ((*i).getCardNum() == keeperban) {
						ban = false;
					}
				}
				if (chk1 && chk2 && ban) {
					return i;
				}
			}
		}
		chk1 = false;
		chk2 = false;
	}
	return 0;
}