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
#include <netdb.h>   
#include <arpa/inet.h>
#endif

#include "../../cardLib_code/cardLib.h"
#include "../../config.h"
#ifdef WIN32
#pragma comment(lib,"ws2_32.lib") 
#endif

class ClientMB  //客户端信箱
{
public:
	ClientMB();
	~ClientMB();
	bool connectServer(const std::string server_ip);

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
protected:
	CardLib& _cards;
	bool ipCheck(const std::string s) const; //用于检测ip地址格式是否正确
	const Card* str2Card(std::string s); //由消息字符串生成Card*
	std::string card2Str(const Card* c); //由Card*生成消息字符串
private:
	int clientSock;
	bool sendMsg(std::string s);
	std::string recvMsg();
};

#endif