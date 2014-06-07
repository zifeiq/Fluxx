#include "serverMB.h"
using namespace std;
//For test!!
int main()
{
	ServerMB server;
	cout << "已建立服务器" << endl;
	server.acceptNewClient();
	//server.acceptNewClient();
	cout << "已建立连接" << endl;
	MsgType m;
	vector<Card*> c;
	string s;
	for (int j = 0; j < 6; j++)
	{
		server.getMsg(0, m, s, c);
		cout << "接收第"<<j<<"条消息： m = " << m << "\ts = " << s << endl << "卡牌信息：";
		for (int i = 0; i < c.size(); i++)
			cout << c[i]->getType() << c[i]->getNum() << "\t";
		cout << endl << endl;
	}

	/*while (1){
		cout << "From client0:" << server.recvMsg(0) << endl;
		cout << "From client1:" << server.recvMsg(1) << endl;
		server.sendMsg(0, "from server to client 0!");
		server.sendMsg(1, "from server to client 1!");
		cout << "已发送" << endl;
	}*/
	//server.sendMsg(0);
}
