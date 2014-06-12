#include"clientMB.h"
using namespace std;

ClientMB::ClientMB():
	_cards(CardLib::getLib())
{
#ifdef WIN32
	//初始化winsock  
	WSADATA wsaD;
	WSAStartup(MAKEWORD(1, 1), &wsaD);
#endif
	//初始化客户端socket  
	clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

ClientMB::~ClientMB()
{
#ifdef WIN32
	closesocket(clientSock);
	WSACleanup();
#else
	close(clientSock);
#endif
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
	char buf[BUFF_MAX];
	int index = 0;
	while (1)
	{
		string temp = s.substr(index, BUFF_MAX - 1);
		memset(buf, 0, sizeof(buf));
#ifdef WIN32
		strcpy_s(buf, temp.c_str());
#else
		strcpy(buf,temp.c_str());
#endif
		if (send(clientSock, buf, strlen(buf) + 1, 0) == -1) //发送出错
			return false; 
		else
		{
			index += BUFF_MAX - 1;
			if (index >= s.size())  //已发完字符串所有内容
				return true;
		}
	}
}

string ClientMB::recvMsg()
{
	string s;
	char buf[BUFF_MAX];
	memset(buf, 0, sizeof(buf));
	if (recv(clientSock, buf, BUFF_MAX, 0) != -1)
		s = buf;
	return s;
}
//发送NACK消息
bool ClientMB::createMsg(MsgType m)  
{
	//解析参数并生成消息字符串
	string s;
	if(m == NACK) s = "0";
	else return false;
	/*switch(m)
	{
		case ACK: s = "1"; break;
		case NACK: s = "2"; break;
		default: return false;
	}*/
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
		s = "1"+name;
	//发送消息
	if (!sendMsg(s))
		return false;
	else
		return true;
}
//发送PLAY_I,DROP_CARD_I,DROP_KEEPER_I,CHOOSE_KEEPER_I,DROP_RULE_I,CHOOSE_GOAL_I消息
bool ClientMB::createMsg(MsgType m, vector<const Card*>relatedCards)
{
	//解析参数并生成消息字符串
	string s;
	switch (m)
	{
	case PLAY_I:
		if (relatedCards.size() != 1)//未传入一张牌
			return false;
		else{
			s = "2" + card2Str(relatedCards[0]);							
			break;
		}
	case DROP_CARD_I:
		if (relatedCards.size() == 0)//未传入卡牌消息
			return false;
		else{
			s = "3";
			for (int i = 0; i < relatedCards.size(); i++)
				s += card2Str(relatedCards[i]);
			break;
		}
	case DROP_KEEPER_I:
		if (relatedCards.size() == 0)//未传入卡牌消息
			return false;
		else{
			s = "4";
			for (int i = 0; i < relatedCards.size(); i++)
			{
				//检测是否为有效的所有物牌编号
				if(relatedCards[i]->getType() == Card::KEEPER && 0<relatedCards[i]->getNum()&&relatedCards[i]->getNum()<19) 
					s += card2Str(relatedCards[i]);
				else
					return false;
			}
			break;
		}
	case CHOOSE_KEEPER_I:
		if (relatedCards.size() != 1 &&relatedCards.size() != 2)//未传入有效卡牌消息
			return false;
		else{
			s = "5";
			for (int i = 0; i < relatedCards.size(); i++)
			{
				//检测是否为有效的所有物牌
				if(relatedCards[i]->getType() == Card::KEEPER && 0<relatedCards[i]->getNum()&&relatedCards[i]->getNum()<19) 
					s += card2Str(relatedCards[i]);
				else
					return false;
			}
			break;
		}
	case DROP_RULE_I:
		if(relatedCards.size()==0)//未传入有效卡牌消息
			return false;
		else{
			s = "6";
			for(int i = 0; i < relatedCards.size();i++)
			{
				//检测是否为有效的规则牌
				if(relatedCards[i]->getType() == Card::NEW_RULE)
					s += card2Str(relatedCards[i]);
				else 
					return false;
			}
			break;
		}
	case CHOOSE_GOAL_I:
		if(relatedCards.size()!=1)//未传入有效卡牌消息
			return false;
		else{
			s = "7";
			//检测是否为有效的目标牌
			if(relatedCards[0]->getType() == Card::GOAL)
				s += card2Str(relatedCards[0]);
			else 
				return false;
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

//发送CHOOSE_PLAYER_I消息
bool ClientMB::createMsg(MsgType m, int relatedPlayer)
{
	string s;
	stringstream ss;
	if(m != CHOOSE_PLAYER_I) //消息类型不匹配
		return false;
	else{
		ss<<relatedPlayer;
		s = "8"+ss.str();
		//发送消息
		if (!sendMsg(s))
			return false;
		else
			return true;
	}
}

//接收所有消息
bool ClientMB::getMsg(MsgType& m) 
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '0': m = NACK; return false;
	case '1': m = ADD_PLAYER; break;
	case '2': m = GAME_START; break;
	case '3': m = ROUND_BEGIN; break;
	case '4': m = CARD_UPDATE; break;
	case '5': m = PLAY_C; break;
	case '6': m = CARD_NUM; break;
	case '7': m = CARD_PLAYED; break;
	case '8': m = CARD_DROPED; break;
	case '9': m = RULE; break;
	case 'A': m = KEEPER_UPDATE; break;
	case 'B': m = DROP_CARD_C; break;
	case 'C': m = DROP_KEEPER_C; break;
	case 'D': m = GAME_OVER; break;
	//case 'E': m = CARD_STOLEN; break;
	case 'F':m = CHOOSE_PLAYER_C; break;
	case 'G': m = CHOOSE_KEEPER_C; break;
	case 'H': m = EXCHANGE_KEEPER_C; break;
	case 'I': m = DROP_RULE_C; break;
	case 'J': m = CHOOSE_GOAL_C ; break;
	default: return false;
	}
	return true;
}
//期待接收ADD_PLAYER消息
bool ClientMB::getMsg(MsgType m, int& relatedPlayer, std::string& name)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '0': return false;
	case '1': 
		if(m == ADD_PLAYER){
			relatedPlayer = atoi(s.substr(1,1).c_str());
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
	relatedCards.clear();
	//解析消息
	switch (s[0])
	{
	case '0': return false;
	case '2': if(m == GAME_START) break;
			  else {
				  createMsg(NACK);
				  return false;
			  }
	case '9': if(m == RULE) break;
			  else {
				  createMsg(NACK);
				  return false;
			  }
	//case 'E': if(m==CARD_STOLEN) break;
	default:
		//接收消息与期待不匹配
		createMsg(NACK); 
		return false;
	}
	for (int i = 1; i < s.size(); i += 3)
		relatedCards.push_back(str2Card(s.substr(i, 3)));
	return true;
}
//期待接收DROP_CARD_C,DROP_KEEPER_C,GAME_OVER,DROP_RULE_C消息
bool ClientMB::getMsg(MsgType m, int& relatedInfo)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '0': return false;
	case 'I': if(m == DROP_RULE_C)break;
			  else {
				  createMsg(NACK);
				  return false;
			  }
	case 'B': if(m == DROP_CARD_C) break;
			else {
				  createMsg(NACK);
				  return false;
			  }
	case 'C': if(m == DROP_KEEPER_C) break;
			  else{
				  createMsg(NACK);
				  return false;
			  }
	case 'D': if(m == GAME_OVER) break;
	default:  createMsg(NACK);
			  return false;
	}
	relatedInfo = atoi(s.substr(1).c_str());
	return true;
}
//期待接收CARD_PLAYED,CARD_DROPED,CARD_UPDATE消息
bool ClientMB::getMsg(MsgType m, std::vector<const Card*>& relatedCards,int& relatedInfo)
{
	//等待接受消息
	string s = recvMsg();
	relatedCards.clear();
	//解析消息
	switch (s[0])
	{
	case '0': return false;
	case '4': if(m == CARD_UPDATE) break;
			  else{
				  createMsg(NACK);
				  return false;
			  }
	case '7': if(m == CARD_PLAYED) break;
			  else{
				  createMsg(NACK);
				  return false;
			  }
	case '8': if(m == CARD_DROPED) break;
	default: createMsg(NACK);
			  return false;
	}
	relatedInfo = atoi(s.substr(1,1).c_str());
	for(int i = 2; i < s.size(); i += 3)
		relatedCards.push_back(str2Card(s.substr(i, 3)));
	return true;
}
//期待接收ROUND_BEGIN，CARD_NUM消息
bool ClientMB::getMsg(MsgType m, int& relatedPlayer, int& additional)
{
	//等待接受消息
	string s = recvMsg();
	//解析消息
	switch (s[0])
	{
	case '0': return false;
	case '3': if(m == ROUND_BEGIN) break;
			  else{
				  createMsg(NACK);
				  return false;
			  }
	case '6': if(m== CARD_NUM) break;
	default:createMsg(NACK);
			return false;
	}
	relatedPlayer = atoi(s.substr(1,1).c_str());
	additional = atoi(s.substr(2).c_str());
	return true;
}
//期待接收KEEPER_UPDATE消息
bool ClientMB::getMsg(MsgType m, std::vector<const Card*>& relatedCards,int& relatedPlayer, int& additional)
{
	//等待接受消息
	string s = recvMsg();
	relatedCards.clear();
	//解析消息
	switch (s[0])
	{
	case '0': if( m== NACK) return false;
	case 'A': 
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
