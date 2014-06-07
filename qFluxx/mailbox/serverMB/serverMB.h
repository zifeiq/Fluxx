#include <iostream>
#include <vector>
#include <string>  
#include <sstream>
#include <winsock2.h>  
#include "CardLib.h"
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 1234; //默认PORT NUMBER = 1234
const int MAXBUFF = 512;
const int MAXCLIENT = 4;

enum MsgType{
	//服务器发送的消息类型
	ADD_PLAYER = 0,//增加玩家
	GAME_START,//游戏开始
	DRAW,//摸牌信息
	ROUND_BEGIN,//回合开始
	ACK,//确认信号
	NACK,//错误信号
	DROP_CARD_C,//弃牌命令
	DROP_KEEPER_C,//弃所有物命令
	CARD_NUM,//其他玩家手牌数量
	CARD_PLAYED,//当前出牌信息
	GAME_OVER,//游戏结束
	RULE,//规则信息
	KEEPER_UPDATE,//其他玩家所有物更新
	CARD_DROPED,  //其他玩家弃牌信息
	//客户端发送的消息类型
	REGISTER,//注册消息
	PLAY,//出牌信息
	DROP_CARD_I, //弃牌信息
	DROP_KEEPER_I //弃所有物信息
	//另外，客户端发的ACK,NACK与服务器的消息类型相同
};

class ServerMB //服务器信箱
{
public:
	ServerMB();
	~ServerMB();
	bool acceptNewClient();        //接收一个新客户
	std::string recvMsg(int num);  //接收来自第num个客户端消息
	bool sendMsg(int num ,std::string s); //向第num个客户端发送消息
	//bool getMsg(int playerNum, MsgType m, std::vector<Card*>& relatedCards, std::string& name);
	//getMsg函数重载以实现多种调用方式
	bool getMsg(int playerNum, MsgType& m);
	bool getMsg(int playerNum,MsgType& m, std::string& name);
	bool getMsg(int playerNum, MsgType& m, std::vector<const Card*>& relatedCards);
	//createMsg函数重载以实现多种调用方式
	bool createMsg(int playerNum, MsgType m); //发送ACK,NACK消息
	bool createMsg(int playerNum, MsgType m, int relatedPlayer, std::string name);//发送ADD_PLAYER消息
	bool createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards);//发送GAME_START,RULE消息
	bool createMsg(int playerNum, MsgType m, int relatedInfo); //发送ROUND_BEGIN,DROP_CARD_C,DROP_KEEPER_C,GAME_OVER消息
	bool createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards,int relatedInfo); //发送DRAW,CARD_PLAYED,CARD_DROPED消息
	bool createMsg(int playerNum, MsgType m, int relatedPlayer, int additional); //发送CARD_NUM消息
	bool createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards,int relatedPlayer, int additional); //发送KEEPER_UPDATE消息

private:
	SOCKET serverSock;
	int _clientNum;
	std::vector<SOCKET> clientSock;
	CardLib& _cards;
	const Card* str2Card(std::string s); //由消息字符串生成Card*
	std::string card2Str(const Card* c); //由Card*生成消息字符串
};