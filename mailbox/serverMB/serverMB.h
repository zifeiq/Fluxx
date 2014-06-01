#include <iostream>
#include <vector>
#include <string>  
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 1234; //默认PORT NUMBER = 1234
const int MAXBUFF = 512;
const int MAXCLIENT = 4;

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