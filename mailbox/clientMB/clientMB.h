#include <iostream>
#include <vector>
#include <string>  
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 1234; //默认PORT NUMBER = 8080
const int BUFFMAX = 512;

/*
class MailBox  //信箱的接口类
{
public:
	virtual bool sendMsg(std::string s) = 0;
	virtual std::string recvMsg() = 0;
	virtual ~MailBox(){}
};
*/

class ClientMB  //客户端信箱
{
public:
	ClientMB() :ClientMB("127.0.0.1"){} 
	ClientMB(std::string server_ip);
	~ClientMB();
	 bool sendMsg(std::string s);
	 std::string recvMsg();
private:
	SOCKET clientSock;
};

/*
struct tparam
{
	SOCKET clientSock;
	std::string sendBuff;
	std::string recvBuff;
};
*/


//void ThreadProc(ServerMB& mailbox);

