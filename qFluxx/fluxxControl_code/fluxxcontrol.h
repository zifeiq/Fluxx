#ifndef FLUXXCONTROL_H
#define FLUXXCONTROL_H

#include <vector>
#include <string>
#include <time.h>
#include "..\fluxxControl_code\fluxxrules.h"
#include "..\cardLib_code\cardLib.h"
#include "..\mailbox\serverMB\serverMB.h"
#include "..\fluxxControl_code\player.h"

typedef int ruleMsg;
extern enum MsgType;
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
	void dealCard(); //发牌,入口参数为从规则中得到的信息
	void playCard();//出牌阶段控制函数，入口参数为从规则中得到的信息
	void settleCard(const Card&); //单张出牌与结算，入口参数为卡牌的引用
	void dropCard();//弃牌阶段控制函数，口参数为从规则中得到的信息
	void removeCard(Player&);//移除手牌，需要指定目标
	int checkWinner();

	int getclientnum(Player&);
	~fluxxControl ();

private:
	//单个回合控制相关
	gameState presentState;
	Player& presentPlayer;
	fluxxRules rule;
	//信息传递相关
	int msgbufAdditional;//附加消息编号
	int clientNum;//选中的client编号
	MsgType msgbufMsgtype;//消息类型缓存
	std::string msgbufString;//字符串缓存
	std::vector<Card*> msgbufCards;//卡牌消息缓存
	//整局游戏控制相关
	CardLib& cards;
	ServerMB msgBox;
	std::vector<Player> players;//所有玩家
	std::vector<Card*> deck;//牌堆
	std::vector<Card*> droppeddeck;//弃牌堆
};

#endif