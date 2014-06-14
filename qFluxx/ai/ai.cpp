#include "ai.h"
using namespace std;

AI::AI(int n):
_ownNum(0),
_playerNum(0)
{
	stringstream ss;
	ss << n;
	_ownName = "AI" + ss.str();	
}

AI::~AI()
{
	_ownName.clear();
	_ownCards.clear();
	//_ownKeepers.clear();
	_allCardNum.clear();
	_rules.clear();
	for (int i = 0; i < _allKeepers.size();i++)
		_allKeepers[i].clear();
	_allKeepers.clear();
}

//运行函数，由上层调用，收到gameover消息后退出
void AI::run()
{
	//加入并等到游戏
	if (!joinGame()) return;  
	//游戏开始后循环接收，处理或回应消息，直到游戏结束
	MsgType m;
	vector<const Card*> cards;
	int relatedPlayer, additional;
	while (1)
	{
		if (!_mailbox.getMsg(m))  {
			cout << "获得消息失败" << endl;
			while(1){}return;
		}
		else
		{
			cout << "收到第一条消息类型为" << convert(m) << endl;
			switch (m)
			{
			case RULE:
					if (!_mailbox.getMsg(m, cards)) 
						{ cout << "获得消息失败" << endl; while(1){}return; }
						cout << "收到第二条消息: " << convert(m) << "\t";
					for (int i = 0; i < cards.size(); i++)
						cout << cards[i]->getType() << cards[i]->getNum() << "\t";
					cout << endl; 
				//处理消息
				_rules = cards; 
				break;
			case PLAY_C:
				//回应消息
				play(); 
				break;

			case DROP_CARD_C:
					if (!_mailbox.getMsg(m, additional))
					{
						cout << "获得消息失败" << endl; while(1){}return;
					}
					cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl;
				//回应消息
				dropCard(additional); 
				break;
			case DROP_KEEPER_C:
				if (!_mailbox.getMsg(m, additional)) 
					{ cout << "获得消息失败" << endl; while(1){}return; }
				cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl;
				//回应消息
				dropKeeper(additional);
				break;
			case DROP_RULE_C:
				if (!_mailbox.getMsg(m, additional))
					{ cout << "获得消息失败" << endl; while(1){}return; }
				cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl;
				//回应消息
				dropRule(additional);
				break;
			case CHOOSE_KEEPER_C:
				//回应消息
				chooseKeeper(1);
				break;
			case EXCHANGE_KEEPER_C:
				//回应消息
				chooseKeeper(2);
				break;
			case CHOOSE_PLAYER_C:
				//回应消息
				choosePlayer();
				break;
			case CHOOSE_GOAL_C:
				//回应消息
				chooseGoal();
				break;
			case GAME_OVER:
				if (!_mailbox.getMsg(m, additional)) 
				{
					cout << "获得消息失败" << endl; while(1){}return;
				}
				cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl; 
				break;
			case CARD_PLAYED: 
				if (!_mailbox.getMsg(m, cards,additional)) { cout << "获得消息失败" << endl;while(1){} return; }
				cout << "收到第二条消息: " << convert(m) << "\t额外信息：" << additional<<"卡牌：" << cards[0]->getType()<<cards[0]->getNum()<< endl; 
				if (additional!= _ownNum)  //自己出的牌已在play中减去
					_allCardNum[additional]--;
				break;
			case CARD_DROPED: 
				if (!_mailbox.getMsg(m, cards, additional)){ cout << "获得消息失败" << endl; while(1){}return; }
				cout << "收到第二条消息: " << convert(m) << "\t额外信息：" << additional << "卡牌：";
				for (int i = 0; i < cards.size(); i++)
					cout << cards[i]->getType() << cards[i]->getNum() << "\t";
				cout << endl; 
				//处理消息
				for (int i = 0; i < cards.size();i++)
					_allCardNum[additional]--;
				break;
			case CARD_UPDATE:
				if (!_mailbox.getMsg(m, cards, additional)){ cout << "获得消息失败" << endl; while(1){}return; }
				cout << "收到第二条消息: " << convert(m) << "\t额外信息：" << additional << "卡牌：";
				for (int i = 0; i < cards.size(); i++)
					cout << cards[i]->getType() << cards[i]->getNum() << "\t";
				cout << endl; 
				//处理消息
				if (additional == 0)
				{
					//删除手牌
					for (int i = 0; i < cards.size(); i++)
					{
						for (int j = 0; j < _ownCards.size(); j++)
						{
							if (_ownCards[j] == cards[i])
							{
								vector<const Card*>::iterator it = _ownCards.begin() + j;
								_ownCards.erase(it);
								break;
							}
						}
					}	
				}
				else if (additional == 1)
				{	//添加手牌
					for (int i = 0; i < cards.size(); i++)
						_ownCards.push_back(cards[i]);
				}
				else if (additional == 2)
					//完全更新手牌
					_ownCards = cards;
				//更新手牌数
				_allCardNum[_ownNum] = _ownCards.size();
				break;

			case ROUND_BEGIN:
				if (!_mailbox.getMsg(m, relatedPlayer, additional)) { cout << "获得消息失败" << endl; while(1){}return; }
				cout << "收到第二条消息: " << convert(m) << "\t玩家为" << relatedPlayer << "\t额外信息为" << additional << endl; 
				//忽略消息
				break;
			case CARD_NUM:
				if (!_mailbox.getMsg(m, relatedPlayer, additional)) { cout << "获得消息失败" << endl;while(1){} return; }
				cout << "收到第二条消息: " << convert(m) << "\t玩家为" << relatedPlayer << "\t额外信息为" << additional << endl;
				//处理消息
				_allCardNum[relatedPlayer] = additional;
				break;
			case KEEPER_UPDATE:
				if (!_mailbox.getMsg(m, cards, relatedPlayer, additional)){ cout << "获得消息失败" << endl; while(1){}return; }
				cout << "收到第二条消息: " << convert(m) << "\t玩家为" << relatedPlayer << "\t额外信息为" << additional << endl;
				for (int i = 0; i < cards.size(); i++)
					cout << cards[i]->getType() << cards[i]->getNum() << "\t";
				cout << endl; 
				//处理消息
				if (additional == 0)
				{
					//删除所有物
					for (int i = 0; i < cards.size(); i++)
					{
						for (int j = 0; j < _allKeepers[relatedPlayer].size(); j++)
						{
							if (_allKeepers[relatedPlayer][j] == cards[i])
							{
								vector<const Card*>::iterator it = _allKeepers[relatedPlayer].begin() + j;
								_ownCards.erase(it);
								break;
							}
						}
					}

				}
				else if (additional == 1)
				{	//添加手牌
					for (int i = 0; i < cards.size(); i++)
						_allKeepers[relatedPlayer].push_back(cards[i]);
				}
				else if (additional == 2)
					//完全更新手牌
					_allKeepers[relatedPlayer] = cards;
				break;
			default:
				while(1){}return;
			}
		}
		if (m == GAME_OVER) break;
	}
}
//向服务器注册并加入游戏
bool AI::joinGame()
{
	if (!_mailbox.connectServer("127.0.0.1"))  //与服务器建立连接
	//if (!_mailbox.connectServer("192.168.0.112"))  //与服务器建立连接
		{
			cout<<"连接服务器失败"<<endl;
			return false;
	}
	else
		cout<<"连接服务器成功"<<endl;
	MsgType m = REGISTER;
	string name;
	int playerNum;
	//向服务器注册
	_mailbox.createMsg(REGISTER, _ownName);
	//添加玩家，等待开局
	while (m != GAME_START)
	{
		if (!_mailbox.getMsg(m)) return false;
		else
		{
			if (m == ADD_PLAYER)  //添加玩家信息
			{
				if (!_mailbox.getMsg(m, playerNum, name)) return false;
				vector<const Card*> keepers;
				_allKeepers.push_back(keepers);
				if (name == _ownName)
					_ownNum = playerNum;
				_playerNum++;
			}
			else if (m == GAME_START)  //游戏开局
			{
				vector<const Card*> temp;
				if (!_mailbox.getMsg(m, temp)) return false;
				_ownCards = temp;  //更新手牌
				cout<<"初始手牌：";
				for(int i = 0;i < 3;i++) cout<<_ownCards[i]->getType()<<_ownCards[i]->getNum()<<"\t";
				cout<<endl;
				for (int i = 0; i < _playerNum; i++)
					_allCardNum.push_back(3); //初始默认3张手牌
			}
			else
				return false;
		}
	}
	return true;
}
//出牌，回应PLAY_C消息
void AI::play()
{
	int chosen;
	vector<const Card*> cards;
	//随机选牌
	chosen = rand()%_allCardNum[_ownNum];
	//取出本牌待出
	cards.push_back(_ownCards[chosen]);
	vector<const Card*>::iterator it = _ownCards.begin() + chosen;
	_ownCards.erase(it);
	_allCardNum[_ownNum]--;
	//发消息打出本牌
	if(_mailbox.createMsg(PLAY_I, cards))
		cout << "发送消息： PLAY_I " <<cards[0]->getType()<< cards[0]->getNum()<< endl; 
}
//弃牌，回应DROP_CARD_C消息
void AI::dropCard(int n)
{
	vector<const Card*> cards;
	int chosen;
	for (int i = 0; i < n; i++)
	{
		chosen = rand() % _allCardNum[_ownNum];
		//取出本牌待弃
		cards.push_back(_ownCards[chosen]);
		vector<const Card*>::iterator it = _ownCards.begin() + chosen;
		_ownCards.erase(it);
		_allCardNum[_ownNum]--;
	}
	//发消息弃牌
	if(_mailbox.createMsg(DROP_CARD_I, cards))
	{	cout << "发送消息： DROP_CARD_I ";
	for(int i = 0; i < cards.size();i++) cout<<cards[i]->getType()<< cards[i]->getNum()<< "\t"; 
	cout<<"\n";
	}
}
//弃所有物，回应DROP_KEEPER_C消息
void AI::dropKeeper(int n)
{
	vector<const Card*> cards;
	vector<const Card*> temp =  _allKeepers[_ownNum];
	int chosen;
	for (int i = 0; i < n; i++)
	{
		chosen = rand() % temp.size();
		cards.push_back(temp[chosen]);
		vector<const Card*>::iterator it = temp.begin() + chosen;
		temp.erase(it);
	}
	//发消息弃牌
	if(_mailbox.createMsg(DROP_KEEPER_I, cards))
	{		cout << "发送消息： DROP_KEEPER_I ";
	for(int i = 0; i < cards.size();i++) cout<<cards[i]->getType()<< cards[i]->getNum()<< "\t"; 
	cout<<"\n";
	}
}
//选择在场的所有物牌，回应CHOOSE_KEEPER_C, EXCHANGE_KEEPER_C消息
void AI::chooseKeeper(int n)
{
	vector<const Card*> cards;
	int i,j;
	//选别人的所有物一张
	while (1)
	{
		i = rand() % _playerNum;
		if (i == _ownNum||_allKeepers[i].empty())  //选中自己或所选玩家无所有物
			continue;
		j = rand() % _allKeepers[i].size();
		break;
	}
	cards.push_back(_allKeepers[i][j]);
	
	if (n == 2)//还需选自己的所有物一张
	{
		i = rand() % _allKeepers[_ownNum].size();
		cards.push_back(_allKeepers[_ownNum][i]);
	}
	//发选所有物的消息
	if(_mailbox.createMsg(CHOOSE_KEEPER_I, cards))
	{		
		cout << "发送消息： CHOOSE_KEEPER_I ";
		for(int i = 0; i < cards.size();i++) cout<<cards[i]->getType()<< cards[i]->getNum()<< "\t"; 
			cout<<"\n";
	}
}
//弃规则，回应DROP_RULE_C消息
void AI::dropRule(int n)
{
	vector<const Card*> cards;
	vector<const Card*> temp = _rules;
	for (int i = 0; i < n; i++)
	{
		int j = -1;
		while(j == -1)
		{
			//选择规则牌
			j = rand() % temp.size();
			if(temp[j] ->getType() == Card::BASIC_RULE) j = -1;
		}
		cards.push_back(temp[j]);
		vector<const Card*>::iterator it = temp.begin() + j;
		temp.erase(it);
	}
	//发选规则的消息
	if(_mailbox.createMsg(DROP_RULE_I, cards))
		{		
		cout << "发送消息： DROP__RULE_I ";
		for(int i = 0; i < cards.size();i++) cout<<cards[i]->getType()<< cards[i]->getNum()<< "\t"; 
			cout<<"\n";
	}
}
//选玩家，回应CHOOSE_PLAYER_C消息
void AI::choosePlayer()
{
	int chosen;
	//选择玩家
	while (1)
	{
		chosen = rand() % _playerNum;
		if (chosen != _ownNum)
			break;
	}
	//发选玩家的消息
	if(_mailbox.createMsg(CHOOSE_PLAYER_I, chosen))
		cout << "发送消息： CHOOSE_PLAYER_I "<<chosen<<endl;
}
//选目标，回应CHOOSE_GOAL_C消息
void AI::chooseGoal()
{
	int chosen = _rules.size() - 1 - rand() % 2; //两目标必在_rules的最后两个
	vector<const Card*> cards;
	cards.push_back(_rules[chosen]);
	//发选目标的消息
	if(_mailbox.createMsg(CHOOSE_GOAL_I,cards))
			cout << "发送消息： CHOOSE_GOAL_I " <<cards[0]->getType()<< cards[0]->getNum()<< endl; 
}

