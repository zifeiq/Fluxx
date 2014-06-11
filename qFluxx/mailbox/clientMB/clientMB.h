#ifndef CLIENTMB_H
#define CLIENTMB_H

#include <iostream>
#include <vector>
#include <string>  
#include <sstream>
#include <cctype>
#if defined(WIN32)//WINDOWS
#include <winsock2.h>
#else//LINUX
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>      // define structures like hostent
#endif
//#include "cardLib.h"
#include "../../cardLib_code/cardLib.h"
#include "../../config.h"
#ifdef WIN32
#pragma comment(lib,"ws2_32.lib") 
#endif
//const int PORT = 1234; //默认PORT NUMBER = 1234
//const int BUFFMAX = 512;
//extern enum MsgType;
/*enum MsgType{
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
//extern enum MsgType;
class ClientMB  //客户端信箱
{
public:
	ClientMB();
	~ClientMB();
	bool connectServer(const std::string server_ip);
	bool sendMsg(std::string s);
	std::string recvMsg();
		//createMsg函数重载以实现多种调用方式
	bool createMsg(MsgType m); //发送NACK消息
	bool createMsg(MsgType m, std::string name);//发送REGISTER消息
	bool createMsg(MsgType m, std::vector<const Card*>relatedCards);//发送PLAY_I,DROP_CARD_I,DROP_KEEPER_I,CHOOSE_KEEPER_I,DROP_RULE_I,CHOOSE_GOAL_I消息
	bool createMsg(MsgType m, int relatedPlayer); //发送CHOOSE_PLAYER_I消息

		//getMsg函数重载以实现多种调用方式
	bool getMsg(MsgType& m); //接收所有消息类型
	bool getMsg(MsgType m, int& relatedPlayer, std::string& name);//接收ADD_PLAYER消息
	bool getMsg(MsgType m, std::vector<const Card*>& relatedCards);//接收GAME_START,RULE消息
	bool getMsg(MsgType m, int& relatedInfo); //接收DROP_CARD_C,DROP_KEEPER_C,GAME_OVER,DROP_RULE_C消息
	bool getMsg(MsgType m, std::vector<const Card*>& relatedCards,int& relatedInfo); //接收CARD_PLAYED,CARD_DROPED,CARD_UPDATE消息
	bool getMsg(MsgType m, int& relatedPlayer, int& additional); //接收ROUND_BEGIN，CARD_NUM消息
	bool getMsg(MsgType m, std::vector<const Card*>& relatedCards,int& relatedPlayer, int& additional); //接收KEEPER_UPDATE消息
private:
	//SOCKET clientSock;
	int clientSock;
	CardLib& _cards;
	bool ipCheck(const std::string s) const; //用于检测ip地址格式是否正确
	const Card* str2Card(std::string s); //由消息字符串生成Card*
	std::string card2Str(const Card* c); //由Card*生成消息字符串
};

#endif