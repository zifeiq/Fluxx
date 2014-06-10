#include "serverMB.h"
using namespace std;

ServerMB::ServerMB():
_clientNum(0),
_cards(CardLib::getLib())
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
	listen(serverSock, CLIENT_MAX);   //最多同时建立4个连接
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
	char buf[BUFF_MAX];
	unsigned int index = 0;
	while (1)
	{
		string temp = s.substr(index, BUFF_MAX - 1);
		memset(buf, 0, sizeof(buf));
		strcpy_s(buf, temp.c_str());
		if (send(clientSock[num], buf, strlen(buf) + 1, 0) == -1) //发送出错
			return false;
		else
		{
			index += BUFF_MAX - 1;
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
	char buf[BUFF_MAX];
	memset(buf, 0, sizeof(buf));
	int recvLen = 0;
	if (recv(clientSock[num], buf, BUFF_MAX, 0) != -1)
		s = buf;
	return s;
}

bool ServerMB::acceptNewClient()
{
	if (_clientNum == CLIENT_MAX)
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
//接收ACK或NACK消息
/*
bool ServerMB::getMsg(int playerNum, MsgType& m)
{
	//等待接受消息
	string s = recvMsg(playerNum);
	//解析消息
	switch (s[0])
	{
	case '1': m = ACK; break;
	case '2': m = NACK; break;
	default: return false;
	}
	return true;
}*/

//期待接收REGISTER消息
bool ServerMB::getMsg(int playerNum, MsgType m, string& name)
{
	//等待接受消息
	string s = recvMsg(playerNum);
	//解析消息
	switch (s[0])
	{
	case '0': return false;
	case '1':
		if (m == REGISTER){
			name = s.substr(1);
			return true;
		}
	default:
		//接收消息与期待不匹配
		createMsg(playerNum,NACK);
		return false;
	}
}
//期待接收PLAY_I,DROP_CARD_I,DROP_KEEPER_I,CHOOSE_KEEPER_I,DROP_RULE_I,CHOOSE_GOAL_I消息
bool ServerMB::getMsg(int playerNum, MsgType m, vector<const Card*>& relatedCards)
{
	//等待接受消息
	string s = recvMsg(playerNum);
	//解析消息
	relatedCards.clear();
	switch (s[0])
	{
	case '0': return false;
	case '2': if (m == PLAY_I) break;
			  else {
				  createMsg(playerNum,NACK);
				  return false;
			  }
	case '3': if (m == DROP_CARD_I) break;
			  else {
				  createMsg(playerNum, NACK);
				  return false;
			  }
	case '4': if (m == DROP_KEEPER_I) break;
			  else {
				  createMsg(playerNum, NACK);
				  return false;
			  }
	case '5':if (m == CHOOSE_KEEPER_I) break;
			 else {
				 createMsg(playerNum, NACK);
				 return false;
			 }
	case '6':if (m == DROP_RULE_I) break;
			 else {
				 createMsg(playerNum, NACK);
				 return false;
			 }
	case '7':if (m == CHOOSE_GOAL_I) break;
	default:
		//接收消息与期待不匹配
		createMsg(playerNum, NACK);
		return false;
	}
	for (unsigned int i = 1; i < s.size(); i += 3)
		relatedCards.push_back(str2Card(s.substr(i, 3)));
	return true;
}
//期待接收CHOOSE_PLAYER_I消息
bool ServerMB::getMsg(int playerNum, MsgType m, int& relatedPlayer)
{
	//等待接受消息
	string s = recvMsg(playerNum);
	//解析消息
	switch (s[0])
	{
	case '0': return false;
	case '8':
		if (m == CHOOSE_PLAYER_I){
			relatedPlayer = atoi(s.substr(1).c_str());
			return true;
		}
	default:
		//接收消息与期待不匹配
		createMsg(playerNum, NACK);
		return false;
	}
}

//发送所有消息
bool ServerMB::createMsg(int playerNum, MsgType m) 
{
	//解析参数并生成消息字符串
	string s;
	switch (m)
	{
	case NACK: s = "0"; break;
	case ADD_PLAYER: s = "1"; break;
	case GAME_START: s = "2"; break;
	case ROUND_BEGIN: s = "3"; break;
	case CARD_UPDATE: s = "4"; break;
	case PLAY_C: s = "5"; break;
	case CARD_NUM: s = "6"; break;
	case CARD_PLAYED: s = "7"; break;
	case CARD_DROPED: s = "8"; break;
	case RULE: s = "9"; break;
	case KEEPER_UPDATE: s = "A"; break;
	case DROP_CARD_C: s = "B"; break;
	case DROP_KEEPER_C: s = "C"; break;
	case GAME_OVER: s = "D"; break;
	//case CARD_STOLEN: s = "E"; break;
	case CHOOSE_PLAYER_C:s = "F"; break;
	case CHOOSE_KEEPER_C: s = "G"; break;
	case EXCHANGE_KEEPER_C: s = "H"; break;
	case DROP_RULE_C: s = "I"; break;
	case CHOOSE_GOAL_C: s = "J"; break;
	default: return false;
	}
	//发送消息
	if (!sendMsg(playerNum, s))
		return false;
	else
		return true;
}
//发送ADD_PLAYER消息
bool ServerMB::createMsg(int playerNum, MsgType m, int relatedPlayer, std::string name)
{
	//解析参数并生成消息字符串
	string s;
	stringstream ss;
	if (m != ADD_PLAYER||name == "" || relatedPlayer < 0 || relatedPlayer>3) //消息类型错误，或未传入正确名字或编号
			return false;
	else
	{
		ss << relatedPlayer;
		s = "1" + ss.str() + name;
	}
	//发送消息
	if (!sendMsg(playerNum, s))
		return false;
	else
		return true;
}
//发送GAME_START,RULE消息
bool ServerMB::createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards)
{
	//解析参数并生成消息字符串
	string s;
	stringstream ss;
	switch (m)
	{
	case GAME_START:
		if (relatedCards.size() != 3) //未传入初始三张手牌信息
			return false;
		else{
			s = "2";
			break;
		}
	case RULE:
		if (relatedCards.size() <2)//未传入当前规则的有效信息
			return false;
		for (unsigned int i = 0; i < relatedCards.size(); i++)  //检测是否传入的均为规则牌和目标牌
		{
			Card::Type type = relatedCards[i]->getType();
			if (type != Card::BASIC_RULE&&type != Card::NEW_RULE&&type != Card::GOAL)
				return false;
		}
		s = "9";
		break;
	/*case CARD_STOLEN:
		if (relatedCards.size() != 1) return false;
		s = "E";
		break;
	*/
	default: return false;
	}
	for (unsigned int i = 0; i < relatedCards.size(); i++)
		s += card2Str(relatedCards[i]);
	//发送消息
	if (!sendMsg(playerNum, s))
		return false;
	else
		return true;
}
//发送DROP_CARD_C,DROP_KEEPER_C,GAME_OVER,DROP_RULE_C消息
bool ServerMB::createMsg(int playerNum, MsgType m, int relatedInfo) 
{
	//解析参数并生成消息字符串
	string s;
	stringstream ss;
	switch (m)
	{
	case DROP_CARD_C:
		if (relatedInfo < 0)  //未传入弃牌张数的有效信息
			return false;
		else{
			s = "B";
			break;
		}
	case DROP_KEEPER_C:
		if (relatedInfo<= 0)  //未传入弃所有物张数的有效信息
			return false;
		else{
			s = "C";
			break;
		}
	case GAME_OVER:
		if (relatedInfo<0 || relatedInfo>3) //玩家编号异常
			return false;
		else{
			s = "D";
			break;
		}
	case DROP_RULE_C:
		if (relatedInfo < 1) return false;
		else{
			s = "I";
			break;
		}
	default: return false;
	}
	ss << relatedInfo;
	s += ss.str();
	//发送消息
	if (!sendMsg(playerNum, s))
		return false;
	else
		return true;
}
//发送CARD_PLAYED,CARD_DROPED,CARD_UPDATE消息
bool ServerMB::createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards, int relatedInfo) 
{
	//解析参数并生成消息字符串
	string s;
	stringstream ss;
	switch (m)
	{
	case CARD_PLAYED:
		if (relatedInfo<0 || relatedInfo>3 || relatedCards.size() != 1) //玩家编号异常或未传入出牌的有效信息
			return false;
		else{
			s = "7";
			break;
		}
	case CARD_DROPED:
		if (relatedInfo<0 || relatedInfo>3 || relatedCards.size() == 0) //玩家编号异常或未传入出牌的有效信息
			return false;
		else{
			s = "8";
			break;
		}
	case CARD_UPDATE:
		if (relatedInfo != 0 && relatedInfo != 1 && relatedInfo != 2) //未传入有效标志位
			return false;
		else{
			s = "4";
			break;
		}
	default: return false;
	}
	ss << relatedInfo;
	s += ss.str();
	for (unsigned int i = 0; i < relatedCards.size(); i++)
		s += card2Str(relatedCards[i]);
	//发送消息
	if (!sendMsg(playerNum, s))
		return false;
	else
		return true;
}
//发送ROUND_BEGIN，CARD_NUM消息
bool ServerMB::createMsg(int playerNum, MsgType m, int relatedPlayer, int additional) 
{
	//解析参数并生成消息字符串
	string s;
	stringstream ss;
	switch (m)
	{
	case ROUND_BEGIN:
		if (relatedPlayer<0 || relatedPlayer>3 || (additional != 0 && additional != 1)) //玩家编号异常或未传入有效标志位
			return false;
		else {
			s = "3";
			break;
		}
	case CARD_NUM:
		if (additional < 0 || relatedPlayer<0 || relatedPlayer>3) //未传入有效手牌张数或玩家编号异常
			return false;
		else {
			s = "6";
			break;
		}
	default: return false;
	}
	ss << relatedPlayer << additional;
	s += ss.str();
	//发送消息
	if (!sendMsg(playerNum, s))
		return false;
	else
		return true;
}
//发送KEEPER_UPDATE消息
bool ServerMB::createMsg(int playerNum, MsgType m, std::vector<const Card*> relatedCards, int relatedPlayer, int additional) 
{
	//解析参数并生成消息字符串
	string s;
	stringstream ss;
	if (m != KEEPER_UPDATE || relatedPlayer < 0 || relatedPlayer>3 || additional<0 || additional>2)  //消息类型错误，或玩家编号异常或未传入额外信息
		return false;
	ss << relatedPlayer<< additional;
	s = "A" + ss.str();
	for (unsigned int i = 0; i < relatedCards.size(); i++)
		s += card2Str(relatedCards[i]);
	//发送消息
	if (!sendMsg(playerNum, s))
		return false;
	else
		return true;
}
/*
bool ServerMB::createMsg(int playerNum, MsgType m, vector<Card*> relatedCards, int relatedPlayer, int additional, string name)
{
	//解析参数并生成消息字符串
	string s;
	stringstream ss;
	switch (m)
	{
	case ADDPLAYER:
		if (name == "" || relatedPlayer < 0 || relatedPlayer>3) //未传入正确名字或编号
			return false;
		else
		{
			ss << relatedPlayer;
			s = "00" + ss.str() + name;
			break;
		}
	case ACK:
		s = "01";
		break;
	case NACK:
		s = "02";
		break;
	case GAMESTART:
		s = "03";
		if (relatedCards.size() != 3) //未传入初始三张手牌信息
			return false;
		for (int i = 0; i < 3; i++)
			s += card2Str(relatedCards[i]);
		break;
	case ROUNDBEGIN:
		if (relatedPlayer < 0 || relatedPlayer >3) //未传入正确玩家编号
			return false;
		else
		{
			ss << relatedPlayer;
			s = "04" + ss.str();
			break;
		}
	case DRAW:
		s = "05";
		if (additional != 0 && additional != 1)  //未传入是否随机开场的有效信息 
			return false;
		else{
			ss << additional;
			s += ss.str();
		}

		for (int i = 0; i < relatedCards.size(); i++)
			s += card2Str(relatedCards[i]);
		break;
	case DROPCARD:
		if (additional < 0)  //未传入弃牌张数的有效信息
			return false;
		else{
			ss << additional;
			s = "06" + ss.str();
			break;
		}
	case DROPKEEPER:
		if (additional <= 0)  //未传入弃所有物张数的有效信息
			return false;
		else{
			ss << additional;
			s = "07" + ss.str();
			break;
		}
	case CARDNUM:
		if (additional < 0 || relatedPlayer<0 || relatedPlayer>3) //未传入有效手牌张数或玩家编号异常
			return false;
		else{
			ss << relatedPlayer << additional;
			s = "08" + ss.str();
			break;
		}
	case CARDPLAYING:
		if (relatedPlayer<0 || relatedPlayer>3 || relatedCards.size() != 1) //玩家编号异常或未传入出牌的有效信息
			return false;
		else{
			ss << relatedPlayer;
			s = "09" + ss.str() + card2Str(relatedCards[0]);
			break;
		}
	case GAMEOVER:
		if (relatedPlayer<0 || relatedPlayer>3) //玩家编号异常
			return false;
		else{
			ss << relatedPlayer;
			s = "10" + ss.str();
			break;
		}
	case RULE:
		if (relatedCards.size() == 0)//未传入当前规则的有效信息
			return false;
		for (int i = 0; i < relatedCards.size(); i++)  //检测是否传入的均为规则牌和目标牌
		{
			Card::Type type = relatedCards[i]->getType();
			if (type != Card::BASIC_RULE&&type != Card::NEW_RULE&&type != Card::GOAL)
				return false;
		}

		s = "11";
		for (int i = 0; i < relatedCards.size(); i++)
			s += card2Str(relatedCards[i]);
		break;
	case KEEPERUPDATE:
		if (relatedPlayer < 0 || relatedPlayer>3 || additional<0 || additional>2)  //玩家编号异常或未传入额外信息
			return false;
		ss << additional;
		s = "12" + ss.str();
		for (int i = 0; i < relatedCards.size(); i++)
			s += card2Str(relatedCards[i]);
		break;
	default:return false;
	}


}*/

//由server收到的字符串信息得到对应的卡牌信息(s的正确性由MB和client保证，此处不再检测）
const Card* ServerMB::str2Card(string s)
{
	switch (s[0])
	{
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

string ServerMB::card2Str(const Card* c)
{
	string s;
	stringstream ss;
	ss << c->getNum();
	switch (c->getType())
	{
	case Card::BASIC_RULE: s = "00" + ss.str(); break;
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