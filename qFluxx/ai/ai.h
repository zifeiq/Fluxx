#include <iostream>
#include <sstream>
//#include <windows.h>
//#include "../fluxxControl_code/player.h"

#include "../mailbox/serverMB/serverMB.h"
#include "../mailbox/clientMB/clientMB.h"
//AI基类，简单模式
class AI
{
public:
	AI(int n);
	virtual ~AI();
	void run();				//运行函数，由上层调用，收到gameover消息后退出
	
protected:
	ClientMB _mailbox;
	std::string _ownName;
	int _ownNum;
	int _playerNum;
	std::vector<const Card*> _ownCards;
	std::vector<int> _allCardNum;
	std::vector<std::vector<const Card*>> _allKeepers;
	std::vector<const Card*> _rules;

	bool joinGame();		//向服务器注册并加入游戏
	virtual void play();			//出牌，回应PLAY_C消息
	virtual void dropCard(int);		//弃牌，回应DROP_CARD_C消息
	virtual void dropKeeper(int);   //弃所有物，回应DROP_KEEPER_C消息
	virtual void chooseKeeper(int);    //选择在场的所有物牌，回应CHOOSE_KEEPER_C, EXCHANGE_KEEPER_C消息
	virtual void dropRule(int);        //弃规则，回应DROP_RULE_C消息
	virtual void choosePlayer();    //选玩家，回应CHOOSE_PLAYER_C消息
	virtual void chooseGoal();      //选目标，回应CHOOSE_GOAL_C消息
};

//AI子类，困难模式
class advancedAI:public AI
{
public:
	advancedAI(int n): AI(n){}
	virtual ~advancedAI();
protected:
	virtual void play();			//出牌，回应PLAY_C消息
	virtual void dropCard(int);		//弃牌，回应DROP_CARD_C消息
	virtual void dropKeeper(int);   //弃所有物，回应DROP_KEEPER_C消息
	virtual void chooseKeeper(int);    //选择在场的所有物牌，回应CHOOSE_KEEPER_C, EXCHANGE_KEEPER_C消息
	virtual void dropRule(int);        //弃规则，回应DROP_RULE_C消息
	virtual void choosePlayer();    //选玩家，回应CHOOSE_PLAYER_C消息
	virtual void chooseGoal();      //选目标，回应CHOOSE_GOAL_C消息
	//辅助函数
	bool betterCard(const Card* c1, const Card* c2);//true: c1比c2好 所有物>相关目标牌>正规则牌>行动牌>无关规则牌>无关目标牌>负规则牌
	bool isRelatedGoal(const Card*);//检测自己的手牌或所有物中是否包含此目标对应的所有物
	bool isRelatedKeeper(const Card*); //检测自己的手牌或当前目标中是否包含此所有物
	int isRelatedRule(const Card*);//return -1: 负规则（自己的手牌、所有物超过上限，会受到不好的影响）;0: 无关规则; 1:正规则（自己会受到好影响）
};


std::string convert(MsgType m);