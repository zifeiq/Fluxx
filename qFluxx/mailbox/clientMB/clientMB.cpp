#include"clientMB.h"
using namespace std;

ClientMB::ClientMB():
	_cards(CardLib::getLib())
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
//发送ACK、NACK消息
bool ClientMB::createMsg(MsgType m)  
{
	//解析参数并生成消息字符串
	string s;
	switch(m)
	{
		case ACK: s = "1"; break;
		case NACK: s = "2"; break;
		default: return false;
	}
	//发送消息
	if (!sendMsg(s))
		return false;
	else
		return true;
}
//发送REGISTER消息
bool ClientMB::createMsg(MsgType m, std::string name)
{
	//解析参数并生成消息字符串
	string s;
	if(m != REGISTER||name == "")//消息类型错误或未传入玩家名字
		return false;
	else 
		s = "0"+name;
	//发送消息
	if (!sendMsg(s))
		return false;
	else
		return true;
}
//发送PLAY,DROP_CARD_I,DROP_KEEPER_I消息
bool ClientMB::createMsg(MsgType m, vector<const Card*>relatedCards)
{
	//解析参数并生成消息字符串
	string s;
	int num;
	switch (m)
	{
		case PLAY:
		if (relatedCards.size() != 1)//未传入一张牌
			return false;
		else{
			s = "3" + card2Str(relatedCards[0]);							
			break;
		}
	case DROP_CARD_I:
		if (relatedCards.size() == 0)//未传入卡牌消息
			return false;
		else{
			s = "4";
			for (int i = 0; i < relatedCards.size(); i++)
				s += card2Str(relatedCards[i]);
			break;
		}
	case DROP_KEEPER_I:
		if (relatedCards.size() == 0)//未传入卡牌消息
			return false;
		else{
			s = "5";
			for (int i = 0; i < relatedCards.size(); i++)
			{
				//检测是否为有效的所有物牌编号
				if(relatedCards[i]->getType() == Card::KEEPER && 0<relatedCards[i]->getNum()<19) 
					s += card2Str(relatedCards[i]);
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
//期待接收ACK消息
bool ClientMB::getMsg(MsgType m) 
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{	
	case '2': return false;
	case '1': if(m == ACK) return true;
	default: 	
		//接收消息与期待不匹配
		createMsg(NACK); 
		return false;
	}

}
//期待接收ADD_PLAYER消息
bool ClientMB::getMsg(MsgType m, int& relatedPlayer, std::string& name)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '2': return false;
	case '0': 
		if(m == ADD_PLAYER){
			relatedPlayer = s[1];
			name = s.substr(2); 
			return true;
		}
	default: 
		//接收消息与期待不匹配
		createMsg(NACK); 
		return false;
	}
}
//期待接收GAME_START,RULE消息
bool ClientMB::getMsg(MsgType m, std::vector<const Card*>& relatedCards)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '2': return false;
	case '3': if(m == GAME_START) break;
			  else {
				  createMsg(NACK);
				  return false;
			  }
	case 'B': if(m == RULE) break;
	default:
		//接收消息与期待不匹配
		createMsg(NACK); 
		return false;
	}
	for (int i = 1; i < s.size(); i += 3)
		relatedCards.push_back(str2Card(s.substr(i, 3)));
	return true;
}
//期待接收ROUND_BEGIN,DROP_CARD_C,DROP_KEEPER_C,GAME_OVER消息
bool ClientMB::getMsg(MsgType m, int& relatedInfo)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '2': return false;
	case '4': if(m == ROUND_BEGIN)break;
			  else {
				  createMsg(NACK);
				  return false;
			  }
	case '6': if(m == DROP_CARD_C) break;
			else {
				  createMsg(NACK);
				  return false;
			  }
	case '7': if(m == DROP_KEEPER_C) break;
			  else{
				  createMsg(NACK);
				  return false;
			  }
	case 'A': if(m == GAME_OVER) break;
	default:  createMsg(NACK);
			  return false;
	}
	relatedInfo = atoi(s.substr(1).c_str());
	return true;
}
//期待接收DRAW,CARD_PLAYED,CARD_DROPED消息
bool ClientMB::getMsg(MsgType m, std::vector<const Card*>& relatedCards,int& relatedInfo)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '2': return false;
	case '5': if(m == DRAW) break;
			  else{
				  createMsg(NACK);
				  return false;
			  }
	case '9': if(m == CARD_PLAYED) break;
			  else{
				  createMsg(NACK);
				  return false;
			  }
	case 'D': if(m == CARD_DROPED) break;
	default: createMsg(NACK);
			  return false;
	}
	relatedInfo = atoi(s.substr(1,1).c_str());
	for(int i = 2; i < s.size(); i += 3)
		relatedCards.push_back(str2Card(s.substr(i, 3)));
	return true;
}
//期待接收CARD_NUM消息
bool ClientMB::getMsg(MsgType m, int& relatedPlayer, int& additional)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '2': return false;
	case '8': 
		if(m== CARD_NUM){
		relatedPlayer = s[1];
		additional = atoi(s.substr(2).c_str());
		return true;
		}
	default:createMsg(NACK);
			return false;
	}

}
//期待接收KEEPER_UPDATE消息
bool ClientMB::getMsg(MsgType m, std::vector<const Card*>& relatedCards,int& relatedPlayer, int& additional)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '2': if( m== NACK) return false;
	case 'C': 
		if(m== KEEPER_UPDATE){
		relatedPlayer = atoi(s.substr(1,1).c_str());
		additional = atoi(s.substr(2,1).c_str());
		for(int i = 3; i < s.size(); i += 3)
			relatedCards.push_back(str2Card(s.substr(i, 3)));
		return true;
		}
	default: createMsg(NACK);
			 return false;
	}
}

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

string ClientMB::card2Str(const Card* c)
{
	string s;
	stringstream ss;
	ss << c->getNum();
	switch (c->getType())
	{
	case Card::NEW_RULE: s = "1" + ss.str(); break;
	case Card::KEEPER: 
		s = "2" + (c->getNum() < 10 ? "0" + ss.str() : ss.str()); break;
	case Card::GOAL:
		s = "3" + (c->getNum() < 10 ? "0" + ss.str() : ss.str()); break;
	case Card::ACTION:
		s = "4" + (c->getNum() < 10 ? "0" + ss.str() : ss.str()); break;
	default: break;
	}
	return s;
}

const Card* ClientMB::str2Card(string s)
{
	switch (s[0])
	{
	case '0':  //基本规则牌
		return _cards.getCard(atoi(s.c_str()));
	case '1':  //新规则牌
		if (s[1] == '1')      //手牌上限
			return _cards.getCard(atoi(s.c_str()) - 108);
		else if (s[1] == '2')  //所有物上限
			return _cards.getCard(atoi(s.c_str()) - 117);
		else if (s[1] == '3')//抓牌数量
			return _cards.getCard(atoi(s.c_str()) - 124);
		else if (s[1] == '4')//出牌数量
			return _cards.getCard(atoi(s.c_str()) - 130);
		else                //其他新规则
			return _cards.getCard(atoi(s.c_str()) - 136);
	case '2':  //所有物牌
		return _cards.getCard(atoi(s.c_str()) - 178);
	case '3':  //目标牌
		return _cards.getCard(atoi(s.c_str()) - 260);
	case '4':  //行动牌
		return _cards.getCard(atoi(s.c_str()) - 337);
	default:   //不会进入此
		return _cards.getCard(0);
	}
}
