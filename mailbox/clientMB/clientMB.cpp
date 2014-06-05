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

bool ClientMB::connectServer(const string server_ip)
{
	if (!ipCheck(server_ip))   //ip地址的格式错误
		return false;
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

bool ClientMB::createMsg(MsgType m, vector<string> relatedCards,string name)
{
	//解析参数并生成消息字符串
	string s;
	int num;
	switch (m)
	{
	case REGISTER:
		if (name == "")			//未传入玩家名字
			return false;
		else
			s = "0" + name;
		break;
	case ACK:
		s = "1";
		break;
	case NACK:
		s = "2";
		break;
	case PLAY:
		if (relatedCards.size() != 1)//未传入一张牌
			return false;
		else{
			if (isEffectiveCard(relatedCards[0]))
				s = "3" + relatedCards[0];
			else
				return false;
			
			break;
		}
	case DROPCARD:
		if (relatedCards.size() == 0)//未传入卡牌消息
			return false;
		else{
			s = "4";
			for (int i = 0; i < relatedCards.size(); i++)
			{
				if (isEffectiveCard(relatedCards[i]))
					s += relatedCards[i];
				else
					return false;
			}
			break;
		}
	case DROPKEEPER:
		if (relatedCards.size() == 0)//未传入卡牌消息
			return false;
		else{
			s = "5";
			for (int i = 0; i < relatedCards.size(); i++)
			{
				//检测是否为有效的所有物牌编号
				int j;
				for (j = 0; j < relatedCards[i].size();j++)
					if (!isdigit(relatedCards[i][j])) break; //检测到非数字
				if (j != relatedCards[i].size())   //包含非数字的字符
					return false;
				num = atoi(relatedCards[i].c_str());
				if (num / 100 == 2 && 0 < (num - 200) < 19) 
					s += relatedCards[i];
				else
					return false;
			}
			break;
		}
	default: return false;
	}
	//发送消息
	if (!sendMsg(s))
		return false;
	else
		return true;
}
//bool ClientMB::getMsg(MsgType& m, std::string& name, int& playerNum, vector<int>& relatedCards, int& additional)
//{
//	string s = recvMsg()
//}

bool ClientMB::ipCheck(const string s) const
{
	int counter = 0, cnum = 0, cpoint = 0;
	string numStr;
	for (int i = 0; i < s.size(); i++)
	{
		if (isdigit(s[i]))
			counter++;
		else if (s[i] == '.')
		{
			cpoint++;
			numStr = s.substr(i - counter, counter);
		}
		else                //包含除数字和'.'之外的字符
			return false;
		if (i == s.size() - 1)  //到达字符串末尾
		{
			if (numStr == "")
				numStr = s.substr(i + 1 - counter, counter);
			else                //末尾为'.'
				return false;
		}

		if (numStr != "")   //检测到数字串
		{
			counter = 0;
			int tempInt = atoi(numStr.c_str());
			numStr = "";
			if (tempInt < 0 || tempInt > 255)  //数字不在0~255范围内
				return false;
			else
			{
				cnum++;
			}
		}
	}
	return (cnum == 4 && cpoint == 3);      //检测是否包含4段数字和3个'.'
}

bool ClientMB::isEffectiveCard(string s) const
{
	//检测是否仅包含数字
	int j;
	for (j = 0; j < s.size(); j++)
		if (!isdigit(s[j])) break; //检测到非数字
	if (j !=s.size())   //包含非数字的字符
		return false;
	int num = atoi(s.c_str());
	int type = num / 100;
	int ruleType,ruleNum;
	switch (type)
	{
	case 0: return false; //不可传入基本规则牌或其他数字
	case 1:               //新规则牌
		ruleType = (num - 100) / 10;
		ruleNum = num - 100 - ruleType * 10;
		if (ruleType == 1 && 0 <= ruleNum <= 2) //手牌上限规则牌
			return true;
		else if (ruleType == 2 && 2 <= ruleNum <= 4)//所有物个数规则牌
			return true;
		else if (ruleType == 3 && 2 <= ruleNum <= 5)//抓牌张数规则牌
			return true;
		else if (ruleType == 4 && 2 <= ruleNum <= 4)//出牌张数规则牌
			return true;
		else if (ruleType == 5 && 1 <= ruleNum <= 8)//其他规则牌
			return true;
		else
			return false;
	case 2:       //所有物牌
		if (1 <= (num - 200) <= 18)
			return true;
		else
			return false;
	case 3:     //目标牌
		if (1 <= (num - 300) <= 23)
			return true;
		else
			return false;
	default: return false;
	}
}