#include <iostream>
#include <sstream>
//#include <windows.h>
//#include "../fluxxControl_code/player.h"

#include "../mailbox/serverMB/serverMB.h"
#include "../mailbox/clientMB/clientMB.h"

class AI
{
public:
	AI(int n);
	~AI();
	void run();				//运行函数，由上层调用，收到gameover消息后退出
	bool joinGame();		//向服务器注册并加入游戏
	void play();			//出牌，回应PLAY_C消息
	void dropCard(int);		//弃牌，回应DROP_CARD_C消息
	void dropKeeper(int);   //弃所有物，回应DROP_KEEPER_C消息
	void chooseKeeper(int);    //选择在场的所有物牌，回应CHOOSE_KEEPER_C, EXCHANGE_KEEPER_C消息
	void dropRule(int);        //弃规则，回应DROP_RULE_C消息
	void choosePlayer();    //选玩家，回应CHOOSE_PLAYER_C消息
	void chooseGoal();      //选目标，回应CHOOSE_GOAL_C消息
protected:
	ClientMB _mailbox;
	std::string _ownName;
	int _ownNum;
	int _playerNum;
	std::vector<const Card*> _ownCards;
	//std::vector<const Card*> _ownKeepers;
	std::vector<int> _allCardNum;
	std::vector<std::vector<const Card*>> _allKeepers;
	std::vector<const Card*> _rules;
};

std::string convert(MsgType m);