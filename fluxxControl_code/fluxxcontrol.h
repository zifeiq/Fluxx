#ifndef FLUXXCONTROL_H
#define FLUXXCONTROL_H

#include <vector>
#inlcude <string>
#include <time.h>
#include "..\fluxxControl_code\fluxxrules.h"
#include "..\cardLib_code\cardLib.h"
#include "..\mailbox\serverMB\serverMB.h"
#include "player.h"

typedef int ruleMsg;
extern enum msgType;
enum gameState{ 
	WaitforPlayers,
	PlayersTurns,
};
class fluxxControl {
public:
	fluxxControl ();
	void initrules();//初始化规则，在默认构造函数中使用
	void addplayers();//添加玩家，在默认构造函数和其他适当的时机使用
	void setpresentPlayer(Player&);
	void shuffleCard();//洗牌
	void dealCard(ruleMsg); //发牌,入口参数为从规则中得到的信息
	void playCard(ruleMsg);//出牌阶段控制函数，入口参数为从规则中得到的信息
	void settleCard(const Card&); //单张出牌与结算，入口参数为卡牌的引用
	void dropCard(ruleMsg);//弃牌阶段控制函数，口参数为从规则中得到的信息

	int getclientnum(Player&);
	~fluxxControl ();

private:
	//单个回合控制相关
	gameState presentState;
	Player& presentPlayer;
	fluxxRules rule;
	//信息传递相关
	int msgbufAdditional;
	int clientNum;
	msgType msgbufMsgtype;
	std::string msgbufString;
	std::vector<const Card&> msgbufCards;
	//整局游戏控制相关
	CardLib& cards;
	ServerMB msgBox;
	std::vector<Player> players;
	std::vector<const Card&> deck;
	std::vector<const Card&> droppeddeck;
};

#endif