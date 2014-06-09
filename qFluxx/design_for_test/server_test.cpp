#include "clientMB.h"
using namespace std;


string convert(MsgType m);
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
	CardLib& lib = CardLib::getLib();

	int command;
	string s;
	int card,player,additional;
	vector<const Card*> cards;
	MsgType m;
	while(1)
	{
		cout<<"选择操作:\n1.CLIENT发消息\t2.CLIENT接消息\n3.退出测试\n";
		cin>>command;
		if(command == 1)
		{
			cout<<"选择发消息的类型：\n1.NACK\t2.REGISTER\t3.PLAY_I\n4.DROP_CARD_I\t5.DROP_KEEPER_I\t6.CHOOSE_KEEPER_I\n7.DROP_RULE_I\t8.CHOOSE_GOAL_I\t9.CHOOSE_PLAYER_I\n";
			cin>>command;
			switch(command)
			{
			case 1: client.createMsg(NACK);break;
			case 2: cout<<"输入玩家姓名： "; cin >>s; client.createMsg(REGISTER,s);break;
			case 3: 
				cards.clear();
				cout<<"输入卡牌在牌库的编号：\n";cin >>card;
				cards.push_back(lib.getCard(card));
				client.createMsg(PLAY_I,s);break;
			case 4:
				cards.clear();
				cout<<"输入卡牌在牌库的编号：\n";cin >>card;
				cards.push_back(lib.getCard(card));
				client.createMsg(DROP_CARD_I,s);break;
			case 5:
				cards.clear();
				cout<<"输入卡牌在牌库的编号：\n";cin >>card;
				cards.push_back(lib.getCard(card));
				client.createMsg(DROP_KEEPER_I,s);break;
			case 6:
				cards.clear();
				cout<<"输入卡牌在牌库的编号：\n";cin >>card;
				cards.push_back(lib.getCard(card));
				client.createMsg(CHOOSE_KEEPER_I,s);break;
			case 7:
				cards.clear();
				cout<<"输入卡牌在牌库的编号：\n";cin >>card;
				cards.push_back(lib.getCard(card));
				client.createMsg(DROP_RULE_I,s);break;
			case 8:
				cards.clear();
				cout<<"输入卡牌在牌库的编号：\n";cin >>card;
				cards.push_back(lib.getCard(card));
				client.createMsg(CHOOSE_GOAL_I,s);break;
			case 9:
				cout<<"输入玩家编号： "; cin>>player;
				client.createMsg(CHOOSE_PLAYER_I,player);break;
			default: break;
			}
		}
		else if(command == 2)
		{
			if(!client.getMsg(m)) return 1;
			else
			{
				cout<<"收到第一条消息类型为"<<convert(m)<<endl;
				switch(m)
				{
				case ADD_PLAYER: if(!client.getMsg(m,player,s)) return 1;
								 cout<<"收到第二条消息: "<<convert(m)<<"\t"<<player<<"\t"<<s<<endl;break;
				case GAME_START: 
				case RULE:
					if(!client.getMsg(m,cards)) return 1;
					cout<<"收到第二条消息: "<<convert(m)<<"\t";
					for(int i =0 ; i < cards.size(); i++)
						 cout<<cards[i]->getType()<<cards[i]->getNum()<<"\t";
					cout<<endl;break;
				case DROP_CARD_C:
				case DROP_KEEPER_C:
				case GAME_OVER:
				case DROP_RULE_C:
					if(!client.getMsg(m,additional)) return 1;
					cout<<"收到第二条消息: "<<convert(m)<<"\t"<<additional<<endl;break;
				case CARD_PLAYED:
				case CARD_DROPED:
				case CARD_UPDATE:
					if(!client.getMsg(m,cards,additional)) return 1;
					cout<<"收到第二条消息: "<<convert(m)<<"\t"<<additional<<endl;
					for(int i =0 ; i < cards.size(); i++)
						 cout<<cards[i]->getType()<<cards[i]->getNum()<<"\t";
					cout<<endl;break;
				case ROUND_BEGIN:
				case CARD_NUM:
					if(!client.getMsg(m,player,additional)) return 1;
					cout<<"收到第二条消息: "<<convert(m)<<"\t玩家为"<<player<<"\t额外信息为"<<additional<<endl;
				case KEEPER_UPDATE:
					if(!client.getMsg(m,cards,player,additional)) return 1;
					cout<<"收到第二条消息: "<<convert(m)<<"\t玩家为"<<player<<"\t额外信息为"<<additional<<endl;
					for(int i =0 ; i < cards.size(); i++)
						 cout<<cards[i]->getType()<<cards[i]->getNum()<<"\t";
					cout<<endl;break;
				}
			}
		}
		else if(command == 3) break;
		else cout<<"请重新输入编号\n";
	}

	/*//client发消息测试
	vector<const Card*> cards;
	client.createMsg(REGISTER,"Stella");
	cout << "已发送第一条消息:REGISTER" << endl;
	//client.createMsg(ACK);
	//cout << "已发送第二条消息:ACK" << endl;
	client.createMsg(NACK);
	cout << "已发送第三条消息:NACK" << endl;

	cards.push_back(lib.getCard(401));
	//if(client.createMsg(PLAY, cards))
	//cout << "已发送第四条消息:PLAY" << endl;
	//else cout << "第四条消息发送出错" << endl;

	cards.push_back(lib.getCard(402));
	if (client.createMsg(DROP_CARD_I, cards))
		cout << "已发送第五条消息:DROPCARD" << endl;
	else cout << "第五条消息发送出错" << endl;
	cards. clear();
	cards.push_back(lib.getCard(30));
	if (client.createMsg(DROP_KEEPER_I, cards))
		cout << "已发送第六条消息:DROPKEEPER" << endl;
	else cout << "第六条消息发送出错" << endl;
	*/
	//接消息
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

string convert(MsgType m)
{
	switch(m)
	{
	case NACK: return "NACK";
	case ADD_PLAYER: return "ADD_PLAYER";
	case GAME_START: return "GAME_START";
	case ROUND_BEGIN: return "ROUND_BEGIN";
	case CARD_UPDATE: return "CARD_UPDATE";
	case PLAY_C: return "PLAY_C";
	case CARD_NUM: return "CARD_NUM";
	case CARD_PLAYED: return "CARD_PLAYED";
	case CARD_DROPED: return "CARD_DROPED";
	case RULE: return "RULE";
	case KEEPER_UPDATE: return "KEEPER_UPDATE";
	case  DROP_CARD_C: return "DROP_CARD_C";
	case  DROP_KEEPER_C: return "DROP_KEEPER_C";
	case  GAME_OVER: return "GAME_OVER";
	//case 'E': m = CARD_STOLEN: return "NACK";
	case  CHOOSE_PLAYER_C: return "CHOOSE_PLAYER_C";
	case  CHOOSE_KEEPER_C: return "CHOOSE_KEEPER_C";
	case  EXCHANGE_KEEPER_C: return "EXCHANGE_KEEPER_C";
	case  DROP_RULE_C: return "DROP_RULE_C";
	case  CHOOSE_GOAL_C : return "CHOOSE_GOAL_C";
	}
}