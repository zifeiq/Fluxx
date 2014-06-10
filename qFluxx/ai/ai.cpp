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
			return;
		}
		else
		{
			cout << "收到第一条消息类型为" << convert(m) << endl;
			switch (m)
			{
			case RULE:
					if (!_mailbox.getMsg(m, cards)) 
						{ cout << "获得消息失败" << endl; return; }
						cout << "收到第二条消息: " << convert(m) << "\t";
					for (int i = 0; i < cards.size(); i++)
						cout << cards[i]->getType() << cards[i]->getNum() << "\t";
					cout << endl; 
				//处理消息
				_rules = cards; 
				break;
			case PLAY_C:
					cout << "发送消息： PLAY_I" << endl; 
				//回应消息
				play(); 
				break;

			case DROP_CARD_C:
					if (!_mailbox.getMsg(m, additional))
					{
						cout << "获得消息失败" << endl; return;
					}
					cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl;
					cout << "发送消息： DROP_CARD_I" << endl; 
				//回应消息
				dropCard(additional); 
				break;
			case DROP_KEEPER_C:
				if (!_mailbox.getMsg(m, additional)) 
					{ cout << "获得消息失败" << endl; return; }
				cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl;
				cout << "发送消息： DROP_KEEPER_I" << endl; 
				//回应消息
				dropKeeper(additional);
				break;
			case DROP_RULE_C:
				if (!_mailbox.getMsg(m, additional))
					{ cout << "获得消息失败" << endl; return; }
				cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl;
				cout << "发送消息： DROP_RULE_I" << endl;
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
					cout << "获得消息失败" << endl; return;
				}
				cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl; 
				break;
			case CARD_PLAYED: 
				if (additional!= _ownNum)  //自己出的牌已在play中减去
					_allCardNum[additional]--;
				break;
			case CARD_DROPED: 
				if (!_mailbox.getMsg(m, cards, additional)){ cout << "获得消息失败" << endl; return; }
				cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl;
				for (int i = 0; i < cards.size(); i++)
					cout << cards[i]->getType() << cards[i]->getNum() << "\t";
				cout << endl; 
				//处理消息
				for (int i = 0; i < cards.size();i++)
					_allCardNum[additional]--;
				break;
			case CARD_UPDATE:
				if (!_mailbox.getMsg(m, cards, additional)){ cout << "获得消息失败" << endl; return; }
				cout << "收到第二条消息: " << convert(m) << "\t" << additional << endl;
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
				if (!_mailbox.getMsg(m, relatedPlayer, additional)) { cout << "获得消息失败" << endl; return; }
				cout << "收到第二条消息: " << convert(m) << "\t玩家为" << relatedPlayer << "\t额外信息为" << additional << endl; 
				//忽略消息
				break;
			case CARD_NUM:
				if (!_mailbox.getMsg(m, relatedPlayer, additional)) { cout << "获得消息失败" << endl; return; }
				cout << "收到第二条消息: " << convert(m) << "\t玩家为" << relatedPlayer << "\t额外信息为" << additional << endl;
				//处理消息
				_allCardNum[relatedPlayer] = additional;
				break;
			case KEEPER_UPDATE:
				if (!_mailbox.getMsg(m, cards, relatedPlayer, additional)){ cout << "获得消息失败" << endl; return; }
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
				return;
			}
		}
		if (m == GAME_OVER) break;
	}
}
//向服务器注册并加入游戏
bool AI::joinGame()
{
	if (!_mailbox.connectServer("127.0.0.1"))  //与服务器建立连接
		return false;
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
				for (int i = 0; i < _playerNum; i++)
					_allCardNum.push_back(3); //初始默认3张手牌
			}
			else
				return false;
		}
	}
}
//出牌，回应PLAY_C消息
//出牌优先级：所有物>行动牌>目标牌>规则牌
void AI::play()
{
	vector<const Card*> cards;
	int chosen = -1;
	
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
	_mailbox.createMsg(DROP_CARD_I, cards);
}

//弃所有物，回应DROP_KEEPER_C消息
//尽量不弃当前目标牌对应的所有物(未实现）
void AI::dropKeeper(int n)
{
	vector<const Card*> cards;
	int chosen;
	for (int i = 0; i < n; i++)
	{
		chosen = rand() % _allKeepers[_ownNum].size();
		cards.push_back(_allKeepers[_ownNum][chosen]);
	}
	//发消息弃牌
	_mailbox.createMsg(DROP_KEEPER_I, cards);
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
	_mailbox.createMsg(CHOOSE_KEEPER_I, cards);
}
//弃规则，回应DROP_RULE_C消息
void AI::dropRule(int n)
{
	vector<const Card*> cards;
	vector<const Card*> temp = _rules;
	for (int i = 0; i < n; i++)
	{
		//选择规则牌
		int j = rand() % temp.size();
		cards.push_back(temp[j]);
		vector<const Card*>::iterator it = temp.begin() + j;
		temp.erase(it);
	}
	//发选规则的消息
	_mailbox.createMsg(DROP_RULE_I, cards);
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
	_mailbox.createMsg(CHOOSE_PLAYER_I, chosen);
}

//选目标，回应CHOOSE_GOAL_C消息
void AI::chooseGoal()
{
	int chosen = _rules.size() - 1 - rand() % 2; //两目标必在_rules的最后两个
	vector<const Card*> cards;
	cards.push_back(_rules[chosen]);
	//发选目标的消息
	_mailbox.createMsg(CHOOSE_GOAL_I,cards);
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
	}
}