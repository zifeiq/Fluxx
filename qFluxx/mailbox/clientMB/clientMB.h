#include <iostream>
#include <vector>
#include <string>  
#include <sstream>
#include <cctype>
#include <winsock2.h>  
#include "cardLib.h"
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 1234; //默认PORT NUMBER = 1234
const int BUFFMAX = 512;

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
	
class ClientMB  //客户端信箱
{
public:
	ClientMB();
	~ClientMB();
	bool connectServer(const std::string server_ip);
	bool sendMsg(std::string s);
	std::string recvMsg();
		//createMsg函数重载以实现多种调用方式
	bool createMsg(MsgType m); //发送ACK、NACK消息
	bool createMsg(MsgType m, std::string name);//发送REGISTER消息
	bool createMsg(MsgType m, std::vector<const Card*>relatedCards);//发送PLAY,DROP_CARD_I,DROP_KEEPER_I消息

		//getMsg函数重载以实现多种调用方式
	bool getMsg(MsgType& m); //发送ACK,NACK消息
	bool getMsg(MsgType& m, int& relatedPlayer, std::string& name);//发送ADD_PLAYER消息
	bool getMsg(MsgType& m, std::vector<const Card*>& relatedCards);//发送GAME_START,RULE消息
	bool getMsg(MsgType& m, int relatedInfo); //发送ROUND_BEGIN,DROP_CARD_C,DROP_KEEPER_C,GAME_OVER消息
	bool getMsg(MsgType& m, std::vector<const Card*> relatedCards,int relatedInfo); //发送DRAW,CARD_PLAYED,CARD_DROPED消息
	bool getMsg(MsgType& m, int relatedPlayer, int additional); //发送CARD_NUM消息
	bool getMsg(MsgType& m, std::vector<const Card*> relatedCards,int relatedPlayer, int additional); //发送KEEPER_UPDATE消息
private:
	SOCKET clientSock;
	CardLib& _cards;
	bool ipCheck(const std::string s) const; //用于检测ip地址格式是否正确
	const Card* str2Card(std::string s); //由消息字符串生成Card*
	std::string card2Str(const Card* c); //由Card*生成消息字符串
};

