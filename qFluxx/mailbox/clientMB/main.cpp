#include "clientMB.h"
using namespace std;

int main()
{
	ClientMB client;
	cout << "客户端已建立" << endl;
	if (client.connectServer("127.0.0.1"))
		cout << "已连接到服务器" << endl;
	else
	{
		cout << "连接出错" << endl;
		return -1;
	}

	//client发消息测试
	vector<string> cards;
	client.createMsg(REGISTER, cards, "Stella");
	cout << "已发送第一条消息:REGISTER" << endl;
	client.createMsg(ACK, cards);
	cout << "已发送第二条消息:ACK" << endl;
	client.createMsg(NACK, cards);
	cout << "已发送第三条消息:NACK" << endl;

	cards.push_back("000");
	if(client.createMsg(PLAY, cards))
	cout << "已发送第四条消息:PLAY" << endl;
	else cout << "第四条消息发送出错" << endl;
	cards.clear();
	cards.push_back("102");
	if (client.createMsg(PLAY, cards))
		cout << "已发送第四条消息:PLAY" << endl;
	else cout << "第四条消息发送出错" << endl;
	cards.clear();
	cards.push_back("142");
	if (client.createMsg(PLAY, cards))
		cout << "已发送第四条消息:PLAY" << endl;
	else cout << "第四条消息发送出错" << endl;

	cards.push_back("158");
	if (client.createMsg(DROPCARD, cards))
		cout << "已发送第五条消息:DROPCARD" << endl;
	else cout << "第五条消息发送出错" << endl;

	if (client.createMsg(DROPKEEPER, cards))
		cout << "已发送第六条消息:DROPKEEPER" << endl;
	else cout << "第六条消息发送出错" << endl;

	cards.clear();
	cards.push_back("201");
	if (client.createMsg(DROPKEEPER, cards))
		cout << "已发送第六条消息:DROPKEEPER" << endl;
	else cout << "第六条消息发送出错" << endl;
	/*client.sendMsg("From client 0 to server!");
	cout << "已发送" << endl;
	cout << "FROM server： "<<client.recvMsg() << endl;
	while (1)
	{
		string s;
		getline(cin,s);
		client.sendMsg(s);
		cout << "已发送" << endl;
		cout << "FROM server： " << client.recvMsg() << endl;
	}*/
}