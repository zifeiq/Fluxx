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
	WAIT_FOR_PLAYERS,
	DEAL_ORIGINAL_CARD,
};
class fluxxControl {
public:
	fluxxControl ();
	void initrules();//初始化规则
	void addplayers();//添加玩家
	void setpresentPlayer(Player&);
	void shuffleCard();//洗牌
	void dealCard(); //发牌
	void playCard();//出牌
	void settleCard(const Card&); //单张出牌与结算，入口参数为卡牌的引用
	void dropCard();//弃牌阶段控制函数
	void removeCard(Player&);//移除手牌
	void stagecontrol();
	void updateRules();
	void updateKeepers();
	int checkWinner();
	int getclientnum(Player&);
	~fluxxControl (){};

private:
	//单个回合控制相关
	Player presentPlayer;
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
	gameState presentState;
	std::vector<Player> players;//所有玩家
	std::vector<Card*> deck;//牌堆
	std::vector<Card*> droppeddeck;//弃牌堆
};

#endif