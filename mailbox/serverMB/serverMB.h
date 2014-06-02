#include <iostream>
#include <vector>
#include <string>  
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 1234; //默认PORT NUMBER = 1234
const int MAXBUFF = 512;
const int MAXCLIENT = 4;

enum msgType{
	ADDPLAYER = 0,//增加玩家
	GAMESTART,//游戏开始
	ROUNDBEGIN,//回合开始
	ACK,//确认信号
	NACK,//错误信号
	DROPCARD,//弃牌命令
	DROPKEEPER,//弃所有物命令
	CARDCOUNT,//其他玩家手牌数量
	CARDPLAYING,//当前出牌信息
	GAMEOVER,//游戏结束
	RULE,//规则信息
	KEEPERUPDATE//其他玩家所有物更新
}
class ServerMB //服务器信箱
{
public:
	ServerMB();
	~ServerMB();
	bool acceptNewClient();        //接收一个新客户
	std::string recvMsg(int num);  //接收来自第num个客户端消息
	bool sendMsg(int num ,std::string s); //向第num个客户端发送消息

private:
	SOCKET serverSock;
	int _clientNum;
	std::vector<SOCKET> clientSock;
};