advancedAI::~advancedAI()
{
	_ownName.clear();
	_ownCards.clear();
	//_ownKeepers.clear();
	_allCardNum.clear();
	_rules.clear();
	for (int i = 0; i < _allKeepers.size();i++)
		_allKeepers[i].clear();
	_allKeepers.clear();
}

//出牌，回应PLAY_C消息
//实现出牌优先级：所有物>相关目标牌>正规则牌>行动牌>无关规则牌>无关目标牌>负规则牌
void advancedAI::play()
{
	vector<const Card*> cards;
	int chosen;
	int best = 0;
	//选出当前手牌最佳牌
	for(int j = 1; j < _allCardNum[_ownNum];j++)
	{
		if(betterCard(_ownCards[j],_ownCards[best]))
			best = j;
	}
	chosen = best;
	//取出本牌待出
	cards.push_back(_ownCards[chosen]);
	vector<const Card*>::iterator it = _ownCards.begin() + chosen;
	_ownCards.erase(it);
	_allCardNum[_ownNum]--;
	//发消息出牌
	if(_mailbox.createMsg(PLAY_I, cards))
		cout << "发送消息： PLAY_I " <<cards[0]->getType()<< cards[0]->getNum()<< endl; 


	/*vector<const Card*> cards;
	int chosen = -1;
	//选牌
	while (1){
		int i = rand() % _ownCards.size();
		//检测是否有所有物手牌
		int j = i;
		do {
			if (_ownCards[j]->getType() == Card::KEEPER)
			{
				chosen = j;
				break;
			}
			else
				j = (j - 1) % _ownCards.size();
		} while (j != i);
		if (chosen != -1) break;
		//检测是否有行动牌
		j = i;
		do {
			if (_ownCards[j]->getType() == Card::ACTION)
			{
				chosen = j;
				break;
			}
			else 
				j = (j - 1) % _ownCards.size();
		} while (j != i);
		if (chosen != -1) break;
		//检测是否有目标牌
		j = i;
		do {
			if (_ownCards[j]->getType() == Card::ACTION)
			{
				chosen = j;
				break;
			}
			else
				j = (j - 1) % _ownCards.size();
		} while (j != i);
		if (chosen != -1) break;
		//均为规则牌，随机选择一张
		chosen = i;
		break;
	}
	//取出本牌待出
	cards.push_back(_ownCards[chosen]);
	vector<const Card*>::iterator it = _ownCards.begin() + chosen;
	_ownCards.erase(it);
	_allCardNum[_ownNum]--;
	//发消息打出本牌
	_mailbox.createMsg(PLAY_I, cards);
	*/
}
//弃牌，回应DROP_CARD_C消息
//出牌的逆序优先级
void advancedAI::dropCard(int n)
{
	vector<const Card*> cards;
	int chosen;
	int worst;
	for (int i = 0; i < n; i++)
	{
		worst = 0;
		//选出当前手牌最差牌
		for(int j = 1; j < _allCardNum[_ownNum];j++)
		{
			if(betterCard(_ownCards[worst],_ownCards[j]))
				worst = j;
		}
		chosen = worst;
		//取出本牌待弃
		cards.push_back(_ownCards[chosen]);
		vector<const Card*>::iterator it = _ownCards.begin() + chosen;
		_ownCards.erase(it);
		_allCardNum[_ownNum]--;
	}
	//发消息弃牌
	if(_mailbox.createMsg(DROP_CARD_I, cards))
	{	cout << "发送消息： DROP_CARD_I ";
	for(int i = 0; i < cards.size();i++) cout<<cards[i]->getType()<< cards[i]->getNum()<< "\t"; 
	cout<<"\n";
	}
}

