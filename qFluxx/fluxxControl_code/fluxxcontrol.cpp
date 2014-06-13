#include "..\fluxxControl_code\fluxxcontrol.h"
#include <iostream>

extern enum Card::Type;

fluxxControl::fluxxControl(fluxxRules& r):cards(CardLib::getLib()), rule(r){
	std::cout << "牌堆初始化开始" << std::endl;
	for(int i = 0; i < cards.getCardNum(); i++)  {
		if (i >= 2) {
			droppeddeck.push_back(cards.getCard(i));
		}
	}//初始化牌堆
	clientNum  = 0;
	std::cout << "牌堆初始化结束,等待玩家开始" << std::endl;
	std::cout << "*********游戏准备阶段*********" << std::endl;
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
					_dealcardnum += 4;
				}
				else 
					_dealcardnum += 3;
			}
		}
		if(rule.ispoorBonus()) {
			for(int i = (clientNum+1)%4; i != clientNum; i = (i+1)%4) {
				if(players[i]->getkeeper().size() <= presentPlayer->getkeeper().size()) {
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
	if(rule.isrichBonus()) {
		bool _ismost = true;
		for(int i = (clientNum+1)%4; i != clientNum; i = (i+1)%4) {
			if(players[i]->getkeeper().size() >= presentPlayer->getkeeper().size()) {
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
	}
	if (rule.isinflation()) {
		_playcardnum = _playcardnum+1+rule.getplay();
	}
	else {
		_playcardnum += rule.getplay();
	}
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
	std::cout << "##出牌阶段：检查胜利条件##" << std::endl;
	//遍历每个玩家的所有物，与当前的目标牌进行匹配
	//完全匹配则返回玩家编号，没有匹配返回-1
	if (rule.firstgoal().getNum() == 0) {
		std::cout << "没有玩家胜利" << std::endl;
		return -1;
	}
	else {
		int _subValue = rule.firstgoal().getNum();
		int _winNum;
		bool _chk1 = false;
		bool _chk2 = false;
		bool _ban = false;
		std::vector<const Card*> _tempKeeper;
		const Card* _keep1;
		const Card* _keep2;
		const Card* _keeperban;
		if (_subValue == 22) {//五件所有物
			std::cout << "##胜利条件：五件所有物##" << std::endl;
			for (unsigned int i = 0; i < players.size(); i++) {
				if(players[i]->getKeepercnt() >= 5) {
					_chk1 = true;
					break;
				}
			}
			if (_chk1) {
				Player* _tempwinner = players[0];
				_chk2 = true;
				_winNum = 0;
				for (int i = 1; 1 < players.size(); i++) {
					if(players[i]->getKeepercnt() > _tempwinner->getKeepercnt()) {
						_tempwinner = players[i];
						_chk2 = true;
						_winNum = i;
					}
					else if (players[i]->getKeepercnt() == _tempwinner->getKeepercnt()) {
						_chk2 = false;
					}
				}
			}
			if(_chk1 && _chk2) {
				std::cout << "玩家" << _winNum << "胜利" << std::endl;
				return _winNum;
			}
			else {
				std::cout << "没有玩家胜利" << std::endl;
				if(!rule.isdoublegoals()){
					return -1;
				}
			}		
		}//所有物最多检查完毕
		else if (_subValue == 23) {//手牌最多
			std::cout << "##胜利条件：手牌最多##" << std::endl;
			for (unsigned int i = 0; i < players.size(); i++) {
				if(players[i]->getHandcnt() >= 10) {
					_chk1 = true;
					break;
				}
			}
			if (_chk1) {
				Player* _tempwinner = players[0];
				_chk2 = true;
				_winNum = 0;
				for (int i = 1; 1 < players.size(); i++) {
					if(players[i]->getHandcnt() > _tempwinner->getHandcnt()) {
						_tempwinner = players[i];
						_chk2 = true;
						_winNum = i;
					}
					else if (players[i]->getHandcnt() == _tempwinner->getHandcnt()) {
						_chk2 = false;
					}
				}
			}
			if(_chk1 && _chk2) {
				std::cout << "玩家" << _winNum << "胜利" << std::endl;
				return _winNum;
			}
			else {
				std::cout << "没有玩家胜利" << std::endl;
				if(!rule.isdoublegoals()) {
					return -1;
				}
			}		
		}//手牌最多检查完毕
		else {//匹配所有物
			cards.getInfo(&rule.firstgoal(),_tempKeeper);
			if (_tempKeeper.empty()) {
				//这是一个异常情况
				std::cout << "检查了胜利条件但是为空！" << std::endl;
				return -1;
			}//目标1异常
			else {
				_keep1 = _tempKeeper[0];
				if (rule.firstgoal().getNum() == 19) {//只需要爱
					std::cout << "##胜利条件：只需要爱" << std::endl;
					for (unsigned int i = 0; i < players.size(); i++) {
						if(players[i]->getKeepercnt() == 1 && players[i]->getkeeper()[0]->getNum() == 1) {
							std::cout << "获胜玩家为" << i << std::endl;
							return i;
						}
					}
					if(!rule.isdoublegoals()) {
						std::cout << "没有玩家胜利" << std::endl;
						return -1;
					}
				}
				else if (rule.firstgoal().getNum() == 20 || rule.firstgoal().getNum() == 21) {
					_keeperban = _tempKeeper[1];
					std::cout << "##胜利条件：只有 " << _keep1->getNum() << " 没有 " <<  _keeperban->getNum() << "##" << std::endl;
					//先检查禁止物品
					for (unsigned int i = 0; i < players.size(); ++i) {
						if(!players[i]->getkeeper().empty()) {
							for (unsigned int j = 0; j < players[i]->getkeeper().size(); ++j) {
								if((*(players[i]->getkeeper()[j])) == *_keeperban) {
									_ban = true;
									break;
								}
							}
						}//单个玩家检查完毕
						if(_ban) {
							break;//一旦检查到违禁物品就退出
						}
					}//禁止物品检查完毕
					if (!_ban) {
						for (unsigned int i = 0; i < players.size(); ++i) {
							if(!players[i]->getkeeper().empty()) {
								for (unsigned int j = 0; j < players[i]->getkeeper().size(); ++j) {
									if((*(players[i]->getkeeper()[j])) == *_keep1) {
										_chk1 = true;
										_winNum = i;
										break;
									}
								}
							}//单个玩家检查完毕
							if (_chk1) {
								std::cout << "玩家" << _winNum << "获得了胜利！" << std::endl;
								return _winNum;
							}//找到了唯一的需求，返回胜利玩家编号
						}//检查了所有玩家，没有人符合要求
						std::cout << "没有玩家胜利" << std::endl;
						if(!rule.isdoublegoals()) {
							return -1;
						}
					}//如果场上没有违禁物品
					else{
						std::cout << "没有玩家胜利" << std::endl;
						if(!rule.isdoublegoals()) {
							return -1;
						}
					}//如果场上有违禁物品
				}//一种需要一种不需要的胜利情况
				else {
					_keep2 = _tempKeeper[1];
					std::cout << "##胜利条件：有 " << _keep1->getNum() << " 还有 " <<  _keep2->getNum() << "##" << std::endl;
					for (unsigned int i = 0; i < players.size(); ++i) {
						if (!players[i]->getkeeper().empty()) {
							for (unsigned int j = 0; j < players[i]->getkeeper().size(); ++j) {
								if (*(players[i]->getkeeper()[j]) == *_keep1) {
									_chk1 = true;
								}
								if (*(players[i]->getkeeper()[j]) == *_keep2) {
									_chk2 = true;
								}
							}
						}
						if (_chk1 && _chk1) {
							std::cout << "玩家" << i << "胜利" << std::endl;
							return i;
						}
						else {
							_chk1 = false;
							_chk2 = false;
						}
					}//所有玩家都检查完毕
					if (!rule.isdoublegoals()) {
						std::cout << "没有玩家胜利" << std::endl;
						return -1;
					}
				}//需要两种胜利物品的情况
			}//目标1正常结算完毕
		}//匹配所有物检查完毕
		if (rule.isdoublegoals() && rule.secondgoal().getNum() != 0) {//第二个目标有效且存在
			std::cout << "##出牌阶段：检查胜利条件2##" << std::endl;
			_subValue = rule.secondgoal().getNum();
			if (_subValue == 22) {//五件所有物
				std::cout << "##胜利条件：五件所有物##" << std::endl;
				for (unsigned int i = 0; i < players.size(); i++) {
					if(players[i]->getKeepercnt() >= 5) {
						_chk1 = true;
						break;
					}
				}
				if (_chk1) {
					Player* _tempwinner = players[0];
					_chk2 = true;
					_winNum = 0;
					for (int i = 1; 1 < players.size(); i++) {
						if(players[i]->getKeepercnt() > _tempwinner->getKeepercnt()) {
							_tempwinner = players[i];
							_chk2 = true;
							_winNum = i;
						}
						else if (players[i]->getKeepercnt() == _tempwinner->getKeepercnt()) {
							_chk2 = false;
						}
					}
				}
				if(_chk1 && _chk2) {
					std::cout << "玩家" << _winNum << "胜利" << std::endl;
					return _winNum;
				}
				else {
					std::cout << "没有玩家胜利" << std::endl;
					return -1;
				}		
			}//所有物最多检查完毕
			else if (_subValue == 23) {//手牌最多
				std::cout << "##胜利条件：手牌最多##" << std::endl;
				for (unsigned int i = 0; i < players.size(); i++) {
					if(players[i]->getHandcnt() >= 10) {
						_chk1 = true;
						break;
					}
				}
				if (_chk1) {
					Player* _tempwinner = players[0];
					_chk2 = true;
					_winNum = 0;
					for (int i = 1; 1 < players.size(); i++) {
						if(players[i]->getHandcnt() > _tempwinner->getHandcnt()) {
							_tempwinner = players[i];
							_chk2 = true;
							_winNum = i;
						}
						else if (players[i]->getHandcnt() == _tempwinner->getHandcnt()) {
							_chk2 = false;
						}
					}
				}
				if(_chk1 && _chk2) {
					std::cout << "玩家" << _winNum << "胜利" << std::endl;
					return _winNum;
				}
				else {
					std::cout << "没有玩家胜利" << std::endl;
					return -1;
				}		
			}//手牌最多检查完毕
			else {
				_tempKeeper.clear();
				cards.getInfo(&rule.secondgoal(), _tempKeeper);
				if (_tempKeeper.empty()) {
					//这是一个异常情况
					std::cout << "检查了胜利条件但是为空！" << std::endl;
					return -1;
				}//目标2异常
				else {
					_keep1 = _tempKeeper[0];
					if (rule.firstgoal().getNum() == 19) {//只需要爱
						std::cout << "##胜利条件：只需要爱" << std::endl;
						for (unsigned int i = 0; i < players.size(); i++) {
							if(players[i]->getKeepercnt() == 1 && players[i]->getkeeper()[0]->getNum() == 1) {
								std::cout << "获胜玩家为" << i << std::endl;
								return i;
							}
						}
						return -1;
					}
					else if (rule.firstgoal().getNum() == 20 || rule.firstgoal().getNum() == 21) {
						_keeperban = _tempKeeper[1];
						std::cout << "##胜利条件：只有 " << _keep1->getNum() << " 没有 " <<  _keeperban->getNum() << "##" << std::endl;
						//先检查禁止物品
						for (unsigned int i = 0; i < players.size(); ++i) {
							if(!players[i]->getkeeper().empty()) {
								for (unsigned int j = 0; j < players[i]->getkeeper().size(); ++j) {
									if(*(players[i]->getkeeper()[j]) == *_keeperban) {
										_ban = true;
										break;
									}
								}
							}//单个玩家检查完毕
							if(_ban) {
								break;//一旦检查到违禁物品就退出
							}
						}//禁止物品检查完毕
						if (!_ban) {
							for (unsigned int i = 0; i < players.size(); ++i) {
								if(!players[i]->getkeeper().empty()) {
									for (unsigned int j = 0; j < players[i]->getkeeper().size(); ++j) {
										if(*(players[i]->getkeeper()[j]) == *_keep1) {
											_chk1 = true;
											_winNum = i;
											break;
										}
									}
								}//单个玩家检查完毕
								if (_chk1) {
									std::cout << "玩家" << _winNum << "获得了胜利！" << std::endl;
									return _winNum;
								}//找到了唯一的需求，返回胜利玩家编号
							}//检查了所有玩家，没有人符合要求
							std::cout << "没有玩家胜利" << std::endl;
							return -1;
						}//如果场上没有违禁物品
						else{
							std::cout << "没有玩家胜利" << std::endl;
							return -1;
						}//如果场上有违禁物品
					}//一种需要一种不需要的胜利情况						
					else {
						_keep2 = _tempKeeper[1];
						std::cout << "##胜利条件：有 " << _keep1->getNum() << " 还有 " <<  _keep2->getNum() << "##" << std::endl;
						for (unsigned int i = 0; i < players.size(); ++i) {
							if (!players[i]->getkeeper().empty()) {
								for (unsigned int j = 0; j < players[i]->getkeeper().size(); ++j) {
									if (*(players[i]->getkeeper()[j]) == *_keep1) {
										_chk1 = true;
									}
									if (*(players[i]->getkeeper()[j]) == *_keep2) {
										_chk2 = true;
									}
								}
							}
							if (_chk1 && _chk1) {
								std::cout << "玩家" << i << "胜利" << std::endl;
								return i;
							}
							else {
								_chk1 = false;
								_chk2 = false;
							}
						}//所有玩家都检查完毕
						std::cout << "没有玩家胜利" << std::endl;
						return -1;
					}//目标2需要两种胜利物品的情况						
				}//目标2正常
			}//目标2需要匹配所有物的胜利条件
		}
		else {
			std::cout << "没有玩家胜利" << std::endl;
			return -1;
		}
	}
}
void fluxxControl::_shuffleCard() {
	std::cout << "洗牌中……" << std::endl;
	deck = droppeddeck;
	/*int size = droppeddeck.size();
	int step = size/4;
	for (int i = 0; i < step; i++) {
		for(int j = 0; j < 4; j++) {
			deck.push_back(droppeddeck[i+step*j]);
		}
	}
	for (int i = droppeddeck.size() - deck.size(); i >0 ; i--) {
		deck.push_back(droppeddeck[droppeddeck.size()-i]);
	}*/
	droppeddeck.clear();
	for(unsigned int i = 0; i < deck.size(); i++) {
		const Card* tempCard = deck[i];
		int temp = rand()%deck.size();
		deck[i] = deck[temp];
		deck[temp] = tempCard;
	}

	std::cout << "洗牌结束" << std::endl;
}
void fluxxControl::_updateRules() {
	std::cout << "##更新规则信息##" << std::endl;
	msgbufCards.push_back(&rule.getdrawrule());
	msgbufCards.push_back(&rule.getplayrule());
	if (rule.gethandlimitation()>=0) {
		msgbufCards.push_back(&rule.getChandlimit());
	}
	if (rule.getkeeperlimitation()>=0) {
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
	if(!(rule.firstgoal() == *cards.getCard(0))) {
		msgbufCards.push_back(&rule.firstgoal());
	}
	if (rule.isdoublegoals()) {
		if(!(rule.secondgoal() == *cards.getCard(0))) {
			msgbufCards.push_back(&rule.secondgoal());
		}
	}
	std::cout << "更新后的规则为：" << std::endl;
	for (unsigned i =0; i < msgbufCards.size(); i++) {
		std::cout << msgbufCards[i]->getNum() << "\t";
	}
	std::cout << std::endl;
	//消息未发送，此处不能清空buf
}
void fluxxControl::_settleRulecard(const Card* targetCard) {
	int _subType = targetCard->getNum()/10;//规则牌
	int _subValue = targetCard->getNum()%10;
	switch(_subType) {
		case 1:{//手牌上限相关
			if(rule.gethandlimitation() >= 0) {
				droppeddeck.push_back(&rule.getChandlimit());
			}
			rule.sethandlimitation(_subValue);
			rule.sethandlimrule(targetCard);
			break;
			}
		case 2: {//所有物上限相关
			if (rule.getkeeperlimitation() >= 0) {
				droppeddeck.push_back(&rule.getCkeeperlimit());
			}
			rule.setkeeperlimitation(_subValue);
			rule.setkeeperlimrule(targetCard);
			break;
			}
		case 3: {//摸牌相关
			if (rule.getdraw() > 1){
				droppeddeck.push_back(&rule.getdrawrule());
			}
			rule.setdraw(_subValue);
			rule.setdrawrule(targetCard);
			break;
			}
		case 4: {//出牌相关
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
	//广播新规则(准备数据先）
	msgbufMsgtype = RULE;
	msgbufCards.clear();//清空之前为该回合出的卡牌
	_updateRules();
	for (unsigned int i = 0; i < players.size(); i++) {
		msgBox.createMsg(i,msgbufMsgtype);//通知
		msgBox.createMsg(i,msgbufMsgtype,msgbufCards);//数据
	}
	msgbufCards.clear();//清空之前为更新的规则信息
	return;//规则牌结算完成，返回到settlecard
}
void fluxxControl::_settleKeepercard(const Card* targetCard){
	if(rule.getkeeperlimitation()<0 || presentPlayer->getKeepercnt() < rule.getkeeperlimitation()) {
		presentPlayer->addKeeper(*targetCard);//所有物
		msgbufMsgtype = KEEPER_UPDATE;
		msgbufAdditional = 1;
		msgbufCards.push_back(targetCard);
		for(unsigned int i = 0; i < players.size(); i++) {
			Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);//通知
			Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,clientNum,msgbufAdditional);//信息
		}
		msgbufCards.clear();//清空之前用于广播的所有物
		return;//返回到settlecard
	}
	else {
		std::cout << "这个玩家的所有物即将超过上限，需要丢弃一张才能继续" << std::endl;
		msgbufMsgtype = DROP_KEEPER_C;
		msgBox.createMsg(clientNum, msgbufMsgtype);
		msgbufAdditional = 1;
		msgBox.createMsg(clientNum, msgbufMsgtype,msgbufAdditional);
		msgbufMsgtype = DROP_KEEPER_I;
		if(msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards)) {
			if(presentPlayer->removeKeeper(*msgbufCards[0])){
				std::cout << "玩家" << clientNum << "弃掉了所有物" << msgbufCards[0]->getNum() << std::endl;
				msgbufMsgtype = KEEPER_UPDATE;
				msgbufAdditional = 2;
				for (unsigned int i = 0; i < players.size(); ++i) {
					msgBox.createMsg(i, msgbufMsgtype);
					Sleep(300);
					msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufAdditional);
					Sleep(300);
				}//广播所有物更新完毕
			}//弃所有物更新完毕
			else {
				std::cout << "玩家" << clientNum << "弃所有物失败，因为这个所有物不属于他" << std::endl;
			}
		}
		msgbufCards.clear();//清空之前为弃掉的所有物消息
		return;//返回到settlecard
	}
}
void fluxxControl::_settleGoalcard(const Card* targetCard) {
	if (rule.isdoublegoals()) {
	//双重目标情况下
		if (rule.firstgoal().getNum() == 0) {
			rule.setfirstgoal(targetCard);
		}
		else if (rule.secondgoal().getNum() == 0) {
			rule.setsecondgoal(targetCard);
		}
		else {
			msgbufMsgtype = CHOOSE_GOAL_C;
			msgBox.createMsg(clientNum, msgbufMsgtype);
			msgbufMsgtype = CHOOSE_GOAL_I;
			if(msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards)) {
				if(*msgbufCards[0] == rule.firstgoal()) {
					droppeddeck.push_back(&rule.firstgoal());
					rule.setfirstgoal(msgbufCards[0]);
				}
				else if(*msgbufCards[0] == rule.secondgoal()) {
					droppeddeck.push_back(&rule.secondgoal());
					rule.setsecondgoal(msgbufCards[0]);
				}
			}
		}
	}
	else {
		if (rule.firstgoal().getNum() == 0) {
			rule.setfirstgoal(targetCard);
		}
		else {
			droppeddeck.push_back(&rule.firstgoal());
			rule.setfirstgoal(targetCard);
		}
	}
	msgbufMsgtype = RULE;//与规则广播的消息相同
	msgbufCards.clear();//清空之前可能为当前回合出牌或空
	_updateRules();//cardbuf没有清空，可以发送消息
	for (unsigned int i = 0; i < players.size(); i++) {
		Sleep(300);
		msgBox.createMsg(i,msgbufMsgtype);//通知
		Sleep(300);
		msgBox.createMsg(i,msgbufMsgtype,msgbufCards);//数据
	}
	msgbufCards.clear();//清空之前为更新的规则消息
	return;//返回到settlecard
}
void fluxxControl::_settleCard(const Card* targetCard) {
	//分析卡牌信息
	std::cout << "##出牌阶段：结算卡牌##" << std::endl;
	Card::Type cardType = targetCard->getType();
	switch (cardType) {
	case 1:{ //规则牌
		std::cout << "这是一张规则卡" << std::endl;
		_settleRulecard(targetCard);//已经广播，cardbuf已经清空
		break;
	}
	case 2: { 
		std::cout << "这是一张所有物" << std::endl;
		_settleKeepercard(targetCard);//已经广播，cardbuf已经清空
		break;
		}
	case 3: {//目标卡
		std::cout << "这是一张目标卡" << std::endl;
		_settleGoalcard(targetCard);	
		break;
	}
	case 4: {//行动卡
			std::cout << "##这是一张行动卡##" << std::endl;
			actioncard(*targetCard);
			droppeddeck.push_back(targetCard);
			break;
		}
	}
	return;//返回到playcard
}
void fluxxControl::addPlayers() {
	std::cout << "正在创建新的玩家" << std::endl;
	Player* _newplayer = new Player(msgbufString);
	this->players.push_back(_newplayer);
	std::cout << "新玩家创建完成, 新玩家信息为：" << std::endl;
	players.back()->infoPlayer();
	msgbufMsgtype = ADD_PLAYER;
	//广播新玩家信息
	for(unsigned int i = 0; i < players.size()-1; i++) {
		Sleep(300);
		msgBox.createMsg(i, msgbufMsgtype);//通知消息的类型
		Sleep(300);
		msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufString);//发送真正的消息
	}
	for(unsigned int i = 0; i < players.size(); i++) {
		msgbufString = players[i]->getName();
		msgBox.createMsg(clientNum, msgbufMsgtype);
		Sleep(300);
		msgBox.createMsg(clientNum, msgbufMsgtype, i, msgbufString);
		Sleep(300);
	}
	if(clientNum == 3) {
		//限制玩家数量上限为4
		std::cout << "所有玩家已经加入，当前玩家人数为" << players.size() <<  std::endl;
		presentState = DEAL_ORIGINAL_CARD;
		srand(int(time(NULL)));
		_shuffleCard();
		//分发初始手牌
		std::cout << "分发初始手牌" << std::endl;
		std::cout << "##当前游戏状态##" << std::endl;
		for(unsigned int i = 0; i < players.size(); i++) {
			setpresentPlayer(i);
			msgbufMsgtype = GAME_START;
			msgbufCards.clear();
			dealCard(_checkCntdraw());
			presentPlayer->infoPlayer();
			msgbufCards = presentPlayer->gethand();
			msgBox.createMsg(i,msgbufMsgtype);//通知消息类型
			Sleep(300);
			msgBox.createMsg(i, msgbufMsgtype, msgbufCards);//发送真正的消息
			Sleep(300);
		}
		//随机选择开始的玩家
		clientNum = rand()%players.size();
		setpresentPlayer(clientNum);
		presentState = ROUND_PREPARE;
		msgbufCards.clear();
		return;
	}
	clientNum++;
	msgbufString.clear();
	return;
}
void fluxxControl::dealCard(int totalDraw) {
	//开始摸牌
	std::cout << "##摸牌中……##" << std::endl;
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
	if (presentState == DEAL_ORIGINAL_CARD) {
		//GAME_START在外面发
		msgbufCards.clear();
		return;
	}
	else {
		//随机开场的调整
		if (rule.israndomstart()) {
			std::vector<const Card*> _temp  = presentPlayer->gethand();
			int _card = rand()%_temp.size();
			const Card* _tempcard = _temp[_card];
			msgbufCards.insert(msgbufCards.begin(),_tempcard);
		}
		//给当前玩家发手牌
		msgbufMsgtype = CARD_UPDATE;
		msgbufAdditional = 1;
		msgBox.createMsg(clientNum, msgbufMsgtype);
		Sleep(300);
		msgBox.createMsg(clientNum, msgbufMsgtype, msgbufCards, msgbufAdditional);
		//给其他玩家广播手牌数量
		msgbufMsgtype = CARD_NUM;
		msgbufAdditional = presentPlayer->getHandcnt();
		for (int i = (clientNum+1)%4; i != clientNum; i = (i+1)%4) {
			Sleep(300);
			msgBox.createMsg(i, msgbufMsgtype);
			Sleep(300);
			msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufAdditional);
		}
		presentState = PLAYING_CARD;
		msgbufCards.clear();
		return;
	}
}
void fluxxControl::setpresentPlayer(int nextPlayer) {
	presentPlayer = players[nextPlayer];
}
void fluxxControl::playCard(const Card* targetCard) {
	std::cout << "玩家" << presentPlayer->getName() << "打出了卡牌" << targetCard->getNum() << std::endl;
	int _winnernum;
	msgbufMsgtype = CARD_PLAYED;
		//结算，通知信息有效，广播结算结果
	if(presentPlayer->removeHand(*targetCard)){
		msgbufMsgtype = CARD_PLAYED;
		if(msgbufCards.empty()) {
			msgbufCards.push_back(targetCard);
		}
		for( unsigned int i = 0; i != players.size(); i++) {
			msgBox.createMsg(i,msgbufMsgtype);//通知
			Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,clientNum);//内容
			Sleep(300);
		}
		msgbufCards.clear();//清空之前为发送的出牌消息
		_settleCard(targetCard);//msgbuf已经清空，出牌结算结束
		//3.检查胜利情况
		_winnernum = _checkWinner();//检查结束
		if (_winnernum >= 0) {
			for (unsigned int i = 0; i < players.size(); i++){
				msgbufMsgtype = GAME_OVER;
				msgbufAdditional = _winnernum;
				msgBox.createMsg(i,msgbufMsgtype);
				Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype,msgbufAdditional);
				Sleep(300);
				presentState = GAME_END;
			}//广播获胜信息结束
			return;//返回游戏控制
		}//获胜情况处理结束
		presentPlayer->setConsumedcard(presentPlayer->getConsumedcard()+1);
	}//有效出牌处理结束,出牌张数已经增加，msgbufcard已经清空
	else {
		std::cout << "该出牌无效" << std::endl;
		msgbufCards.clear();
		return;//返回游戏控制，出牌消息并没有增加
	}//出牌无效处理结束，msgbufcard已经清空，直接返回
	//4.检查上限类规则
	if(rule.gethandlimitation() > 0) {
		for (unsigned int i = 0; i < players.size(); i++) {
			if(players[i]->gethand().size() > unsigned int(rule.gethandlimitation())) {
				msgbufMsgtype = DROP_CARD_C;
				msgbufAdditional = players[i]->gethand().size()-rule.gethandlimitation();
				msgBox.createMsg(i, msgbufMsgtype);//通知
				Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype,msgbufAdditional);//内容
				msgbufMsgtype = DROP_CARD_I;
				if(msgBox.getMsg(i,msgbufMsgtype,msgbufCards)) {
					for(unsigned int j = 0; j < msgbufCards.size(); j++) {
						players[i]->removeHand(*msgbufCards[j]);
						droppeddeck.push_back(msgbufCards[j]);
					}
					for(unsigned int j = 0; j < players.size(); j++) {//广播弃牌消息
						msgbufMsgtype = CARD_DROPED;
						msgBox.createMsg(j,msgbufMsgtype);
						Sleep(300);
						msgBox.createMsg(j,msgbufMsgtype,msgbufCards,clientNum);
						Sleep(300);
					}
				}//收到有效弃牌消息
				else {
					std::cout << "玩家" << i << "弃牌消息无效" << std::endl;
					i--;
				}//没有收到有效弃牌消息
				msgbufCards.clear();//清空之前为前一个人弃的牌
			}
		}
	}//手牌上限检查完毕，msgbuf已经清空
	if(rule.getkeeperlimitation() > 0) {
		for (unsigned int i = 0; i < players.size(); i++) {
			if (rule.getkeeperlimitation() > 0 && players[i]->getkeeper().size() > unsigned int(rule.getkeeperlimitation())) {
				msgbufMsgtype = DROP_KEEPER_C;
				msgbufAdditional = players[i]->getkeeper().size()-rule.getkeeperlimitation();
				msgBox.createMsg(i, msgbufMsgtype,msgbufAdditional);
				Sleep(300);
				msgbufMsgtype = DROP_KEEPER_I;
				if (msgBox.getMsg(i, msgbufMsgtype, msgbufCards)) {
					for (unsigned int j = 0; j < msgbufCards.size(); j++) {
						players[i]->removeKeeper(*msgbufCards[j]);
						droppeddeck.push_back(msgbufCards[j]);
					}//弃掉被选择的所有物
					msgbufCards = presentPlayer->getkeeper();//更新之前是被弃掉的所有物
					for (unsigned int j = 0; j < players.size(); j++) {//广播qi所有物信息
						msgbufMsgtype = KEEPER_UPDATE;
						msgbufAdditional = 2;
						msgBox.createMsg(j, msgbufMsgtype);
						Sleep(300);
						msgBox.createMsg(j, msgbufMsgtype, msgbufCards, i, msgbufAdditional);
						Sleep(300);	
					}
					msgbufCards.clear();//清空之前为前一个玩家弃的所有物
				}//正常弃所有物处理完毕
				else {
					std::cout << "弃所有物消息接收失败" << std::endl;
				}
			}
		}
	}//所有物上限检查完毕,msgbuf已经清空
	//5.检查新的摸牌规则
	if (_checkCntdraw() > presentPlayer->getcntAddcard()) {
		dealCard(_checkCntdraw() - presentPlayer->getcntAddcard());
	}//新的摸牌结束，msgbuf已经清空
}
void fluxxControl::dropCard(int totalDrop) {
	std::cout << "##该玩家需要弃牌" << totalDrop << "张##" << std::endl;
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
		msgbufMsgtype = CARD_DROPED;
		for(unsigned int i = 0; i < players.size(); i++) {//广播弃牌信息
			msgBox.createMsg(clientNum,msgbufMsgtype);
			msgBox.createMsg(i, msgbufMsgtype, msgbufCards);
		}//广播弃牌信息结束
		msgbufCards.clear();//清空之前为弃的牌
	}//整个弃牌处理结束
	else {
		std::cout << "弃牌信息接收失败" << std::endl;
		msgbufCards.clear();//清空之前为弃的牌（错误）
	}
	return;//返回，msgbuf已经清空
}
void fluxxControl::fluxxRun() {
	while(1) {
		if (presentState == WAIT_FOR_PLAYERS) {
			msgbufMsgtype = REGISTER;
			if(msgBox.acceptNewClient()) {
				std::cout << "有新的玩家注册！" << std::endl;
				msgBox.getMsg(clientNum,msgbufMsgtype,msgbufString);
				addPlayers();
			}
		}
		else if (presentState == ROUND_PREPARE) {
			//广播ROUNDBEGIN
			std::cout<< "**********回合开始**********" << std::endl;
			//msgbufCards.clear();
			presentPlayer->infoPlayer();
			msgbufMsgtype = ROUND_BEGIN;
			if (rule.israndomstart()) {
				std::cout << "这个回合随机开场" << std::endl;
				msgbufAdditional = 1;
			}
			else {
				msgbufAdditional = 0;
			}
			for(unsigned int i = 0; i < players.size(); i++) {
				Sleep(300);
				msgBox.createMsg(i, msgbufMsgtype);
				Sleep(300);
				msgBox.createMsg(i, msgbufMsgtype, clientNum, msgbufAdditional);
			}
			presentPlayer->setcntAddcard(_checkCntdraw());
			dealCard(presentPlayer->getcntAddcard());
		}
		else if (presentState == PLAYING_CARD) {
			std::cout << "##出牌中……##" << std::endl;
			presentPlayer->infoPlayer();
			while (presentPlayer->getHandcnt() > 0 && presentPlayer->getConsumedcard() < _checkCntplay()) {
				msgbufMsgtype = PLAY_C;
				msgbufCards.clear();
				Sleep(300);
				msgBox.createMsg(clientNum, msgbufMsgtype);
				msgbufMsgtype = PLAY_I;
				if(msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards)) {
					playCard(msgbufCards[0]);//msgbuf已经清空，出的手牌数量已经增加
				}
				else {
					std::cout << "接收出牌消息失败" << std::endl;
				}
				if (presentState == GAME_END) {
						break;
				}
			}
			if (presentState != GAME_END) {
				presentState = DROPING_CARD;
			}
		}
		else if (presentState == DROPING_CARD) {
			std::cout << "##弃牌中……##" << std::endl;
			if (rule.gethandlimitation() >= 0) {
				msgbufAdditional = presentPlayer->gethand().size()-rule.gethandlimitation();
				if (msgbufAdditional > 0) {
					dropCard(msgbufAdditional);//msgbuf已经清空
				}
			}
			std::cout << "********本回合结束********" << std::endl;
			presentPlayer->infoPlayer();
			presentPlayer->setcntAddcard(0);
			presentPlayer->setConsumedcard(0);
			if(rule.isorderreverse()) {
				clientNum = (clientNum+3)%players.size();
			}
			else {
				clientNum = (clientNum+1)%players.size();
			}
			setpresentPlayer(clientNum);

			presentState = ROUND_PREPARE;
		}
		else if (presentState == GAME_END) {
			//游戏胜利消息已经广播
			while (1) {}
		}
	}
}
fluxxControl::~fluxxControl() {
	for (unsigned int i = 0; i < players.size(); i++) {
		delete players[i];
	}
}