#include <iostream>  
#include <string>  
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 
const int PORT = 8080; //默认PORT NUMBER = 8080
const int BUFFMAX = 512;

class MailBox  //信箱的接口类
{
public:
	virtual bool sendMsg(std::string s) = 0;
	virtual std::string recvMsg() = 0;
	//virtual std::string getRecvBuff() const= 0;
	//virtual void setSendBuff(std::string) = 0;
	virtual ~MailBox(){}
};

class ClientMB : public MailBox //客户端信箱
{
public:
	ClientMB() :ClientMB("127.0.0.1"){} 
	ClientMB(std::string server_ip);
	~ClientMB();
	virtual bool sendMsg(std::string s);
	virtual std::string recvMsg();
private:
	SOCKET clientSock;
};

class ServerMB : public MailBox //客户端信箱
{
public:
	ServerMB();
	~ServerMB();
	virtual bool sendMsg(std::string s);
	virtual std::string recvMsg();
private:
	SOCKET serverSock;
};
