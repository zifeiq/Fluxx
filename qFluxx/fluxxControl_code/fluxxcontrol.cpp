#include "..\fluxxControl_code\fluxxcontrol.h"

extern enum gameState;
extern enum Card::Type;

fluxxControl::fluxxControl():cards(CardLib::getLib()), rule(cards), presentPlayer(players[0]){
	for(int i = 0; i < cards.getCardNum(); i++)  {
		droppeddeck.push_back(cards.getCard(i));
	}//初始化牌堆
	clientNum  = -1;
	presentState = WAIT_FOR_PLAYERS;
}
int fluxxControl::_checkCntdraw() {
	int _dealcardnum = 0;
	bool _isleast = true;
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
		for(int i = (clientNum+1)%4; i != clientNum; i = (i+1)%4) {
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
	return _dealcardnum;
}
int fluxxControl::_checkCntplay() {
	int _playcardnum = 0;
	bool _ismost = true;
	for(int i = (clientNum+1)%4; i != clientNum; i = (i+1)%4) {
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
	return _playcardnum;
}
int fluxxControl::_checkCntdrop(){
	int _dropcardnum = 0;
	_dropcardnum = presentPlayer.getHandcnt() - rule.gethandlimitation();
	if (_dropcardnum < 0) {
		return -1;
	}
	else {
		return _dropcardnum;
	}
}
void fluxxControl::_shuffleCard() {
	deck = droppeddeck;
	droppeddeck.clear();
	for(int i = 0; i < deck.size(); i++) {
		const Card* tempCard = deck[i];
		int temp = rand()%deck.size();
		deck[i] = deck[temp];
		deck[temp] = tempCard;
	}
}
void fluxxControl::_settleCard(const Card& targetCard) {
	//分析卡牌信息
	Card::Type cardType = targetCard.getType();
	switch (cardType) {
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
void fluxxControl::addPlayers() {
	players.push_back(Player(msgbufString));
	msgbufMsgtype = ADD_PLAYER;
	//广播新玩家信息
	for(int i = 0; i < players.size()-1; i++) {
		msgBox.createMsg(i, msgbufMsgtype);//通知消息的类型
		msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufString);//发送真正的消息
	}
	for(int i = 0; i < players.size()-1; i++) {
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgBox.createMsg(clientNum, msgbufMsgtype, i, msgbufString);
	}
	if(clientNum == 3) {
		//限制玩家数量上限为4
		presentState = DEAL_ORIGINAL_CARD;
		//洗牌
		srand(int(time(NULL)));
		_shuffleCard();
		//分发初始手牌
		for(int i = 0; i < players.size(); i++) {
			setpresentPlayer(i);
			dealCard(_checkCntdraw());
			msgbufMsgtype = GAME_START;
			msgbufCards = presentPlayer.gethand();
			msgBox.createMsg(i,msgbufMsgtype);//通知消息类型
			msgBox.createMsg(i, msgbufMsgtype, msgbufCards);//发送真正的消息
			msgbufCards.clear();
		}
		//随机选择开始的玩家
		clientNum = rand()%4;
		setpresentPlayer(clientNum);
		presentState = ROUND_PREPARE;
		return;
	}
	clientNum++;
	return;
}
void fluxxControl::dealCard(int totalDraw) {
	//开始摸牌
	for(int i = 0; i < totalDraw; i++) {
		if (!deck.empty()) {
			presentPlayer.addHand(*deck.back());
			msgbufCards.push_back(deck.back());
			deck.pop_back();
		}
		else {
			_shuffleCard();
			i--;
		}
	}
	//广播ROUNDBEGIN啦！！
	msgbufMsgtype = ROUND_BEGIN;
	if (rule.israndomstart()) {
		msgbufAdditional = 1;
	}
	else {
		msgbufAdditional = 0;
	}
	for(int i = 0; i < players.size(); i++) {
		msgBox.createMsg(i, msgbufMsgtype);
		msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufAdditional);
	}
	//随机开场的调整
	if (rule.israndomstart()) {
		std::vector<const Card*> _temp  =presentPlayer.gethand();
		int _card = rand()%_temp.size();
		const Card* _tempcard = _temp[_card];
		msgbufCards.insert(msgbufCards.begin(),_tempcard);
	}
	//发手牌啦！！！！
	msgbufMsgtype = CARD_UPDATE;
	msgbufAdditional = 1;
	msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards, msgbufAdditional);
	presentState = PLAYING_CARD;
}
void fluxxControl::setpresentPlayer(int nextPlayer) {
	presentPlayer = players[nextPlayer];
}
void fluxxControl::playCard(const Card& targetCard) {
	//开始出牌循环
	bool _isHandLimitchanged = false;
	bool _isKeeperLimitchanged = false;
	int _winnernum;
	msgbufMsgtype = CARD_PLAYED;
		//结算，通知信息有效，广播结算结果
	if(presentPlayer.removeHand(*msgbufCards[0])){
		msgbufMsgtype = CARD_PLAYED;
		for( int i = 0; i != players.size(); i++) {
			msgBox.createMsg(i,msgbufMsgtype);//通知
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);//内容
		}
		_settleCard(*msgbufCards[0]);
		presentPlayer.setConsumedcard(presentPlayer.getConsumedcard()+1);
		//3.检查胜利情况
		_winnernum = _checkWinner();
		if (_winnernum > 0) {
			for (int i = 0; i < players.size(); i++){
				msgbufMsgtype = GAME_OVER;
				msgbufAdditional = _winnernum;
				msgBox.createMsg(i,msgbufMsgtype);
				msgBox.createMsg(i,msgbufMsgtype,msgbufAdditional);
			}//广播获胜信息结束
		}//获胜情况处理结束
	}//有效出牌处理结束
	//4.检查上限类规则
	if(_isHandLimitchanged) {
		for (int i = 0; i < players.size(); i++) {
			if(players[i].gethand().size > rule.gethandlimitation()) {
				msgbufMsgtype = DROP_CARD_C;
				msgbufAdditional = players[i].gethand().size()-rule.gethandlimitation();
				msgbufCards.clear();
				msgBox.createMsg(i, msgbufMsgtype);//通知
				msgBox.createMsg(i,msgbufMsgtype,msgbufAdditional);//内容
				msgbufMsgtype = DROP_CARD_I;
				if(msgBox.getMsg(i,msgbufMsgtype,msgbufCards)) {
					for(int j = 0; j < msgbufCards.size(); j++) {
						players[i].removeHand(*msgbufCards[j]);
						droppeddeck.push_back(msgbufCards[j]);
					}
					for(int j = 0; j < players.size(); j++) {
					//广播弃牌消息啦！！！！
						msgbufMsgtype = CARD_DROPED;
						msgBox.createMsg(j,msgbufMsgtype);
						msgBox.createMsg(j,msgbufMsgtype,msgbufCards,clientNum);
					}
				}
				msgbufCards.clear();
			}
		}
	}//手牌上限检查完毕
	if(_isKeeperLimitchanged) {
		for (int i = 0; i < players.size(); i++) {
			if (players[i].getkeeper().size() > rule.getkeeperlimitation()) {
				msgbufMsgtype = DROP_KEEPER_C;
				msgbufCards.clear();
				msgbufAdditional = players[i].getkeeper().size()-rule.getkeeperlimitation();
				msgBox.createMsg(i, msgbufMsgtype,msgbufAdditional);
			}
			if (msgBox.getMsg(i, msgbufMsgtype, msgbufCards)) {
				for (int j = 0; j < players.size(); j++) {
					players[i].removeKeeper(*msgbufCards[j]);
					droppeddeck.push_back(msgbufCards[j]);
				}
				//广播器所有物信息啦！！
				for (int j = 0; j < players.size(); j++) {
					msgbufMsgtype = KEEPER_UPDATE;
					msgbufAdditional = 0;
					msgBox.createMsg(j, msgbufMsgtype);
					msgBox.createMsg(j, msgbufMsgtype, msgbufCards, i, msgbufAdditional);
				}
			}
			msgbufCards.clear();
		}//所有物上限检查完毕
	}
}
void fluxxControl::dropCard(int totalDrop) {
	msgbufMsgtype = DROP_CARD_C;
	msgbufAdditional = totalDrop;
	msgBox.createMsg(clientNum, msgbufMsgtype, msgbufAdditional);
	msgbufMsgtype = DROP_CARD_I;
	if(msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards)) {
		for(int i = 0; i < msgbufCards.size(); i++) {
			if(presentPlayer.removeHand(*msgbufCards[i])) {
				droppeddeck.push_back(msgbufCards[i]);
			}
		}
		//广播弃牌信息
		for(int i = 0; i < players.size(); i++) {
			msgbufMsgtype = CARD_DROPED;
			msgBox.createMsg(clientNum,msgbufMsgtype);
			msgBox.createMsg(i, msgbufMsgtype, msgbufCards);
		}//广播弃牌信息结束
	}//整个弃牌处理结束
}
int fluxxControl::_checkWinner() {
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
void fluxxControl::fluxxRun() {
	bool _isRoundbegin = false;
	while(1) {
		if (presentState == WAIT_FOR_PLAYERS) {
			msgbufMsgtype = REGISTER;
			if(msgBox.getMsg(clientNum, msgbufMsgtype, msgbufString)) {
				addPlayers();
			}
		}
		else if (presentState == ROUND_PREPARE) {
			dealCard(_checkCntdraw());
		}
		else if (presentState == PLAYING_CARD) {
			while (presentPlayer.getConsumedcard() < _checkCntplay()) {
				msgbufMsgtype = PLAY_C;
				msgbufCards.clear();
				msgbufAdditional = 0;
				msgBox.createMsg(clientNum, msgbufMsgtype);
				msgbufMsgtype = PLAY_I;
				if(msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards)) {
					playCard(*msgbufCards[0]);
					presentPlayer.setConsumedcard(presentPlayer.getConsumedcard()+1);
				}
			}
			presentState = DROPING_CARD;
		}
		else if (presentState == DROPING_CARD) {
			msgbufAdditional = presentPlayer.gethand().size()-rule.gethandlimitation();
			if (msgbufAdditional > 0) {
				dropCard(msgbufAdditional);
			}
			if(rule.isorderreverse()) {
				clientNum++;
			}
			else {
				clientNum--;
			}
			setpresentPlayer(clientNum);
			presentState = ROUND_PREPARE;
		}
		else if (presentState == GAME_END) {
		}
	}
}