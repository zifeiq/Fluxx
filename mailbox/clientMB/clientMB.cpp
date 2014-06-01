#include"clientMB.h"
using namespace std;

ClientMB::ClientMB()
{
	//初始化winsock  
	WSADATA wsaD;
	WSAStartup(MAKEWORD(1, 1), &wsaD);
	//初始化客户端socket  
	clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

ClientMB::~ClientMB()
{
	closesocket(clientSock);
	WSACleanup();
}

bool ClientMB::connectServer(string server_ip)
{
	//连接服务器  
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(server_ip.c_str()); //指定服务器ip 
	servAddr.sin_port = htons(PORT);             //指定端口  
	if (connect(clientSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) //通过套接字连接主机
		return false;
	else
		return true;
}

bool ClientMB::sendMsg(string s)
{
	char buf[BUFFMAX];
	int index = 0;
	while (1)
	{
		string temp = s.substr(index, BUFFMAX - 1);
		memset(buf, 0, sizeof(buf));
		strcpy_s(buf, temp.c_str());
		if (send(clientSock, buf, strlen(buf) + 1, 0) == -1) //发送出错
			return false; 
		else
		{
			index += BUFFMAX - 1;
			if (index >= s.size())  //已发完字符串所有内容
				return true;
		}
	}
}

string ClientMB::recvMsg()
{
	string s;
	char buf[BUFFMAX];
	memset(buf, 0, sizeof(buf));
	if (recv(clientSock, buf, BUFFMAX, 0) != -1)
		s = buf;
	return s;
}