//弃所有物，回应DROP_KEEPER_C消息
//尽量不弃相关所有物
void advancedAI::dropKeeper(int n)
{
	vector<const Card*> cards;
	vector<const Card*> temp =  _allKeepers[_ownNum];
	int chosen;
	for (int i = 0; i < n; i++)
	{
		chosen = -1;
		//优先选一无关所有物
		int m = n = rand()%temp.size();
		do{
			if(!isRelatedKeeper(temp[n]))  
			{
				chosen = n;
				break;
			}
			else
				n = (n-1)%temp.size();
		}while(n!= m);
		//无符合要求的所有物，则随机选择
		if(chosen == -1)
			chosen = rand() % temp.size();
		//将所选牌加入待弃牌堆
		cards.push_back(temp[chosen]);
		vector<const Card*>::iterator it = temp.begin() + chosen;
		temp.erase(it);
	}
	//发消息弃牌
	if(_mailbox.createMsg(DROP_KEEPER_I, cards))
	{		cout << "发送消息： DROP_KEEPER_I ";
	for(int i = 0; i < cards.size();i++) cout<<cards[i]->getType()<< cards[i]->getNum()<< "\t"; 
	cout<<"\n";
	}
}

//选择在场的所有物牌，回应CHOOSE_KEEPER_C, EXCHANGE_KEEPER_C消息
//选别人的所有物：尽量选当前目标牌对应的所有物; 选自己的所有物：尽量不选当前目标牌对应的所有物 
void advancedAI::chooseKeeper(int n)
{
	vector<const Card*> cards;
	int i = -1,j = -1;

	//检测别人是否有相关所有物
	for(int k = 0; k < _playerNum; k++)
		if(k!= _ownNum)
		{
			for(int m = 0; m < _allKeepers[k].size();m++)
			{
				if(isRelatedKeeper(_allKeepers[k][m]))
				{
					i = k; 
					j = m;
					break;
				}
			}
			if(i != -1 && j != -1) break;
		}

	if(i == -1 && j == -1)  //其他玩家手中无相关所有物，随机抽所有物
	{
		while (1)
		{
			i = rand() % _playerNum;
			if (i == _ownNum||_allKeepers[i].empty())  //选中自己或所选玩家无所有物
				continue;
			j = rand() % _allKeepers[i].size();
			break;
		}
	}
	//将所选牌加入选择所有物的待发牌堆
	cards.push_back(_allKeepers[i][j]);
	
	if (n == 2)//还需选自己的所有物一张
	{
		i = -1;
		int m = n = rand()%_allKeepers[_ownNum].size();
		do{
			if(!isRelatedKeeper(_allKeepers[_ownNum][i])) //所选牌为无关所有物
			{
				i = n;
				break;
			}
			else
				n = (n-1)%_allKeepers[_ownNum].size();
		}while(n!=m);
		//无符合要求的所有物，则随机选择
		if(i == -1)
			i = rand() % _allKeepers[_ownNum].size();
		cards.push_back(_allKeepers[_ownNum][i]);
	}
	//发选所有物的消息
	if(_mailbox.createMsg(CHOOSE_KEEPER_I, cards))
	{		
		cout << "发送消息： CHOOSE_KEEPER_I ";
		for(int i = 0; i < cards.size();i++) cout<<cards[i]->getType()<< cards[i]->getNum()<< "\t"; 
			cout<<"\n";
	}
}

