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
	ADDPLAYER = 0,//增加玩家
	GAMESTART,//游戏开始
	DRAW,//摸牌信息
	ROUNDBEGIN,//回合开始
	ACK,//确认信号
	NACK,//错误信号
	DROPCARD,//弃牌命令
	DROPKEEPER,//弃所有物命令
	CARDNUM,//其他玩家手牌数量
	CARDPLAYING,//当前出牌信息
	GAMEOVER,//游戏结束
	RULE,//规则信息
	KEEPERUPDATE,//其他玩家所有物更新
	//客户端发送的消息类型
	REGISTER,//注册消息
	PLAY//出牌信息
	//另外，客户端发的ACK,NACK,DROPCARD,DROPKEEPER与服务器的消息类型相同
};

class ServerMB //服务器信箱
{
public:
	ServerMB();
	~ServerMB();
	bool acceptNewClient();        //接收一个新客户
	std::string recvMsg(int num);  //接收来自第num个客户端消息
	bool sendMsg(int num ,std::string s); //向第num个客户端发送消息
	bool getMsg(int playerNum, MsgType& m, std::vector<Card*>& relatedCards, std::string& name);
	bool createMsg(int playerNum, MsgType m, std::vector<Card*> relatedCards, int relatedPlayer = 0, int additional = -1, std::string name = "");

private:
	SOCKET serverSock;
	int _clientNum;
	std::vector<SOCKET> clientSock;
	CardLib& _cards = CardLib::getLib();
	Card* str2Card(std::string s); //由消息字符串生成Card*
	std::string card2Str(Card* c); //由Card*生成消息字符串
};