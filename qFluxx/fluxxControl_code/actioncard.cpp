#include "fluxxcontrol.h"
#include<time.h>

void fluxxControl::actioncard(Card& act_card)
{
	std::vector<const Card*> player_cards;	//临时存放玩家手牌或所有物或弃牌堆里有用的牌
	//std::vector<const Card*> player_cards_tmp;	//作为中转用
	//std::vector<const Card*> player_cards_tmp2;	//作为中转用
	std::vector<const Card*>::const_iterator ii = droppeddeck.begin();	//case6中用到，从弃牌堆中删除某张牌
	std::vector<int> cnt;	//临时记录玩家所有物数量或其他牌数量的计数器
	int tmp;
	const Card* tmp_card;

	switch(act_card.getNum())
	{
	case 1://抓2用2

		//将手牌放到一边
		std::vector<const Card*>& player_cards_tmp=presentPlayer.gethand();
		player_cards=player_cards_tmp;
		//清空原手牌
		player_cards_tmp.clear();	
		//告诉玩家手牌现在为空
		msgbufMsgtype=CARD_UPDATE;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer.gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		//抓两张牌，包含了广播
		dealCard(2);
		//出两张牌
		for(int i=0;i<2;i++)
		{
			msgbufMsgtype=PLAY_C;
			msgBox.createMsg(clientNum,msgbufMsgtype);
			msgbufMsgtype=PLAY_I;
			msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
			playCard(*msgbufCards[0]);	//包含了广播
		}
		//恢复原手牌
		player_cards_tmp=player_cards;
		//告诉玩家手牌现在恢复
		msgbufMsgtype=CARD_UPDATE;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer.gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		break;

	case 2://弃牌重抓

		//弃光手牌
		std::vector<const Card*>& player_cards_tmp=presentPlayer.gethand();
		tmp=player_cards_tmp.size();
		player_cards_tmp.clear();
		//告诉玩家手牌现在为空
		msgbufMsgtype=CARD_UPDATE;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer.gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		//抓等量的牌，包含告诉玩家抓了哪些牌
		dealCard(tmp);
		break;

	case 3://重置规则

		//摸牌数
		if(rule.getdraw()!=1)
		{
			rule.setdraw(1);
			droppeddeck.push_back(&rule.getdrawrule());
			for(int i=0;i<cards.getCardNum();i++)
				if(cards.getCard(i)->getType==Card::BASIC_RULE&&cards.getCard(i)->getNum()==0)
				{
					rule.setdrawrule(*cards.getCard(i));
					break;
				}
		}
		//出牌数
		if(rule.getplay()!=1)
		{
			rule.setplay(1);
			droppeddeck.push_back(&rule.getplayrule());
			for(int i=0;i<cards.getCardNum();i++)
				if(cards.getCard(i)->getType==Card::BASIC_RULE&&cards.getCard(i)->getNum()==1)
				{
					rule.setplayrule(*cards.getCard(i));
					break;
				}
		}
		//手牌上限
		if(rule.gethandlimitation()!=-1)
		{
			rule.sethandlimitation(-1);
			droppeddeck.push_back(&rule.getChandlimit());
		}
		//所有物上限
		if(rule.getkeeperlimitation()!=-1)
		{
			rule.setkeeperlimitation(-1);
			droppeddeck.push_back(&rule.getCkeeperlimit());
		}
		//出牌方向
		if(rule.isorderreverse)
		{
			rule.setorderreverse(false);
			droppeddeck.push_back(&rule.getCorderreverse());
		}
		//双重目标
		if(rule.isdoublegoals())
		{
			rule.setdoublegoals(false);
			droppeddeck.push_back(&rule.getCdoublegoals());
		}
		//富人奖励
		if(rule.isrichBonus())
		{
			rule.setrichbonus(false);
			droppeddeck.push_back(&rule.getCrichbonus());
		}
		//穷人奖励
		if(rule.ispoorBonus())
		{
			rule.setpoorbonus(false);
			droppeddeck.push_back(&rule.getCpoorbonus());
		}
		//通货膨胀
		if(rule.isinflation())
		{
			rule.setinflation(false);
			droppeddeck.push_back(&rule.getCinflation());
		}
		//无手牌奖励
		if(rule.isnohandbonus())
		{
			rule.setnohandbonus(false);
			droppeddeck.push_back(&rule.getCnohandbonus());
		}
		//随机开场
		if(rule.israndomstart())
		{
			rule.setrandomstart(false);
			droppeddeck.push_back(&rule.getCrandomstart());
		}
		//广播规则更新
		for(int i=0;i<players.size();i++)
		{
			msgbufMsgtype=RULE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=RULE;
			msgbufCards.clear();
			_updateRules();
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
		}
		break;

	case 4://抓3用2

		//将手牌放到一边
		std::vector<const Card*>& player_cards_tmp=presentPlayer.gethand();
		player_cards=player_cards_tmp;
		//清空原手牌
		player_cards_tmp.clear();	
		//告诉玩家手牌现在为空
		msgbufMsgtype=CARD_UPDATE;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer.gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		//抓三张牌，包含广播
		dealCard(3);
		//出两张牌
		for(int i=0;i<2;i++)
		{
			msgbufMsgtype=PLAY_C;
			msgBox.createMsg(clientNum,msgbufMsgtype);
			msgbufMsgtype=PLAY_I;
			msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
			playCard(*msgbufCards[0]);	//包含广播
		}
		//弃一张牌，包含广播
		dropCard(1);
		//恢复原手牌
		player_cards_tmp=player_cards;
		//告诉玩家手牌现在恢复
		msgbufMsgtype=CARD_UPDATE;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer.gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		break;

	case 5://中彩

		//抓三张牌，包含广播
		dealCard(3);
		break;

	case 6://再来一次

		//检测弃牌堆里是否有行动卡或者新规则卡
		for(int i=0;i<droppeddeck.size();i++)
			if(droppeddeck[i]->getType()==Card::ACTION||droppeddeck[i]->getType()==Card::NEW_RULE)
				player_cards.push_back(droppeddeck[i]);
		if(player_cards.size()==0)
			break;	//弃牌堆里没有行动卡或者新规则卡，该行动卡无效
		//随机抽一张，偷偷加入其手牌中，并从弃牌堆中删掉，然后偷偷给他打出
		srand(time(NULL));
		tmp=rand()%(player_cards.size());
		presentPlayer.addHand(*player_cards[tmp]);
		
		for(; ii != droppeddeck.end(); ii++) 
			if ((**ii) == *player_cards[tmp]) 
			{
				droppeddeck.erase(ii);
				break;
			}

		playCard(*player_cards[tmp]);
		break;

	case 7://简化一下

		cnt.push_back(0);	//存放当前新规则卡的数目
		if(rule.getdraw()!=1)
			cnt[0]++;
		if(rule.getplay()!=1)
			cnt[0]++;
		if(rule.gethandlimitation()!=-1)
			cnt[0]++;
		if(rule.getkeeperlimitation()!=-1)
			cnt[0]++;
		if(rule.isorderreverse)
			cnt[0]++;
		if(rule.isdoublegoals())
			cnt[0]++;
		if(rule.isrichBonus())
			cnt[0]++;
		if(rule.ispoorBonus())
			cnt[0]++;
		if(rule.isinflation())
			cnt[0]++;
		if(rule.isnohandbonus())
			cnt[0]++;
		if(rule.israndomstart())
			cnt[0]++;
		//tmp存放需要弃掉的规则卡数目
		if(cnt[0]%2==1)
			tmp=cnt[0]/2+1;
		else
			tmp=cnt[0]/2;
		//发送和接收弃规则卡信息
		msgbufMsgtype=DROP_RULE_C;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=DROP_RULE_C;
		msgbufAdditional=tmp;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufAdditional);
		msgbufMsgtype=DROP_RULE_I;
		msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards);
		//把玩家选择的规则卡弃掉
		for(int i=0;i<msgbufCards.size();i++)
		{
			if(msgbufCards[i]->getType==Card::NEW_RULE)
			{
				if(msgbufCards[i]->getNum()/10==1)	//手牌上限
				{	
					if(rule.gethandlimitation()!=-1)
						droppeddeck.push_back(&rule.getChandlimit());
					rule.sethandlimitation(-1);
				}
				if(msgbufCards[i]->getNum()/10==2)	//所有物上限
				{
					if(rule.getkeeperlimitation()!=-1)
						droppeddeck.push_back(&rule.getCkeeperlimit());
					rule.setkeeperlimitation(-1);
				}
				if(msgbufCards[i]->getNum()/10==3)	//抓牌数
				{
					if(rule.getdraw()!=1)
						droppeddeck.push_back(&rule.getdrawrule());
					rule.setplay(1);
					for(int j=0;j<cards.getCardNum();j++)
						if(cards.getCard(j)->getType==Card::BASIC_RULE&&cards.getCard(j)->getNum()==0)
						{
							rule.setdrawrule(*cards.getCard(j));
							break;
						}
				}
				if(msgbufCards[i]->getNum()/10==4)	//出牌数
				{
					if(rule.getplay()!=1)
						droppeddeck.push_back(&rule.getplayrule());
					rule.setplay(1);
					for(int j=0;j<cards.getCardNum();j++)
						if(cards.getCard(j)->getType==Card::BASIC_RULE&&cards.getCard(j)->getNum()==1)
						{
							rule.setplayrule(*cards.getCard(j));
							break;
						}
				}
				if(msgbufCards[i]->getNum()/10==5)	//特殊规则
				{
					if(msgbufCards[i]->getNum()%10==1)//出光手牌
					{
						if(rule.getplay()!=1)
							droppeddeck.push_back(&rule.getplayrule());
						rule.setplay(1);
						for(int j=0;j<cards.getCardNum();j++)
							if(cards.getCard(j)->getType==Card::BASIC_RULE&&cards.getCard(j)->getNum()==1)
							{
								rule.setplayrule(*cards.getCard(j));
								break;
							}
					}
					if(msgbufCards[i]->getNum()%10==2)//富人奖励
					{
						rule.setrichbonus(false);
						droppeddeck.push_back(&rule.getCrichbonus());
					}
					if(msgbufCards[i]->getNum()%10==3)//穷人奖励
					{
						rule.setpoorbonus(false);
						droppeddeck.push_back(&rule.getCpoorbonus());
					}
					if(msgbufCards[i]->getNum()%10==4)//通货膨胀
					{
						rule.setinflation(false);
						droppeddeck.push_back(&rule.getCinflation());
					}
					if(msgbufCards[i]->getNum()%10==5)//顺序翻转
					{
						rule.setorderreverse(false);
						droppeddeck.push_back(&rule.getCorderreverse());
					}
					if(msgbufCards[i]->getNum()%10==6)//无手牌奖励
					{
						rule.setnohandbonus(false);
						droppeddeck.push_back(&rule.getCnohandbonus());
					}
					if(msgbufCards[i]->getNum()%10==7)//随机开场
					{
						rule.setrandomstart(false);
						droppeddeck.push_back(&rule.getCrandomstart());
					}
					if(msgbufCards[i]->getNum()%10==8)//双重目标
					{
						rule.setdoublegoals(false);
						droppeddeck.push_back(&rule.getCdoublegoals());
					}
				}//特殊规则结束
			}//本张规则卡遍历所有类型的规则结束
		}//所有的规则卡弃牌结束
		//广播规则更新
		for(int i=0;i<players.size();i++)
		{
			msgbufMsgtype=RULE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=RULE;
			msgbufCards.clear();
			_updateRules();
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
		}
		break;


	case 8://交换所有物

		//通知玩家选择所有物，并接收玩家的选择
		msgbufMsgtype=EXCHANGE_KEEPER_C;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_KEEPER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
		//在别人那里拿走所有物和增添所有物
		tmp=-1;
		for(int i=0;i<players.size();i++)
		{
			if(i==clientNum)
				continue;
			for(int j=0;j<players[i].getkeeper().size();j++)
				if(players[i].getkeeper()[j]==msgbufCards[0])		//找到谁有这张所有物
				{
					players[i].removeKeeper(*msgbufCards[0]);
					players[i].addKeeper(*msgbufCards[1]);
					tmp=i;
					break;
				}
			if(tmp!=-1)
				break;
		}
		//在自己这里拿走所有物和增添所有物
		presentPlayer.removeKeeper(*msgbufCards[1]);
		presentPlayer.addKeeper(*msgbufCards[0]);
		//告诉每个玩家这两个人的所有物信息
		for(int i=0;i<players.size();i++)
		{
			//另一个玩家的
			msgbufMsgtype=KEEPER_UPDATE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[tmp].getkeeper();
			msgbufAdditional=0;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,tmp,msgbufAdditional);
			//当前玩家的
			msgbufMsgtype=KEEPER_UPDATE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[clientNum].getkeeper();
			msgbufAdditional=0;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,clientNum,msgbufAdditional);
		}
		break;

	case 9://再动一轮

		if(rule.isorderreverse())
			clientNum=(clientNum+1)%4;
		else
			clientNum=(clientNum-1)%4;
		break;

	case 10://偷窃

		//玩家选所有物
		msgbufMsgtype=CHOOSE_KEEPER_C;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_KEEPER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
		//把所有物添加到当前玩家面前
		presentPlayer.addKeeper(*msgbufCards[0]);
		//把所有物从目标玩家面前删除
		tmp=-1;
		for(int i=0;i<players.size();i++)
		{
			if(i==clientNum)
				continue;
			for(int j=0;j<players[i].getkeeper().size();j++)
				if(players[i].getkeeper()[j]==msgbufCards[0])		//找到谁有这张所有物
				{
					players[i].removeKeeper(*msgbufCards[0]);
					tmp=i;
					break;
				}
			if(tmp!=-1)
				break;
		}
		//告诉每个玩家这两个人的所有物信息
		for(int i=0;i<players.size();i++)
		{
			//另一个玩家的
			msgbufMsgtype=KEEPER_UPDATE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[tmp].getkeeper();
			msgbufAdditional=0;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,tmp,msgbufAdditional);
			//当前玩家的
			msgbufMsgtype=KEEPER_UPDATE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[clientNum].getkeeper();
			msgbufAdditional=0;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,clientNum,msgbufAdditional);
		}
		break;

	case 11://抽税

		//当前玩家从每个玩家手牌中随机抽一张
		srand(time(NULL));
		for(int i=0;i<players.size();i++)
		{
			if(i==clientNum)
				continue;
			if(players[i].getHandcnt()==0)	//如果这个玩家没有手牌，则不抽
				continue;
			tmp=rand()%(players[i].getHandcnt());
			presentPlayer.addHand(*players[i].gethand()[tmp]);
			players[i].removeHand(*players[i].gethand()[tmp]);
		}
		//广播更新手牌信息
		for(int i=0;i<players.size();i++)
		{
			//更新该玩家自己手牌信息
			msgbufMsgtype=CARD_UPDATE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=CARD_UPDATE;
			msgbufCards=players[i].gethand();
			msgbufAdditional=2;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
			//更新其他玩家手牌数量
			for(int j=0;j<players.size();j++)
			{
				if(j==i)
					continue;
				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[j].getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,j,msgbufAdditional);
			}
		}
		break;

	case 12://取消规则

		//发送和接收弃规则卡信息
		msgbufMsgtype=DROP_RULE_C;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=DROP_RULE_C;
		msgbufAdditional=1;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufAdditional);
		msgbufMsgtype=DROP_RULE_I;
		msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards);
		//把玩家选择的规则卡弃掉
		for(int i=0;i<1;i++)
		{
			if(msgbufCards[i]->getType==Card::NEW_RULE)
			{
				if(msgbufCards[i]->getNum()/10==1)	//手牌上限
				{	
					if(rule.gethandlimitation()!=-1)
						droppeddeck.push_back(&rule.getChandlimit());
					rule.sethandlimitation(-1);
				}
				if(msgbufCards[i]->getNum()/10==2)	//所有物上限
				{
					if(rule.getkeeperlimitation()!=-1)
						droppeddeck.push_back(&rule.getCkeeperlimit());
					rule.setkeeperlimitation(-1);
				}
				if(msgbufCards[i]->getNum()/10==3)	//抓牌数
				{
					if(rule.getdraw()!=1)
						droppeddeck.push_back(&rule.getdrawrule());
					rule.setplay(1);
					for(int j=0;j<cards.getCardNum();j++)
						if(cards.getCard(j)->getType==Card::BASIC_RULE&&cards.getCard(j)->getNum()==0)
						{
							rule.setdrawrule(*cards.getCard(j));
							break;
						}
				}
				if(msgbufCards[i]->getNum()/10==4)	//出牌数
				{
					if(rule.getplay()!=1)
						droppeddeck.push_back(&rule.getplayrule());
					rule.setplay(1);
					for(int j=0;j<cards.getCardNum();j++)
						if(cards.getCard(j)->getType==Card::BASIC_RULE&&cards.getCard(j)->getNum()==1)
						{
							rule.setplayrule(*cards.getCard(j));
							break;
						}
				}
				if(msgbufCards[i]->getNum()/10==5)	//特殊规则
				{
					if(msgbufCards[i]->getNum()%10==1)//出光手牌
					{
						if(rule.getplay()!=1)
							droppeddeck.push_back(&rule.getplayrule());
						rule.setplay(1);
						for(int j=0;j<cards.getCardNum();j++)
							if(cards.getCard(j)->getType==Card::BASIC_RULE&&cards.getCard(j)->getNum()==1)
							{
								rule.setplayrule(*cards.getCard(j));
								break;
							}
					}
					if(msgbufCards[i]->getNum()%10==2)//富人奖励
					{
						rule.setrichbonus(false);
						droppeddeck.push_back(&rule.getCrichbonus());
					}
					if(msgbufCards[i]->getNum()%10==3)//穷人奖励
					{
						rule.setpoorbonus(false);
						droppeddeck.push_back(&rule.getCpoorbonus());
					}
					if(msgbufCards[i]->getNum()%10==4)//通货膨胀
					{
						rule.setinflation(false);
						droppeddeck.push_back(&rule.getCinflation());
					}
					if(msgbufCards[i]->getNum()%10==5)//顺序翻转
					{
						rule.setorderreverse(false);
						droppeddeck.push_back(&rule.getCorderreverse());
					}
					if(msgbufCards[i]->getNum()%10==6)//无手牌奖励
					{
						rule.setnohandbonus(false);
						droppeddeck.push_back(&rule.getCnohandbonus());
					}
					if(msgbufCards[i]->getNum()%10==7)//随机开场
					{
						rule.setrandomstart(false);
						droppeddeck.push_back(&rule.getCrandomstart());
					}
					if(msgbufCards[i]->getNum()%10==8)//双重目标
					{
						rule.setdoublegoals(false);
						droppeddeck.push_back(&rule.getCdoublegoals());
					}
				}//特殊规则结束
			}//本张规则卡遍历所有类型的规则结束
		}//所有的规则卡弃牌结束
		//广播规则更新
		for(int i=0;i<players.size();i++)
		{
			msgbufMsgtype=RULE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=RULE;
			msgbufCards.clear();
			_updateRules();
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
		}
		break;
		
	case 13://交换手牌

		//让玩家选择要交换哪个玩家的手牌，并接收玩家的选择
		msgbufMsgtype=CHOOSE_PLAYER_C;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_PLAYER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufAdditional);
		//交换
		tmp=msgbufAdditional;
		std::vector<const Card*>& player_cards_tmp=presentPlayer.gethand();
		std::vector<const Card*>& player_cards_tmp2=players[tmp].gethand();
		player_cards=player_cards_tmp;
		player_cards_tmp=player_cards_tmp2;
		player_cards_tmp2=player_cards;
		//广播手牌变化
		for(int i=0;i<players.size();i++)
		{
			if(i==clientNum)	//当前玩家
			{
				//更新该玩家手牌信息
				msgbufMsgtype=CARD_UPDATE;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_UPDATE;
				msgbufCards=players[i].gethand();
				msgbufAdditional=2;
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
				//更新另一个玩家手牌数量
				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[tmp].getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,tmp,msgbufAdditional);
			}
			else if(i==tmp)	//另一个玩家
			{
				//更新该玩家手牌信息
				msgbufMsgtype=CARD_UPDATE;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_UPDATE;
				msgbufCards=players[i].gethand();
				msgbufAdditional=2;
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
				//更新出牌玩家手牌数量
				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[clientNum].getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,clientNum,msgbufAdditional);
			}
			else        //其他无关的玩家
			{
				//更新这两个玩家的手牌数量
				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[clientNum].getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,clientNum,msgbufAdditional);

				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[tmp].getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,tmp,msgbufAdditional);
			}
		}
		break;

	case 14://各拿1张

		//每个玩家摸一张
		tmp=clientNum;
		for(int i=0;i<players.size();i++)
		{
			clientNum=i;
			dealCard(1);
		}
		clientNum=tmp;
		break;

	case 15://取而用之

		//通知玩家选择一位玩家，并获取其选择
		msgbufMsgtype=CHOOSE_PLAYER_C;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_PLAYER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufAdditional);
		//从目标玩家的手牌中随机选择一张手牌删除
		tmp=msgbufAdditional;
		srand(time(NULL));
		tmp_card=players[tmp].gethand()[rand()%(players[tmp].getHandcnt())];
		players[tmp].removeHand(*tmp_card);
		//告诉目标玩家手牌少了哪张，并广播其他玩家这位玩家的手牌数
		for(int i=0;i<players.size();i++)
		{
			if(i==tmp)
			{
				msgbufMsgtype=CARD_UPDATE;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_UPDATE;
				msgbufCards.clear();
				msgbufCards.push_back(tmp_card);
				msgbufAdditional=0;
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
			}
			else
			{
				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[tmp].getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,tmp,msgbufAdditional);
			}
		}
		//将该手牌偷偷加入本玩家手牌中并偷偷打出
		presentPlayer.addHand(*tmp_card);
		playCard(*tmp_card);
		break;

	case 16://清除垃圾

		//把未摸完的牌全部放入弃牌堆
		while(deck.size()>0)
		{
			droppeddeck.push_back(deck.back());
			deck.pop_back();
		}
		//洗牌
		_shuffleCard();
		//把这张卡作为新的弃牌堆
		droppeddeck.push_back(&act_card);
		break;

	case 17://取消上限

		if(rule.gethandlimitation()!=-1)
		{
			rule.sethandlimitation(-1);
			droppeddeck.push_back(&rule.getChandlimit());
		}
		if(rule.getkeeperlimitation()!=-1)
		{
			rule.setkeeperlimitation(-1);
			droppeddeck.push_back(&rule.getCkeeperlimit());
		}
		//广播规则更新
		for(int i=0;i<players.size();i++)
		{
			msgbufMsgtype=RULE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=RULE;
			msgbufCards.clear();
			_updateRules();
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
		}
		break;

	case 18://重新分配

		//将所有的所有物放在一起
		for(int i=0;i<players.size();i++)
		{
			std::vector<const Card*>& player_cards_tmp=players[i].getkeeper();
			cnt.push_back(player_cards_tmp.size());
			while(player_cards_tmp.size()>0)
			{
				player_cards.push_back(player_cards_tmp.back());
				player_cards_tmp.pop_back();
			}
		}
		//洗匀所有物
		for(int i=0;i<player_cards.size();i++)
		{
			tmp=rand()%(player_cards.size());
			tmp_card=player_cards[i];
			player_cards[i]=player_cards[tmp];
			player_cards[tmp]=tmp_card;
		}
		//发还所有物
		for(int i=0;i<players.size();i++)
		{
			for(int j=0;j<cnt[i];j++)
			{
				players[i].addKeeper(*player_cards.back());
				player_cards.pop_back();
			}
		}
		//告诉每个玩家所有人的所有物信息
		for(int i=0;i<players.size();i++)
			for(int j=0;j<players.size();j++)
			{
				msgbufMsgtype=KEEPER_UPDATE;
				msgBox.createMsg(i,msgbufMsgtype);
				msgbufMsgtype=KEEPER_UPDATE;
				msgbufCards=players[j].getkeeper();
				msgbufAdditional=0;
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards,j,msgbufAdditional);
			}
		break;

	case 19://丢弃

		//玩家选一张所有物
		msgbufMsgtype=CHOOSE_KEEPER_C;
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_KEEPER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
		//把所有物从目标玩家面前删除,目标可以为自己
		tmp=-1;
		for(int i=0;i<players.size();i++)
		{
			for(int j=0;j<players[i].getkeeper().size();j++)
				if(players[i].getkeeper()[j]==msgbufCards[0])		//找到谁有这张所有物
				{
					players[i].removeKeeper(*msgbufCards[0]);
					droppeddeck.push_back(msgbufCards[0]);
					tmp=i;
					break;
				}
			if(tmp!=-1)
				break;
		}
		//告诉每个玩家这个人的所有物信息
		for(int i=0;i<players.size();i++)
		{
			msgbufMsgtype=KEEPER_UPDATE;
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[tmp].getkeeper();
			msgbufAdditional=0;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,tmp,msgbufAdditional);
		}
		break;

	case 20://轮转手牌

		if(rule.isorderreverse())
		{
			std::vector<const Card*>& player_cards_tmp=players[0].gethand();
			player_cards=player_cards_tmp;
			for(int i=0;i<players.size()-1;i++)
			{
				std::vector<const Card*>& player_cards_tmp1=players[i].gethand();
				std::vector<const Card*>& player_cards_tmp2=players[i+1].gethand();
				player_cards_tmp1=player_cards_tmp2;
			}
			std::vector<const Card*>& player_cards_tmp3=players[players.size()-1].gethand();
			player_cards_tmp3=player_cards;
		}
		else
		{
			std::vector<const Card*>& player_cards_tmp=players[players.size()-1].gethand();
			player_cards=player_cards_tmp;
			for(int i=players.size()-1;i>0;i--)
			{
				std::vector<const Card*>& player_cards_tmp1=players[i].gethand();
				std::vector<const Card*>& player_cards_tmp2=players[i-1].gethand();
				player_cards_tmp1=player_cards_tmp2;
			}
			std::vector<const Card*>& player_cards_tmp3=players[0].gethand();
			player_cards_tmp3=player_cards;
		}
		//广播手牌信息和张数
		for(int i=0;i<players.size();i++)
		{
			for(int j=0;j<players.size();j++)
			{
				if(j==i)	//当前玩家手牌信息
				{
					msgbufMsgtype=CARD_UPDATE;
					msgBox.createMsg(i,msgbufMsgtype);
					msgbufMsgtype=CARD_UPDATE;
					msgbufCards=players[i].gethand();
					msgbufAdditional=2;
					msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
				}
				else     //其他玩家手牌数量
				{
					msgbufMsgtype=CARD_NUM;
					msgBox.createMsg(i,msgbufMsgtype);
					msgbufMsgtype=CARD_NUM;
					msgbufAdditional=players[j].getHandcnt();
					msgBox.createMsg(i,msgbufMsgtype,j,msgbufAdditional);
				}
			}
		}

	}//switch结束

}