//弃规则，回应DROP_RULE_C消息
//优先弃负规则牌>无关规则>正规则
void advancedAI::dropRule(int n)
{
	vector<const Card*> cards;
	vector<const Card*> temp = _rules;
	int chosen;
	for (int i = 0; i < n; i++)
	{
		chosen = -1;
		//检测是否存在负规则牌
		for(int j = 0; j <temp.size();j++)
		{
			if(isRelatedRule(temp[j]) == -1)
			{
				chosen = j;
				break;
			}
		}
	
		if(chosen == -1)//无负规则牌，则优先选一无关规则
		{
			int m = n = rand()%temp.size();
			do{
				if(isRelatedRule(temp[n]) == 0)  //无关规则牌
				{
					chosen = n;
					break;
				}
				else
					n = (n-1)%temp.size();
			}while(n!= m);
			//无符合要求的规则，则随机选择正规则
			while(chosen == -1)
			{
				//选择规则牌
				chosen = rand() % temp.size();
				if(temp[chosen] ->getType() == Card::BASIC_RULE) chosen = -1;
			}
		
		}

		//将所选规则加入待弃牌堆
		cards.push_back(temp[chosen]);
		vector<const Card*>::iterator it = temp.begin() + chosen;
		temp.erase(it);
	}
	//发选规则的消息
	if(_mailbox.createMsg(DROP_RULE_I, cards))
		{		
		cout << "发送消息： DROP__RULE_I ";
		for(int i = 0; i < cards.size();i++) cout<<cards[i]->getType()<< cards[i]->getNum()<< "\t"; 
			cout<<"\n";
	}
}

