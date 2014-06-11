#ifndef FLUXXCONTROL_H
#define FLUXXCONTROL_H

#include <vector>
#include <string>
#include <time.h>
#include "..\fluxxControl_code\fluxxrules.h"
#include "..\cardLib_code\cardLib.h"
#include "..\mailbox\serverMB\serverMB.h"
#include "..\fluxxControl_code\player.h"

//extern enum MsgType;
enum gameState{ 
	WAIT_FOR_PLAYERS,
	DEAL_ORIGINAL_CARD,
	ROUND_PREPARE,
	PLAYING_CARD,
	DROPING_CARD,
	GAME_END,
};
class fluxxControl {
public:
	fluxxControl (fluxxRules&);
	void addPlayers();//添加玩家
	/***基本操作函数***/
	void dealCard(int); //发牌
	void playCard(const Card*);//出牌
	void dropCard(int);//弃牌
	/***************************************/
	void setpresentPlayer(int);//修改当前前面函数的作用对象
	void actioncard(const Card& act_card);
	//整局游戏控制
	void fluxxRun();

	~fluxxControl ();

private:
	void _shuffleCard();//洗牌
	void _settleCard(const Card*); //单张结算
	void _updateRules();
	void _updateKeepers();
	void _settleKeepercard(const Card*);
	void _settleRulecard(const Card*);
	void _settleGoalcard(const Card*);
	int _checkCntdraw();//计算摸牌量
	int _checkCntplay();//计算出牌量
	int _checkCntdrop();
	int _checkWinner();//检查胜利
	int _getclientnum(Player&);//计算用于消息传递的玩家编号
	//单个回合控制相关
	Player *presentPlayer;
	fluxxRules rule;
	//信息传递相关
	int msgbufAdditional;//附加消息编号
	int clientNum;//选中的client编号
	MsgType msgbufMsgtype;//消息类型缓存
	std::string msgbufString;//字符串缓存
	std::vector<const Card*> msgbufCards;//卡牌消息缓存
	//整局游戏控制相关
	CardLib& cards;
	ServerMB msgBox;
	gameState presentState;
	std::vector<Player*> players;//所有玩家
	std::vector<const Card*> deck;//牌堆
	std::vector<const Card*> droppeddeck;//弃牌堆
};
#endif