#include "serverMB.h"
using namespace std;

ServerMB::ServerMB():
_clientNum(0)
{
	//初始化winsock   
	WSADATA wsaD;
	WSAStartup(MAKEWORD(1, 1), &wsaD);
	//初始化服务器监听socket
	serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 绑定主机地址和监听套接字  
	struct sockaddr_in servAddr;   //主机地址   
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT); //指定端口  
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(serverSock, (struct sockaddr *)&servAddr, sizeof(servAddr));
	//启动监听  
	listen(serverSock, MAXCLIENT);   //最多同时建立4个连接
}

ServerMB::~ServerMB()
{
	for (int i = 0; i < _clientNum; i++)
		closesocket(clientSock[i]);
	closesocket(serverSock);
	WSACleanup();
}

bool ServerMB::sendMsg(int num,string s)
{
	if (num >= _clientNum)   //参数不正确
		return false;
	char buf[MAXBUFF];
	int index = 0;
	while (1)
	{
		string temp = s.substr(index, MAXBUFF - 1);
		memset(buf, 0, sizeof(buf));
		strcpy_s(buf, temp.c_str());
		if (send(clientSock[num], buf, strlen(buf) + 1, 0) == -1) //发送出错
			return false;
		else
		{
			index += MAXBUFF - 1;
			if (index >= s.size())  //已发完字符串所有内容
				return true;
		}
	}
}

string ServerMB::recvMsg(int num)
{
	if (num >= _clientNum)   //参数不正确
		return "";

	string s;
	char buf[MAXBUFF];
	memset(buf, 0, sizeof(buf));
	int recvLen = 0;
	if (recv(clientSock[num], buf, MAXBUFF, 0) != -1)
		s = buf;
	return s;
}

bool ServerMB::acceptNewClient()
{
	if (_clientNum == MAXCLIENT)
		return false;

	SOCKADDR_IN clientAddr;
	int len = sizeof(sockaddr);
	SOCKET newClient = accept(serverSock, (sockaddr*)&clientAddr, &len);
	if (newClient != INVALID_SOCKET)
	{
		clientSock.push_back(newClient);
		_clientNum++;
		return true;
	}
	else
		return false;
}