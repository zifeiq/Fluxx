#ifndef SERVERMB_H
#define SERVERMB_H

#include <iostream>
#include <vector>
#include <string>  
#include <sstream>

#if defined(WIN32)//WINDOWS
#include <winsock2.h>
#else//LINUX
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>      // define structures like hostent
#include <arpa/inet.h>
#endif

#include "../../cardLib_code/cardLib.h"
#include "../../config.h"

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib") 
#endif
//const int PORT = 1234; //默认PORT NUMBER = 1234
//const int MAXBUFF = 512;
//const int MAXCLIENT = 4;
/*
enum MsgType{
	NACK,//错误信号, 客户端和服务器共用
	//服务器发送的消息类型
	ADD_PLAYER,//增加玩家
	GAME_START,//游戏开始
	//DRAW,//摸牌信息
	ROUND_BEGIN,//回合开始
	CARD_UPDATE,//手牌更新信息
	PLAY_C, //出牌命令
	//ACK,//确认信号
	CARD_NUM,//其他玩家手牌数量因摸牌或行动牌而改变的通知
	CARD_PLAYED,//当前出牌信息
	CARD_DROPED,  //其他玩家弃牌信息
	RULE,//规则信息
	KEEPER_UPDATE,//玩家所有物更新
	DROP_CARD_C,//弃牌命令
	DROP_KEEPER_C,//弃所有物命令
	GAME_OVER,//游戏结束
	//CARD_STOLEN,//抽取的其他玩家卡牌（需要立刻打出），用于行动牌“取而用之”
	CHOOSE_PLAYER_C, //选择一位玩家的命令，用于行动牌“交换手牌”“取而用之”
	CHOOSE_KEEPER_C, //选择一张其他玩家的行动牌命令，用于行动牌“丢弃”“偷窃”
	EXCHANGE_KEEPER_C, //选择交换一张所有物，用于行动牌“交换所有物”
	DROP_RULE_C,//丢弃规则牌的命令，用于行动牌“取消规则”“简化一下”
	CHOOSE_GOAL_C, //选择目标牌的命令，用于双重目标时对某目标的替换

	//客户端发送的消息类型
	REGISTER,//注册消息
	PLAY_I,//出牌信息
	DROP_CARD_I, //弃牌信息
	DROP_KEEPER_I, //弃所有物信息
	CHOOSE_KEEPER_I,//选择的所有物信息，用于行动牌“丢弃”“偷窃”“交换所有物”
	DROP_RULE_I,//选择的规则牌信息，用于行动牌“取消规则”“简化一下”
	CHOOSE_PLAYER_I, //选择的玩家编号， 用于行动牌“交换手牌”“取而用之”
	CHOOSE_GOAL_I
};*/

class ServerMB //服务器信箱
{
public:
	ServerMB();
	~ServerMB();
	bool acceptNewClient();        //接收一个新客户
	std::string recvMsg(int num);  //接收来自第num个客户端消息
	bool sendMsg(int num ,std::string s); //向第num个客户端发送消息

	//getMsg函数重载以实现多种调用方式
	//bool getMsg(int playerNum, MsgType m);
	bool getMsg(int playerNum,MsgType m, std::string& name);
	bool getMsg(int playerNum, MsgType m, std::vector<const Card*>& relatedCards);
	bool getMsg(int playerNum, MsgType m, int& relatedPlayer);
	//createMsg函数重载以实现多种调用方式
	bool createMsg(int playerNum, MsgType m); //发送所有消息
	bool createMsg(int playerNum, MsgType m, int relatedPlayer, std::string name);//发送ADD_PLAYER消息
	bool createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards);//发送收GAME_START,RULE消息
	bool createMsg(int playerNum, MsgType m, int relatedInfo); //发送DROP_CARD_C,DROP_KEEPER_C,GAME_OVER,DROP_RULE_C消息
	bool createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards,int relatedInfo); //发送CARD_PLAYED,CARD_DROPED,CARD_UPDATE消息
	bool createMsg(int playerNum, MsgType m, int relatedPlayer, int additional); //发送ROUND_BEGIN，CARD_NUM消息
	bool createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards,int relatedPlayer, int additional); //发送KEEPER_UPDATE消息

private:
	//SOCKET serverSock;
	int serverSock;
	int _clientNum;
	//std::vector<SOCKET> clientSock;
	std::vector<int> clientSock;
	CardLib& _cards;
	const Card* str2Card(std::string s); //由消息字符串生成Card*
	std::string card2Str(const Card* c); //由Card*生成消息字符串
};
#endif