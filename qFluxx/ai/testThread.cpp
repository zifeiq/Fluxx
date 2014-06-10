#include "ai.h"
using namespace std;

DWORD WINAPI threadAI(LPVOID lpParameter);
DWORD WINAPI threadServer(LPVOID lpParameter);
extern string convert(MsgType m);

int main()
{
	string ip;
	AI ai(0);
	ai.run();
}


/*
//test 多线程成功~~~
int main()
{
	ClientMB client;
	//int pServ = 1, pClient;
	DWORD p[4]; int i = 0;
	CloseHandle(CreateThread(NULL, 0, threadServer, (LPVOID)i, 0, &p[i]));
	i++;
	CloseHandle(CreateThread(NULL, 0, threadAI, (LPVOID)i, 0, &p[i]));

	cout << "player: 你好世界！" << endl;
	string playerName = "player";
	MsgType m;
	if (client.connectServer("127.0.0.1"))
		cout << playerName << ": 与服务器建立连接" << endl;
	else
		cout << playerName << ": 与服务器连接失败" << endl;
	client.createMsg(MsgType::REGISTER, playerName);
	cout << playerName << ": 发送REGISTER给server" << endl;
	client.getMsg(m);
	cout << playerName << ": 接收server消息 " << m << endl;

}*/

DWORD WINAPI threadAI(LPVOID lpParameter)
{
	int para = (int)lpParameter;
	ClientMB client;
	MsgType m;
	stringstream ss;
	ss << para;
	string playerName = "AI" + ss.str();
	cout << endl << playerName << ": 你好世界！" << endl;

	if(client.connectServer("127.0.0.1"))
		cout << playerName << ": 与服务器建立连接" << endl;
	else
		cout << playerName << ": 与服务器连接失败" << endl;
	client.createMsg(MsgType::REGISTER, playerName);
	cout << playerName << ": 发送REGISTER给server" << endl;
	client.getMsg(m);
	cout << playerName << ": 接收server消息 " <<m<< endl;
	cout << playerName<<": 退出" << endl;
	return 0;
}

DWORD WINAPI threadServer(LPVOID lpParameter)
{
	int para = (int)lpParameter;
	cout << endl<<"server"<<para<<": 你好世界！" << endl;
	ServerMB server;
	MsgType m;
	string playerName;
	m = MsgType::REGISTER;
	for (int i = 0; i < 2; i++)
	{
		server.acceptNewClient();
		cout << "server:与一个客户端建立连接" << endl;
	}
	for (int i = 0; i < 2; i++)
	{
		server.getMsg(i, m, playerName);
		cout << "server: 接到第" << i << "个玩家发的消息：" << playerName << endl;
		server.createMsg(i, MsgType::ADD_PLAYER);
		cout << "server: 发送给第" << i << "个玩家ADD_PLAYER消息" << endl;
	}
	cout << "server:退出" << endl;
	return 0;
}