#include <iostream>
#include <vector>
#include <string>  
#include <cctype>
//#include <sstream>
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 1234; //默认PORT NUMBER = 1234
const int BUFFMAX = 512;

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

class ClientMB  //客户端信箱
{
public:
	ClientMB();
	~ClientMB();
	bool connectServer(const std::string server_ip);
	bool sendMsg(std::string s);
	std::string recvMsg();
	bool getMsg(MsgType& m,std::vector<std::string>& relatedCards, int& playerNum, int& additional, std::string& name);
	bool createMsg(MsgType m, std::vector<std::string>relatedCards, std::string name = "");
	//bool createMsg(MsgType m = NACK, std::string name = "", std::vector<std::string>::iterator*  relatedCards = NULL);
private:
	SOCKET clientSock;
	bool ipCheck(const std::string s) const; //用于检测ip地址格式是否正确
	bool isEffectiveCard(std::string s) const;  //用于检测传入卡牌是否有效 //未加行动牌！！！！！！
};

