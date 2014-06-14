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
#include <netdb.h>      
#include <arpa/inet.h>
#endif

#include "../../cardLib_code/cardLib.h"
#include "../../config.h"

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib") 
#endif

class ServerMB //服务器信箱
{
public:
	ServerMB();
	~ServerMB();
	bool acceptNewClient();        //接收一个新客户

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
	int serverSock;
	int _clientNum;
	std::vector<int> clientSock;
	CardLib& _cards;
	std::string recvMsg(int num);  //接收来自第num个客户端消息
	bool sendMsg(int num, std::string s); //向第num个客户端发送消息
	const Card* str2Card(std::string s); //由消息字符串生成Card*
	std::string card2Str(const Card* c); //由Card*生成消息字符串
};
#endif