//选玩家，回应CHOOSE_PLAYER_C消息
//挑选当前手牌数量最多的玩家
void advancedAI::choosePlayer()
{
	//寻找手牌数最多的玩家
	int max = -1;
	for(int i = 0; i < _playerNum; i++)
	{
		if(i != _ownNum && _allCardNum[i]>max)
			max = i;
	}
	//发选玩家的消息
	if(_mailbox.createMsg(CHOOSE_PLAYER_I, max))
		cout << "发送消息： CHOOSE_PLAYER_I "<<max<<endl;
}

//选目标，回应CHOOSE_GOAL_C消息
//尽量选择对自己无关的目标牌
void advancedAI::chooseGoal()
{
	int chosen = _rules.size() - 1;   //两目标必在_rules的最后两个
	if(isRelatedGoal(_rules[chosen])) //此目标与自己相关，则换另一个目标
		chosen--;                     
	vector<const Card*> cards;	
	cards.push_back(_rules[chosen]);
	//发选目标的消息
	if(_mailbox.createMsg(CHOOSE_GOAL_I,cards))
			cout << "发送消息： CHOOSE_GOAL_I " <<cards[0]->getType()<< cards[0]->getNum()<< endl; 
}

//辅助函数实现
//相关所有物>不相关所有物>相关目标牌>正规则牌>行动牌>无关规则牌>无关目标牌>负规则牌>基本规则牌
bool advancedAI::betterCard(const Card* c1, const Card* c2)
{
	int flag1 = -1, flag2 = -1; 
	//7:相关所有物 6:不相关所有物 5:相关目标牌 4:正规则牌 3:行动牌 2:无关规则牌 1:无关目标牌 0:负规则牌 -1:基本规则牌
	//评价c1
	if(c1->getType() == Card::KEEPER)
	{
		if(	isRelatedKeeper(c1)) flag1 = 7;
		else flag1 = 6;
	} 
	else if(c1->getType() == Card::GOAL)
	{
		if(isRelatedGoal(c1)) flag1 = 5;
		else flag1 = 1;
	}
	else if(c1->getType() == Card::NEW_RULE)
	{
		if(isRelatedRule(c1) == 1) flag1 = 4;
		else if(isRelatedRule(c1) == -1) flag1 = 0;
		else flag1 = 2;
	}
	else if(c1->getType() == Card::ACTION)
		flag1 = 3;
	//评价c2
	if(c2->getType() == Card::KEEPER)
	{
		if(	isRelatedKeeper(c2)) flag2 = 7;
		else flag2 = 6;
	} 
	else if(c2->getType() == Card::GOAL)
	{
		if(isRelatedGoal(c2)) flag2 = 5;
		else flag2 = 1;
	}
	else if(c2->getType() == Card::NEW_RULE)
	{
		if(isRelatedRule(c2) == 1) flag2 = 4;
		else if(isRelatedRule(c2) == -1) flag2 = 0;
		else flag2 = 2;
	}
	else if(c2->getType() == Card::ACTION)
		flag2 = 3;
	//选择更好的牌返回
	if(flag1 >= flag2) return true;
	else 
		return false;
}
//检测自己的手牌或所有物中是否包含此目标对应的所有物
bool advancedAI::isRelatedGoal(const Card* c)
{
	if(c->getType() != Card::GOAL||c->getNum()>21)  //类型错误或为“5件所有物”“10张手牌”目标
		return false;
	CardLib& lib = CardLib::getLib();
	vector<const Card*> cards;
	lib.getInfo(c,cards);   //得到目标牌的对应所有物信息
	//检测手牌是否包含对应所有物
	for(int i = 0; i != _ownCards.size(); i++)
	{
		if(_ownCards[i] == cards[0] || (c->getNum() < 19 && _ownCards[i] == cards[1])) 
			return true;
	}
	//检测所有物是否包含对应所有物
	for(int i = 0; i != _allKeepers[_ownNum].size(); i++)
	{
		if(_allKeepers[_ownNum][i] == cards[0] || (c->getNum() < 19 && _allKeepers[_ownNum][i] == cards[1])) 
			return true;
	}

	return false;
}
//检测自己的手牌或当前目标中是否包含此所有物
bool advancedAI::isRelatedKeeper(const Card* c)
{
	if(c->getType() != Card::KEEPER) return false;
	CardLib& lib = CardLib::getLib();
	vector<const Card*> cards;
	//检测当前目标是否包含此所有物
	for(int j = 0;j<_rules.size();j++)
	{
		if(_rules[j]->getType() == Card::GOAL&& _rules[j]->getNum() <22) //找出需检测的目标牌
		{
			lib.getInfo(_rules[j],cards);
			if(c == cards[0] || (_rules[j]->getNum() < 19 && c == cards[1])) 
				return true;
		}
	}
	//检测手牌是否包含对应此所有物的目标
	for(int i = 0; i < _ownCards.size(); i++)
	{
		if(_ownCards[i]->getType() == Card::GOAL&& _ownCards[i]->getNum() < 22)
		{
			lib.getInfo(_ownCards[i],cards);
			if(c == cards[0] || (_ownCards[i]->getNum() < 19 && c == cards[1])) 
				return true;
		}
	}

	return false;
}
//return -1: 负规则（自己的手牌、所有物超过上限，会受到不好的影响）;0: 无关规则; 1:正规则（自己会受到好影响）;-2:其他类型 
int advancedAI::isRelatedRule(const Card* c)
{
	if(c->getType() != Card::NEW_RULE) //其他类型
		return -2;

	int num = c->getNum();
	//检测负规则
	if(num/10==1 && num%10<_allCardNum[_ownNum])//手牌数大于手牌上限
		return -1;
	if(num/10==2 && num%10<_allKeepers[_ownNum].size())//所有物数大于所有物上限
		return -1;
	//检测正规则
	if(num == 52)	//"富人奖励"
	{
		//检测自己是否为所有物最多者
		int keeperMax = _allKeepers[_ownNum].size();
		int max = _ownNum;
		for(int i = 0; i < _playerNum; i++)
		{
			if(_allKeepers[i].size()>keeperMax)
			{
				max = i;
				keeperMax = _allKeepers[i].size();
			}
		}
		if(max == _ownNum) return 1;
	}
	else if(num == 51)  //"穷人奖励"
	{
		//检测自己是否为所有物最少者
		int keeperMin = _allKeepers[_ownNum].size();
		int min = _ownNum;
		for(int i = 0; i < _playerNum; i++)
		{
			if(_allKeepers[i].size()<keeperMin)
			{
				min = i;
				keeperMin = _allKeepers[i].size();
			}
		}
		if(min == _ownNum) return 1;
	}
	else if(num = 56&&_allCardNum[_ownNum]== 0) //"无手牌奖励"
		return 1;

	//无关规则
	return 0;
}

