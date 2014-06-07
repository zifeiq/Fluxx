#include "..\fluxxControl_code\fluxxcontrol.h"

extern enum gameState;

//发送消息:客户端编号，消息类型，有关卡牌， 有关玩家， 附加信息， 有关字符串
//接收消息:客户端编号，消息类型， 有关卡牌，有关字符串
fluxxControl::fluxxControl():cards(CardLib::getLib()), rule(cards){
	for(int i = 0; i < cards.getCardNum(); i++)  {
		droppeddeck.push_back(cards.getCard(i));
	}//初始化牌堆
	//从GUI获取主机玩家信息，创建第一个玩家
	clientNum = 0;
	presentState = WAIT_FOR_PLAYERS;
	//等待其他玩家加入
	while (presentState == WAIT_FOR_PLAYERS) {
		//等待玩家加入阶段，有新的玩家加入，广播消息，告诉该玩家其他玩家信息
		msgbufMsgtype = ADDPLAYER;
		if(msgBox.acceptNewClient()) {
			if(msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards, msgbufString)){
				players.push_back(Player(msgbufString));
			}
			msgbufMsgtype = ACK;
			msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards);
			msgbufMsgtype = ADDPLAYER;
			//广播新玩家信息
			for(int i = 0; i < players.size()-1; i++) {
				msgBox.createMsg(i,msgbufMsgtype, msgbufCards, clientNum, msgbufAdditional, msgbufString);
			}
			for(int i = 0; i < players.size()-1; i++) {
				msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards, i, msgbufAdditional, msgbufString);
			}
		}
		if(clientNum == 4) {
			//限制玩家数量上限为4
			presentState = DEAL_ORIGINAL_CARD;
		}
	}
	//洗牌
	srand(int(time(NULL)));
	shuffleCard();
	//分发初始手牌
	for(int i = 0; i < players.size(); i++) {
		setpresentPlayer(players[i]);
		dealCard();
		msgbufMsgtype = GAMESTART;
		msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards);
		msgbufCards.clear();
	}
	//随机选择开始的玩家
	presentPlayer = *(players.begin()+rand()%players.size());
}
void fluxxControl::shuffleCard() {
	deck = droppeddeck;
	droppeddeck.clear();
	for(int i = 0; i < deck.size(); i++) {
		Card* tempCard = deck[i];
		int temp = rand()%deck.size();
		deck[i] = deck[temp];
		deck[temp] = tempCard;
	}
}
void fluxxControl::dealCard() {
	int _dealcardnum = 0;
	bool _isleast = true;
	//1.计算摸牌量
	if (presentState == DEAL_ORIGINAL_CARD) {
		_dealcardnum = 3;
	}
	else {
		if(rule.isnohandbonus()) {
			if(presentPlayer.gethand().empty()) {
				if (rule.isinflation()) {
					_dealcardnum = 4;
				}
				else 
					_dealcardnum = 3;
			}
		}
		for(int i = (getclientnum(presentPlayer)+1)%4; i != getclientnum(presentPlayer); i = (i+1)%4) {
			if(players[i].getkeeper().size() <= presentPlayer.getkeeper().size()) {
				_isleast = false;
				break;
			}
		}
		if (_isleast) {
			if (rule.isinflation()) {
				_dealcardnum += 2;
			}
			else {
				_dealcardnum += 1;
			}
		}
		if (rule.isinflation()) {
			_dealcardnum += (rule.getdraw()+1);
		}
		else {
			_dealcardnum += rule.getdraw();
		}
	}
	//2.开始摸牌
	for(int i = 0; i < _dealcardnum; i++) {
		if (!deck.empty()) {
			presentPlayer.addHand(*deck.back());
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
void fluxxControl::playCard() {
	//1.计算出牌数量
	int _playcardnum = 0;
	bool _ismost = true;
	for(int i = (getclientnum(presentPlayer)+1)%4; i != getclientnum(presentPlayer); i = (i+1)%4) {
		if(players[i].getkeeper().size() >= presentPlayer.getkeeper().size()) {
			_ismost = false;
			break;
		}
	}
	if (_ismost) {
		if(rule.isinflation()) {
			_playcardnum += 2;
		}
		else {
			_playcardnum += 1;
		}
	}
	_playcardnum += rule.getplay();
	//2.发送玩家手牌，通知玩家回合开始
	if (rule.israndomstart()) {
		msgbufCards = presentPlayer.gethand();
		int _card = rand()%msgbufCards.size();
		Card* _tempcard = msgbufCards[_card];
		msgbufCards.insert(msgbufCards.begin(),_tempcard);
		msgbufAdditional = 1;
		//clientNum = getclientnum(presentPlayer);
		msgbufMsgtype = DRAW;
		msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards, 0, msgbufAdditional);
	}
	else {
		msgbufCards = presentPlayer.gethand();
		msgbufAdditional = 0;
		//clientNum = getclientnum(presentPlayer);
		msgbufMsgtype = DRAW;
		msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards);
	}
	//开始出牌循环
	bool _isHandLimitchanged = false;
	bool _isKeeperLimitchanged = false;
	int _winnernum;
	while(presentPlayer.getConsumedcard() < _playcardnum && !presentPlayer.gethand().empty()) {
		_isHandLimitchanged = false;
		_isKeeperLimitchanged = false;
		//出牌，通知回合开始，期待出牌信息
		//clientNum = getclientnum(presentPlayer);
		msgbufMsgtype = ROUNDBEGIN;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards);
		msgbufMsgtype = PLAY;
		if(!msgBox.getMsg(clientNum, msgbufMsgtype,msgbufCards, msgbufString)) {
					//错误处理
		}
		else {
			//结算，通知信息有效，广播结算结果
			if(presentPlayer.removeHand(*msgbufCards[0])){
				msgbufMsgtype = ACK;
				msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards);
				//手牌有效,广播出牌信息
				msgbufMsgtype = CARDPLAYING;
				for( int i = (1+clientNum)%4; i != clientNum; i++) {
					msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
				}
				settleCard(*msgbufCards[0]);
				presentPlayer.setConsumedcard(presentPlayer.getConsumedcard()+1);
				//3.检查胜利情况
				_winnernum = checkWinner();
				if (_winnernum > 0) {
					for (int i = 0; i < players.size(); i++){
						msgbufMsgtype = GAMEOVER;
						msgbufAdditional = _winnernum;
						msgBox.createMsg(i,msgbufMsgtype,msgbufCards,0,msgbufAdditional);
					}//广播获胜信息结束
				}//获胜情况处理结束
			}//有效出牌处理结束
			else {
				//手牌无效
			}//无效出牌处理结束
		}//收到出牌消息结束
		//4.检查上限类规则
		if(_isHandLimitchanged) {
			for (int i = 0; i < players.size(); i++) {
				if(players[i].gethand().size > rule.gethandlimitation()) {
					msgbufMsgtype = DROPCARD;
					msgbufAdditional = players[i].gethand().size()-rule.gethandlimitation();
					msgBox.createMsg(i,msgbufMsgtype,msgbufCards,0,msgbufAdditional);
					msgbufMsgtype = DROPCARD;
					msgBox.getMsg(i,msgbufMsgtype,msgbufCards,msgbufString);
					for(int j = 0; j < msgbufCards.size(); j++) {
						players[i].removeHand(*msgbufCards[j]);
						droppeddeck.push_back(msgbufCards[j]);
					}
					for(int j = 0; j < players.size(); j++) {
						msgbufMsgtype = DROPCARD;
						msgBox.createMsg(j, msgbufMsgtype, msgbufCards);
					}
				}
				msgbufCards.clear();
			}
		}//手牌上限检查完毕
		if(_isKeeperLimitchanged) {
			for (int i = 0; i < players.size(); i++) {
				if (players[i].getkeeper().size() < rule.getkeeperlimitation()) {
					msgbufMsgtype = DROPKEEPER;
					msgbufAdditional = players[i].getkeeper().size()-rule.getkeeperlimitation();
					msgBox.createMsg(i, msgbufMsgtype,msgbufCards);
					msgBox.getMsg(i, msgbufMsgtype, msgbufCards, msgbufString);
				}
				for (int j = 0; j < players.size(); j++) {
					players[i].removeKeeper(*msgbufCards[j]);
					droppeddeck.push_back(msgbufCards[j]);
				}
				for (int j = 0; j < players.size(); j++) {
					msgbufMsgtype = DROPKEEPER;
					msgBox.createMsg(j, msgbufMsgtype, msgbufCards);
				}
			}
			msgbufCards.clear();
		}//所有物上限检查完毕
	}//一个玩家出完所有手牌
}
void fluxxControl::dropCard() {
	//1.计算应该弃牌数量
	int _dropcardnum = 0;
	_dropcardnum = presentPlayer.getHandcnt() - rule.gethandlimitation();
	if (_dropcardnum < 0) {
		return;
	}
	msgbufMsgtype = DROPCARD;
	msgbufAdditional = _dropcardnum;
	clientNum = getclientnum(presentPlayer);
	msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards, 0, msgbufAdditional);
	msgbufMsgtype = DROPCARD;
	if(!msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards, msgbufString)) {
				//不是弃牌信息
	}
	if (msgbufAdditional > 0) {
		msgbufMsgtype = DROPCARD;
		if(!msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufString)) {
			//WRONG MESSAGE
		}
		//处理弃牌信息
		for(int i = 0; i < msgbufCards.size(); i++) {
			if(presentPlayer.removeHand(*msgbufCards[i])) {
				droppeddeck.push_back(msgbufCards[i]);
			}
			else {
				//弃牌错误
			}
		}
		msgbufMsgtype = ACK;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards);
		//广播弃牌信息
		for(int i = (clientNum+1)%4; i != clientNum; i++) {
			//msgbufmsgtype = OTHERDROPCARD
			msgBox.createMsg(i, msgbufMsgtype, msgbufCards);
			msgbufMsgtype = ACK;
			if(!msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufString)) {
				//WRONG MESSAGE
			}
		}//广播弃牌信息结束
	}//整个弃牌处理结束
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
	//完全匹配则返回玩家编号，没有匹配返回-1
	int keeperone;
	int keepertwo;
	int keeperban;
	bool chk1 = false;
	bool chk2 = false;
	bool ban = true;
	for(int i = 0; i < players.size(); i++) {
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
	return -1;
}
void fluxxControl::stagecontrol() {

}