#include <iostream>
#include <vector>
#include <string>  
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 1234; //默认PORT NUMBER = 8080
const int BUFFMAX = 512;

class ClientMB  //客户端信箱
{
public:
	ClientMB();
	~ClientMB();
	bool connectServer(std::string server_ip);
	bool sendMsg(std::string s);
	std::string recvMsg();
private:
	SOCKET clientSock;
};