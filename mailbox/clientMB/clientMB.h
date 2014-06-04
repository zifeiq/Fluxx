#include <iostream>
#include <vector>
#include <string>  
#include <cctype>
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 1234; //默认PORT NUMBER = 1234
const int BUFFMAX = 512;

class ClientMB  //客户端信箱
{
public:
	ClientMB();
	~ClientMB();
	bool connectServer(const std::string server_ip);
	bool sendMsg(std::string s);
	std::string recvMsg();
private:
	SOCKET clientSock;
	bool ipCheck(const std::string s) const; //用于检测ip地址格式是否正确
};