//for debugging
string convert(MsgType m)
{
	switch (m)
	{
	case NACK: return "NACK";
	case ADD_PLAYER: return "ADD_PLAYER";
	case GAME_START: return "GAME_START";
	case ROUND_BEGIN: return "ROUND_BEGIN";
	case CARD_UPDATE: return "CARD_UPDATE";
	case PLAY_C: return "PLAY_C";
	case CARD_NUM: return "CARD_NUM";
	case CARD_PLAYED: return "CARD_PLAYED";
	case CARD_DROPED: return "CARD_DROPED";
	case RULE: return "RULE";
	case KEEPER_UPDATE: return "KEEPER_UPDATE";
	case  DROP_CARD_C: return "DROP_CARD_C";
	case  DROP_KEEPER_C: return "DROP_KEEPER_C";
	case  GAME_OVER: return "GAME_OVER";
		//case 'E': m = CARD_STOLEN: return "NACK";
	case  CHOOSE_PLAYER_C: return "CHOOSE_PLAYER_C";
	case  CHOOSE_KEEPER_C: return "CHOOSE_KEEPER_C";
	case  EXCHANGE_KEEPER_C: return "EXCHANGE_KEEPER_C";
	case  DROP_RULE_C: return "DROP_RULE_C";
	case  CHOOSE_GOAL_C: return "CHOOSE_GOAL_C";
	default: return "";
	}
}