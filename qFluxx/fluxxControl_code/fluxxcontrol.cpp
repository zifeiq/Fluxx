#include "..\fluxxControl_code\fluxxcontrol.h"
#include <iostream>

extern enum gameState;
extern enum Card::Type;

fluxxControl::fluxxControl(fluxxRules& r):cards(CardLib::getLib()), rule(r){
	
	for(int i = 0; i < cards.getCardNum(); i++)  {
		if (i >= 2) {
			droppeddeck.push_back(cards.getCard(i));
		}
	}//初始化牌堆
	clientNum  = 0;
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
			if(presentPlayer->gethand().empty()) {
				if (rule.isinflation()) {
					_dealcardnum = 4;
				}
				else 
					_dealcardnum = 3;
			}
		}
		for(int i = (clientNum+1)%4; i != clientNum; i = (i+1)%4) {
			if(players[i].getkeeper().size() <= presentPlayer->getkeeper().size()) {
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
		if(players[i].getkeeper().size() >= presentPlayer->getkeeper().size()) {
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
	_dropcardnum = presentPlayer->getHandcnt() - rule.gethandlimitation();
	if (_dropcardnum < 0) {
		return -1;
	}
	else {
		return _dropcardnum;
	}
}
int fluxxControl::_checkWinner() {
	//遍历每个玩家的所有物，与当前的目标牌进行匹配
	//完全匹配则返回玩家编号，没有匹配返回-1
	if (rule.firstgoal().getNum() == 0) {
		return -1;
	}
	int _subValue = rule.firstgoal().getNum();
	int _winNum;
	bool _chk1 = false;
	bool _chk2 = false;
	bool _ban = true;
	if (_subValue == 22) {//五件所有物
		for (unsigned int i = 0; i < players.size(); i++) {
			if(players[i].getKeepercnt() >= 5) {
				_chk1 = true;
				break;
			}
		}
		if (_chk1) {
			Player _tempwinner = players[0];
			_chk2 = true;
			_winNum = 0;
			for (int i = 1; 1 < players.size(); i++) {
				if(players[i].getKeepercnt() > _tempwinner.getKeepercnt()) {
					_tempwinner = players[i];
					_chk2 = true;
					_winNum = i;
				}
				else if (players[i].getKeepercnt() == _tempwinner.getKeepercnt()) {
					_chk2 = false;
				}
			}
		}
		if(_chk1 && _chk2) {
			return _winNum;
		}
		else {
			return -1;
		}		
	}//所有物最多检查完毕
	else if (_subValue == 23) {//手牌最多
		for (unsigned int i = 0; i < players.size(); i++) {
			if(players[i].getHandcnt() >= 10) {
				_chk1 = true;
				break;
			}
		}
		if (_chk1) {
			Player _tempwinner = players[0];
			_chk2 = true;
			_winNum = 0;
			for (int i = 1; 1 < players.size(); i++) {
				if(players[i].getHandcnt() > _tempwinner.getHandcnt()) {
					_tempwinner = players[i];
					_chk2 = true;
					_winNum = i;
				}
				else if (players[i].getHandcnt() == _tempwinner.getHandcnt()) {
					_chk2 = false;
				}
			}
		}
		if(_chk1 && _chk2) {
			return _winNum;
		}
		else {
			return -1;
		}		
	}//手牌最多检查完毕
	else {//匹配所有物
		std::vector<const Card*> _tempKeeper;
		const Card* _keep1 = cards.getCard(0);
		const Card* _keep2 = cards.getCard(0);
		const Card* _keeperban = cards.getCard(0);
		cards.getInfo(&rule.firstgoal(),_tempKeeper);
		_keep1 = _tempKeeper[0];
		if (rule.firstgoal().getNum() == 20 || rule.firstgoal().getNum() == 21) {
			_keeperban = _tempKeeper[1];
		}
		else {
			_keep2 = _tempKeeper[1];
		}
		for(unsigned int i = 0; i < players.size(); i++) {
			if (!players[i].getkeeper().empty()) {
				std::vector<const Card*>::const_iterator j;
				for (j = players[i].getkeeper().begin(); j != players[i].getkeeper().end(); j++) {
					if (**j == *_keep1) {
						_chk1 = true;
					}
					if (**j == *_keep2) {
						_chk2 = true;
					}
					if (**j == *_keep2) {
						_ban = false;
					}
				}
				if (_chk1 && _chk2 && _ban) {
					return i;
				}
			}
			if(!_ban) {
				break;
			}
			else {
			_chk1 = false;
			_chk2 = false;
			}
		}
	}
	if (rule.isdoublegoals()) {
		if (_subValue == 22) {//五件所有物
			for (unsigned int i = 0; i < players.size(); i++) {
				if(players[i].getKeepercnt() >= 5) {
					_chk1 = true;
					break;
				}
			}
			if (_chk1) {
				Player _tempwinner = players[0];
				_chk2 = true;
				_winNum = 0;
				for (int i = 1; 1 < players.size(); i++) {
					if(players[i].getKeepercnt() > _tempwinner.getKeepercnt()) {
						_tempwinner = players[i];
						_chk2 = true;
						_winNum = i;
					}
					else if (players[i].getKeepercnt() == _tempwinner.getKeepercnt()) {
						_chk2 = false;
					}
				}
			}
			if(_chk1 && _chk2) {
				return _winNum;
			}
			else {
				return -1;
			}		
		}//所有物最多检查完毕
		else if (_subValue == 23) {//手牌最多
			for (unsigned int i = 0; i < players.size(); i++) {
				if(players[i].getHandcnt() >= 10) {
					_chk1 = true;
					break;
				}
			}
			if (_chk1) {
				Player _tempwinner = players[0];
				_chk2 = true;
				_winNum = 0;
				for (int i = 1; 1 < players.size(); i++) {
					if(players[i].getHandcnt() > _tempwinner.getHandcnt()) {
						_tempwinner = players[i];
						_chk2 = true;
						_winNum = i;
					}
					else if (players[i].getHandcnt() == _tempwinner.getHandcnt()) {
						_chk2 = false;
					}
				}
			}
			if(_chk1 && _chk2) {
				return _winNum;
			}
			else {
				return -1;
			}		
		}//手牌最多检查完毕
		else {//匹配所有物
			std::vector<const Card*> _tempKeeper;
			const Card* _keep1 = cards.getCard(0);
			const Card* _keep2 = cards.getCard(0);
			const Card* _keeperban = cards.getCard(0);
			cards.getInfo(&rule.firstgoal(),_tempKeeper);
			_keep1 = _tempKeeper[0];
			if (rule.firstgoal().getNum() == 20 || rule.firstgoal().getNum() == 21) {
				_keeperban = _tempKeeper[1];
			}
			else {
				_keep2 = _tempKeeper[1];
			}
			for(unsigned int i = 0; i < players.size(); i++) {
				if (!players[i].getkeeper().empty()) {
					std::vector<const Card*>::const_iterator j;
					for (j = players[i].getkeeper().begin(); j != players[i].getkeeper().end(); j++) {
						if (**j == *_keep1) {
							_chk1 = true;
						}
						if (**j == *_keep2) {
							_chk2 = true;
						}
						if (**j == *_keep2) {
							_ban = false;
						}
					}
					if (_chk1 && _chk2 && _ban) {
						return i;
					}
				}
				if(!_ban) {
					break;
				}
				else {
				_chk1 = false;
				_chk2 = false;
				}
			}//单个玩家检查完毕
		}//第二目标所有物匹配检查完毕
	}//第二目标所有可能检查完毕
	return -1;
}
void fluxxControl::_shuffleCard() {
	deck = droppeddeck;
	droppeddeck.clear();
	for(unsigned int i = 0; i < deck.size(); i++) {
		const Card* tempCard = deck[i];
		int temp = rand()%deck.size();
		deck[i] = deck[temp];
		deck[temp] = tempCard;
	}
}
void fluxxControl::_updateRules() {
	msgbufCards.push_back(&rule.getdrawrule());
	msgbufCards.push_back(&rule.getplayrule());
	if (rule.gethandlimitation()) {
		msgbufCards.push_back(&rule.getChandlimit());
	}
	if (rule.getkeeperlimitation()) {
		msgbufCards.push_back(&rule.getCkeeperlimit());
	}
	if (rule.isorderreverse()) {
		msgbufCards.push_back(&rule.getCorderreverse());
	}
	if (rule.isinflation()) {
		msgbufCards.push_back(&rule.getCinflation());
	}
	if (rule.isnohandbonus()) {
		msgbufCards.push_back(&rule.getCnohandbonus());
	}
	if (rule.isrichBonus()) {
		msgbufCards.push_back(&rule.getCrichbonus());
	}
	if (rule.ispoorBonus()) {
		msgbufCards.push_back(&rule.getCpoorbonus());
	}
	if (rule.israndomstart()) {
		msgbufCards.push_back(&rule.getCrandomstart());
	}
	if (rule.isdoublegoals()) {
		msgbufCards.push_back(&rule.secondgoal());
	}
	if(!(rule.firstgoal() == *cards.getCard(0))) {
		msgbufCards.push_back(&rule.firstgoal());
	}
	if(!(rule.secondgoal() == *cards.getCard(0))) {
		msgbufCards.push_back(&rule.secondgoal());
	}
}
void fluxxControl::_settleCard(const Card& targetCard) {
	//分析卡牌信息
	Card::Type cardType = targetCard.getType();
	switch (cardType) {
	case 1:{ int _subType = targetCard.getNum()/10;//规则牌
		int _subValue = targetCard.getNum()%10;
		switch(_subType) {
		case 1:{//手牌上限相关
			if(rule.gethandlimitation() >= 0) {
				droppeddeck.push_back(&rule.getChandlimit());
			}
			rule.sethandlimitation(_subValue);
			rule.setdrawrule(targetCard);
			break;
			   }
		case 2: {
				//所有物上限相关
			if (rule.getkeeperlimitation() >= 0) {
				droppeddeck.push_back(&rule.getCkeeperlimit());
			}
			rule.setkeeperlimitation(_subValue);
			rule.setkeeperlimrule(targetCard);
			break;
				}
		case 3: {
				//摸牌相关
			if (rule.getdraw() > 1){
				droppeddeck.push_back(&rule.getdrawrule());
			}
			rule.setdraw(_subValue);
			rule.setdrawrule(targetCard);
			break;
				}
		case 4: {
				//出牌相关
			if (rule.getplay() > 1){
				droppeddeck.push_back(&rule.getplayrule());
			}
			rule.setplay(_subValue);
			rule.setplayrule(targetCard);
			break;
				}
		case 5:{ //特殊规则
			switch(_subValue) {
				case 1: { if (rule.getplay() > 1){ droppeddeck.push_back(&rule.getplayrule()); } 
						rule.setplay(100);
						rule.setplayrule(targetCard);
						break;
						}//出光手牌
				case 2: { rule.setrichbonus(true); break; } //富人奖励
				case 3: { rule.setpoorbonus(true); break; } //穷人奖励
				case 4: { rule.setinflation(true); break; } //通货膨胀
				case 5: { rule.setorderreverse(true); break; } //顺序反转
				case 6: { rule.setnohandbonus(true); break; } //无手牌奖励
				case 7: { rule.setrandomstart(true); break; } //随机开场
				case 8: { rule.setdoublegoals(true); break; } //双重目标
				}
			}//特殊规则处理完毕
		}//规则牌处理完毕
		//广播新规则啦！！！(准备数据先= =+）
		msgbufMsgtype = RULE;
		msgbufCards.clear();
		_updateRules();
		for (unsigned int i = 0; i < players.size(); i++) {
			msgBox.createMsg(i,msgbufMsgtype);//通知
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);//数据
		}
	}//规则牌结算完毕
	case 2: { presentPlayer->addKeeper(targetCard);//所有物
		msgbufMsgtype = KEEPER_UPDATE;
		msgbufAdditional = 1;
		for(unsigned int i = 0; i < players.size(); i++) {
			msgBox.createMsg(i,msgbufMsgtype);//通知
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,clientNum,msgbufAdditional);//信息
		}
		break;
		}
	case 3: {//目标卡
			if (rule.isdoublegoals()) {
				//双重目标情况下
				if (rule.firstgoal().getNum() == 0) {
					rule.setfirstgoal(targetCard);
				}
				else if (rule.secondgoal().getNum() == 0) {
					rule.setsecondgoal(targetCard);
				}
				else {
					
				}
			}
			else {
				if (rule.firstgoal().getNum() == 0) {
				}
				else {
				}
			}
			msgbufMsgtype = RULE;//与规则广播的消息相同
			msgbufCards.clear();
			_updateRules();
			for (unsigned int i = 0; i < players.size(); i++) {
				msgBox.createMsg(i,msgbufMsgtype);//通知
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards);//数据
			}
			break;
		}
		case 4: {
			//行动卡
			actioncard(targetCard);
			break;
		}
	}
}
void fluxxControl::addPlayers() {
	players.push_back(Player(msgbufString));
	msgbufMsgtype = ADD_PLAYER;
	//广播新玩家信息
	for(unsigned int i = 0; i < players.size()-1; i++) {
		msgBox.createMsg(i, msgbufMsgtype);//通知消息的类型
		msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufString);//发送真正的消息
	}
	for(unsigned int i = 0; i < players.size(); i++) {
		msgbufString = players[i].getName();
		msgBox.createMsg(clientNum, msgbufMsgtype);
		msgBox.createMsg(clientNum, msgbufMsgtype, i, msgbufString);
		Sleep(300);
	}
	if(clientNum == 3) {
		//限制玩家数量上限为4
		presentState = DEAL_ORIGINAL_CARD;
		//洗牌
		srand(int(time(NULL)));
		_shuffleCard();
		//分发初始手牌
		for(unsigned int i = 0; i < players.size(); i++) {
			setpresentPlayer(i);
			msgbufMsgtype = GAME_START;
			dealCard(_checkCntdraw());
			msgbufCards = presentPlayer->gethand();
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
			presentPlayer->addHand(*deck.back());
			msgbufCards.push_back(deck.back());
			deck.pop_back();
		}
		else {
			_shuffleCard();
			i--;
		}
	}
	//随机开场的调整
	if (rule.israndomstart()) {
		std::vector<const Card*> _temp  = presentPlayer->gethand();
		int _card = rand()%_temp.size();
		const Card* _tempcard = _temp[_card];
		msgbufCards.insert(msgbufCards.begin(),_tempcard);
	}
	//发手牌啦！！！！
	if (presentState == DEAL_ORIGINAL_CARD) {
		return;
	}
	msgbufMsgtype = CARD_UPDATE;
	msgbufAdditional = 1;
	msgBox.createMsg(clientNum, msgbufMsgtype);
	msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards, msgbufAdditional);
	//广播手牌更新啦！！！！！
	msgbufMsgtype = CARD_NUM;
	msgbufAdditional = presentPlayer->getHandcnt();
	for (int i = (clientNum+1)%4; i != clientNum; i = (i+1)%4) {
		Sleep(300);
		msgBox.createMsg(i, msgbufMsgtype);
		msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufAdditional);
	}
	presentState = PLAYING_CARD;
	return;
}
void fluxxControl::setpresentPlayer(int nextPlayer) {
	presentPlayer = &players[nextPlayer];
}
void fluxxControl::playCard(const Card& targetCard) {
	//开始出牌循环
	bool _isHandLimitchanged = false;
	bool _isKeeperLimitchanged = false;
	int _winnernum;
	msgbufMsgtype = CARD_PLAYED;
		//结算，通知信息有效，广播结算结果
	if(presentPlayer->removeHand(*msgbufCards[0])){
		msgbufMsgtype = CARD_PLAYED;
		for( unsigned int i = 0; i != players.size(); i++) {
			msgBox.createMsg(i,msgbufMsgtype);//通知
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,clientNum);//内容
		}
		_settleCard(*msgbufCards[0]);
		presentPlayer->setConsumedcard(presentPlayer->getConsumedcard()+1);
		//3.检查胜利情况
		_winnernum = _checkWinner();
		if (_winnernum > 0) {
			for (unsigned int i = 0; i < players.size(); i++){
				msgbufMsgtype = GAME_OVER;
				msgbufAdditional = _winnernum;
				msgBox.createMsg(i,msgbufMsgtype);
				msgBox.createMsg(i,msgbufMsgtype,msgbufAdditional);
			}//广播获胜信息结束
		}//获胜情况处理结束
	}//有效出牌处理结束
	//4.检查上限类规则
	if(_isHandLimitchanged) {
		for (unsigned int i = 0; i < players.size(); i++) {
			if(players[i].gethand().size() > rule.gethandlimitation()) {
				msgbufMsgtype = DROP_CARD_C;
				msgbufAdditional = players[i].gethand().size()-rule.gethandlimitation();
				msgbufCards.clear();
				msgBox.createMsg(i, msgbufMsgtype);//通知
				msgBox.createMsg(i,msgbufMsgtype,msgbufAdditional);//内容
				msgbufMsgtype = DROP_CARD_I;
				if(msgBox.getMsg(i,msgbufMsgtype,msgbufCards)) {
					for(unsigned int j = 0; j < msgbufCards.size(); j++) {
						players[i].removeHand(*msgbufCards[j]);
						droppeddeck.push_back(msgbufCards[j]);
					}
					for(unsigned int j = 0; j < players.size(); j++) {
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
		for (unsigned int i = 0; i < players.size(); i++) {
			if (players[i].getkeeper().size() > rule.getkeeperlimitation()) {
				msgbufMsgtype = DROP_KEEPER_C;
				msgbufCards.clear();
				msgbufAdditional = players[i].getkeeper().size()-rule.getkeeperlimitation();
				msgBox.createMsg(i, msgbufMsgtype,msgbufAdditional);
			}
			if (msgBox.getMsg(i, msgbufMsgtype, msgbufCards)) {
				for (unsigned int j = 0; j < players.size(); j++) {
					players[i].removeKeeper(*msgbufCards[j]);
					droppeddeck.push_back(msgbufCards[j]);
				}
				//广播器所有物信息啦！！
				for (unsigned int j = 0; j < players.size(); j++) {
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
		for(unsigned int i = 0; i < msgbufCards.size(); i++) {
			if(presentPlayer->removeHand(*msgbufCards[i])) {
				droppeddeck.push_back(msgbufCards[i]);
			}
		}
		//广播弃牌信息
		for(unsigned int i = 0; i < players.size(); i++) {
			msgbufMsgtype = CARD_DROPED;
			msgBox.createMsg(clientNum,msgbufMsgtype);
			msgBox.createMsg(i, msgbufMsgtype, msgbufCards);
		}//广播弃牌信息结束
	}//整个弃牌处理结束
}
void fluxxControl::fluxxRun() {
	while(1) {
		if (presentState == WAIT_FOR_PLAYERS) {
			msgbufMsgtype = REGISTER;
			if(msgBox.acceptNewClient()) {
				msgBox.getMsg(clientNum,msgbufMsgtype,msgbufString);
				addPlayers();
				
			}
		}
		else if (presentState == ROUND_PREPARE) {
			//广播ROUNDBEGIN啦！！
			std::cout<< "Game start!" << std::endl;
			msgbufMsgtype = ROUND_BEGIN;
			if (rule.israndomstart()) {
				msgbufAdditional = 1;
			}
			else {
				msgbufAdditional = 0;
			}
			for(unsigned int i = 0; i < players.size(); i++) {
				Sleep(300);
				msgBox.createMsg(i, msgbufMsgtype);
				msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufAdditional);
			}
			dealCard(_checkCntdraw());
		}
		else if (presentState == PLAYING_CARD) {
			while (presentPlayer->getConsumedcard() < _checkCntplay()) {
				msgbufMsgtype = PLAY_C;
				msgbufCards.clear();
				msgbufAdditional = 0;
				msgBox.createMsg(clientNum, msgbufMsgtype);
				msgbufMsgtype = PLAY_I;
				if(msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards)) {
					playCard(*msgbufCards[0]);
					presentPlayer->setConsumedcard(presentPlayer->getConsumedcard()+1);
				}
			}
			presentState = DROPING_CARD;
		}
		else if (presentState == DROPING_CARD) {
			msgbufAdditional = presentPlayer->gethand().size()-rule.gethandlimitation();
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