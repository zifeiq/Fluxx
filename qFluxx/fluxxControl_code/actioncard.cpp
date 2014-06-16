#include "fluxxcontrol.h"
#include<time.h>

void fluxxControl::actioncard(const Card& act_card)
{
	std::vector<const Card*> player_cards;	
    std::vector<const Card*>::iterator ii = droppeddeck.begin();
	std::vector<int> cnt;	
	int tmp;
	const Card* tmp_card;

	switch(act_card.getNum()) {
	case 1:
	{
		
		std::vector<const Card*>& player_cards_tmp = presentPlayer->gethand();
		hand_cards_buf.push_back(player_cards_tmp);
		
		player_cards_tmp.clear();	
		
		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();
		
		dealCard(2);
		
		for(unsigned int i = 0;i<2;i++)
		{
			msgbufMsgtype=PLAY_C;
            Sleep(300);
			msgBox.createMsg(clientNum,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=PLAY_I;
			msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
			playCard(msgbufCards[0]);	
		}
		
		player_cards_tmp=hand_cards_buf.back();
		hand_cards_buf.pop_back();
		
		for(unsigned int i=0;i<players.size();i++)
		{
			if(i==clientNum)
			{
				msgbufMsgtype=CARD_UPDATE;
				msgBox.createMsg(clientNum,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_UPDATE;
				msgbufCards=presentPlayer->gethand();
				msgbufAdditional=2;
				msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
                Sleep(300);
				msgbufCards.clear();
			}
			else
			{
				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_NUM;
				msgbufCards=presentPlayer->gethand();
				msgbufAdditional=presentPlayer->getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,clientNum,msgbufAdditional);
                Sleep(300);
				msgbufCards.clear();
			}
		}
		break;
	}
	case 2:
	{
		
		std::vector<const Card*>& player_cards_tmp=presentPlayer->gethand();
		hand_cards_buf.push_back(player_cards_tmp);
		player_cards_tmp=hand_cards_buf[0];

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();

		tmp=player_cards_tmp.size();
		while(player_cards_tmp.size()>0)
		{
			droppeddeck.push_back(player_cards_tmp.back());
			player_cards_tmp.pop_back();
		}
		
		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();
		
		dealCard(tmp);

		hand_cards_buf[0]=player_cards_tmp;
		player_cards_tmp=hand_cards_buf.back();
		hand_cards_buf.pop_back();

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();

		break;
	}
	case 3:
	{
		
		if(rule.getdraw()!=1)
		{
			rule.setdraw(1);
			droppeddeck.push_back(&rule.getdrawrule());
			rule.setdrawrule(cards.getCard(0));
		}
		
		if(rule.getplay()!=1)
		{
			rule.setplay(1);
			droppeddeck.push_back(&rule.getplayrule());
			rule.setplayrule(cards.getCard(1));
		}
		
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
		
		if(rule.isorderreverse())
		{
			rule.setorderreverse(false);
			droppeddeck.push_back(&rule.getCorderreverse());
		}
		
		if(rule.isdoublegoals())
		{
			rule.setdoublegoals(false);
			droppeddeck.push_back(&rule.getCdoublegoals());
			if(rule.secondgoal().getNum()!=0)
			{
				droppeddeck.push_back(&rule.secondgoal());
				rule.setsecondgoal(cards.getCard(0));
			}
		}
		
		if(rule.isrichBonus())
		{
			rule.setrichbonus(false);
			droppeddeck.push_back(&rule.getCrichbonus());
		}
		
		if(rule.ispoorBonus())
		{
			rule.setpoorbonus(false);
			droppeddeck.push_back(&rule.getCpoorbonus());
		}
		
		if(rule.isinflation())
		{
			rule.setinflation(false);
			droppeddeck.push_back(&rule.getCinflation());
		}
		
		if(rule.isnohandbonus())
		{
			rule.setnohandbonus(false);
			droppeddeck.push_back(&rule.getCnohandbonus());
		}
		
		if(rule.israndomstart())
		{
			rule.setrandomstart(false);
			droppeddeck.push_back(&rule.getCrandomstart());
		}
		
		for(unsigned int i=0;i<players.size();i++)
		{
			msgbufMsgtype=RULE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
			msgbufMsgtype=RULE;
			msgbufCards.clear();
			_updateRules();
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
		}
		msgbufCards.clear();
		break;
	}
	case 4:
	{
		
		std::vector<const Card*>& player_cards_tmp=presentPlayer->gethand();
		hand_cards_buf.push_back(player_cards_tmp);
		
		player_cards_tmp.clear();	
		
		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();
		
		dealCard(3);
		
		for(unsigned int i=0;i<2;i++)
		{
			msgbufMsgtype=PLAY_C;
            Sleep(300);
			msgBox.createMsg(clientNum,msgbufMsgtype);
			msgbufMsgtype=PLAY_I;
			msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
			playCard(msgbufCards[0]);	
		}
		
		dropCard(1);
		
		player_cards_tmp=hand_cards_buf.back();
		hand_cards_buf.pop_back();
		
		for(unsigned int i=0;i<players.size();i++)
		{
			if(i==clientNum)
			{
				msgbufMsgtype=CARD_UPDATE;
                Sleep(300);
				msgBox.createMsg(clientNum,msgbufMsgtype);
				msgbufMsgtype=CARD_UPDATE;
				msgbufCards=presentPlayer->gethand();
				msgbufAdditional=2;
                Sleep(300);
				msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
				msgbufCards.clear();
			}
			else
			{
				msgbufMsgtype=CARD_NUM;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_NUM;
				msgbufCards=presentPlayer->gethand();
				msgbufAdditional=presentPlayer->getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,clientNum,msgbufAdditional);
				msgbufCards.clear();
			}
		}
		break;
	}
	case 5:
	{
		std::vector<const Card*>& player_cards_tmp=presentPlayer->gethand();
		hand_cards_buf.push_back(player_cards_tmp);
		player_cards_tmp=hand_cards_buf[0];

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();
		
		dealCard(3);

		hand_cards_buf[0]=player_cards_tmp;
		player_cards_tmp=hand_cards_buf.back();
		hand_cards_buf.pop_back();

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();

		break;
	}
	case 6:
	{
		
		for(unsigned int i=0; i<droppeddeck.size(); i++)
			if(droppeddeck[i]->getType()==Card::ACTION||droppeddeck[i]->getType()==Card::NEW_RULE)
				player_cards.push_back(droppeddeck[i]);
		if(player_cards.size()==0)
			break;	
		
		srand(int(time(NULL)));
		tmp=rand()%(player_cards.size());
		presentPlayer->addHand(*player_cards[tmp]);
		
		for(; ii != droppeddeck.end(); ii++) 
			if ((**ii) == *player_cards[tmp]) 
			{
				droppeddeck.erase(ii);
				break;
			}

		playCard(player_cards[tmp]);
		break;
	}
	case 7:
	{
		cnt.push_back(0);	
		if(rule.getdraw()!=1)
			cnt[0]++;
		if(rule.getplay()!=1)
			cnt[0]++;
		if(rule.gethandlimitation()!=-1)
			cnt[0]++;
		if(rule.getkeeperlimitation()!=-1)
			cnt[0]++;
		if(rule.isorderreverse())
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
		
		if(cnt[0]%2==1)
			tmp=cnt[0]/2+1;
		else
			tmp=cnt[0]/2;
		if(tmp == 0) 
			break;
		msgbufMsgtype=DROP_RULE_C;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=DROP_RULE_C;
		msgbufAdditional=tmp;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufAdditional);
		msgbufMsgtype=DROP_RULE_I;
		msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards);
		
		for(unsigned int i=0;i<msgbufCards.size();i++)
		{
			if(msgbufCards[i]->getType() == Card::NEW_RULE)
			{
				if(msgbufCards[i]->getNum()/10==1)	
				{	
					if(rule.gethandlimitation()!=-1)
						droppeddeck.push_back(&rule.getChandlimit());
					rule.sethandlimitation(-1);
				}
				if(msgbufCards[i]->getNum()/10==2)	
				{
					if(rule.getkeeperlimitation()!=-1)
						droppeddeck.push_back(&rule.getCkeeperlimit());
					rule.setkeeperlimitation(-1);
				}
				if(msgbufCards[i]->getNum()/10==3)	
				{
					if(rule.getdraw()!=1)
						droppeddeck.push_back(&rule.getdrawrule());
					rule.setdraw(1);
					rule.setdrawrule(cards.getCard(0));
				}
				if(msgbufCards[i]->getNum()/10==4)	
				{
					if(rule.getplay()!=1)
						droppeddeck.push_back(&rule.getplayrule());
					rule.setplay(1);
					rule.setplayrule(cards.getCard(1));
				}
				if(msgbufCards[i]->getNum()/10==5)	
				{
					if(msgbufCards[i]->getNum()%10==1)
					{
						if(rule.getplay()!=1)
							droppeddeck.push_back(&rule.getplayrule());
						rule.setplay(1);
						rule.setplayrule(cards.getCard(1));
					}
					if(msgbufCards[i]->getNum()%10==2)
					{
						rule.setrichbonus(false);
						droppeddeck.push_back(&rule.getCrichbonus());
					}
					if(msgbufCards[i]->getNum()%10==3)
					{
						rule.setpoorbonus(false);
						droppeddeck.push_back(&rule.getCpoorbonus());
					}
					if(msgbufCards[i]->getNum()%10==4)
					{
						rule.setinflation(false);
						droppeddeck.push_back(&rule.getCinflation());
					}
					if(msgbufCards[i]->getNum()%10==5)
					{
						rule.setorderreverse(false);
						droppeddeck.push_back(&rule.getCorderreverse());
					}
					if(msgbufCards[i]->getNum()%10==6)
					{
						rule.setnohandbonus(false);
						droppeddeck.push_back(&rule.getCnohandbonus());
					}
					if(msgbufCards[i]->getNum()%10==7)
					{
						rule.setrandomstart(false);
						droppeddeck.push_back(&rule.getCrandomstart());
					}
					if(msgbufCards[i]->getNum()%10==8)
					{
						rule.setdoublegoals(false);
						droppeddeck.push_back(&rule.getCdoublegoals());
					}
				}
			}
		}
		
		for(unsigned int i=0;i<players.size();i++)
		{
			msgbufMsgtype=RULE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=RULE;
			msgbufCards.clear();
			_updateRules();
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
		}
		msgbufCards.clear();
		break;
	}
	case 8:
	{
		
		if(presentPlayer->getKeepercnt()==0)
			break;
		
		tmp=0;
		for(unsigned int i=0;i<players.size();i++)
		{
			if(i==clientNum)
				continue;
			if(players[i]->getKeepercnt()>0)
				tmp=-1;
		}
		if(tmp==0)
			break;
		
		msgbufMsgtype=EXCHANGE_KEEPER_C;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_KEEPER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
		
		tmp=-1;
		for(unsigned int i=0;i<players.size();i++)
		{
			if(i==clientNum)
				continue;
			for(unsigned int j=0;j<players[i]->getkeeper().size();j++)
				if(players[i]->getkeeper()[j]==msgbufCards[0])		
				{
					players[i]->removeKeeper(*msgbufCards[0]);
					players[i]->addKeeper(*msgbufCards[1]);
					tmp=i;
					break;
				}
			if(tmp!=-1)
				break;
		}
		
		presentPlayer->removeKeeper(*msgbufCards[1]);
		presentPlayer->addKeeper(*msgbufCards[0]);
		
		for(unsigned int i=0;i<players.size();i++)
		{
			
			msgbufMsgtype=KEEPER_UPDATE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[tmp]->getkeeper();
			msgbufAdditional=0;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,tmp,msgbufAdditional);
			
			msgbufMsgtype=KEEPER_UPDATE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[clientNum]->getkeeper();
			msgbufAdditional=0;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,clientNum,msgbufAdditional);
		}
		msgbufCards.clear();
		break;
	}
	case 9:
	{
		rule.setanotherround(true);
		break;
	}
	case 10:
	{
		tmp=0;
		for(unsigned int i=0;i<players.size();i++)
		{
			if(i==clientNum)
				continue;
			if(players[i]->getKeepercnt()>0)
				tmp=-1;
		}
		if(tmp==0)
			break;

		msgbufMsgtype=CHOOSE_KEEPER_C;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_KEEPER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
		
		presentPlayer->addKeeper(*msgbufCards[0]);
		
		tmp=-1;
		for(unsigned int i=0;i<players.size();i++)
		{
			if(i==clientNum)
				continue;
			for(unsigned int j=0;j<players[i]->getkeeper().size();j++)
				if(players[i]->getkeeper()[j]==msgbufCards[0])		
				{
					players[i]->removeKeeper(*msgbufCards[0]);
					tmp=i;
					break;
				}
			if(tmp!=-1)
				break;
		}
		
		for(unsigned int i=0;i<players.size();i++)
		{
			
			msgbufMsgtype=KEEPER_UPDATE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[tmp]->getkeeper();
			msgbufAdditional=2;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,tmp,msgbufAdditional);
			
			msgbufMsgtype=KEEPER_UPDATE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[clientNum]->getkeeper();
			msgbufAdditional=2;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,clientNum,msgbufAdditional);
		}
		msgbufCards.clear();
		break;
	}
	case 11:
	{
		std::vector<const Card*>& player_cards_tmp=presentPlayer->gethand();
		hand_cards_buf.push_back(player_cards_tmp);
		player_cards_tmp=hand_cards_buf[0];

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();
		
		srand(int(time(NULL)));
		for(unsigned int i=0;i<players.size();i++)
		{
			if(i==clientNum)
				continue;
			if(players[i]->getHandcnt()==0)	
				continue;
			tmp=rand()%(players[i]->getHandcnt());
			presentPlayer->addHand(*players[i]->gethand()[tmp]);
			players[i]->removeHand(*players[i]->gethand()[tmp]);
		}
		
		for(unsigned int i=0;i<players.size();i++)
		{
			
			msgbufMsgtype=CARD_UPDATE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=CARD_UPDATE;
			msgbufCards=players[i]->gethand();
			msgbufAdditional=2;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
			
			for(unsigned int j=0;j<players.size();j++)
			{
				if(j==i)
					continue;
				msgbufMsgtype=CARD_NUM;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[j]->getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,j,msgbufAdditional);
			}
		}
		msgbufCards.clear();

		hand_cards_buf[0]=player_cards_tmp;
		player_cards_tmp=hand_cards_buf.back();
		hand_cards_buf.pop_back();

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();

		break;
	}
	case 12:
	{
		cnt.push_back(0);	
		if(rule.getdraw()!=1)
			cnt[0]++;
		if(rule.getplay()!=1)
			cnt[0]++;
		if(rule.gethandlimitation()!=-1)
			cnt[0]++;
		if(rule.getkeeperlimitation()!=-1)
			cnt[0]++;
		if(rule.isorderreverse())
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
		if(cnt[0] == 0) 
			break;
		
		msgbufMsgtype=DROP_RULE_C;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=DROP_RULE_C;
		msgbufAdditional=1;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufAdditional);
		msgbufMsgtype=DROP_RULE_I;
		msgBox.getMsg(clientNum, msgbufMsgtype, msgbufCards);
		
		for(int i=0;i<1;i++)
		{
			if(msgbufCards[i]->getType() == Card::NEW_RULE)
			{
				if(msgbufCards[i]->getNum()/10==1)	
				{	
					if(rule.gethandlimitation()!=-1)
						droppeddeck.push_back(&rule.getChandlimit());
					rule.sethandlimitation(-1);
				}
				if(msgbufCards[i]->getNum()/10==2)	
				{
					if(rule.getkeeperlimitation()!=-1)
						droppeddeck.push_back(&rule.getCkeeperlimit());
					rule.setkeeperlimitation(-1);
				}
				if(msgbufCards[i]->getNum()/10==3)	
				{
					if(rule.getdraw()!=1)
						droppeddeck.push_back(&rule.getdrawrule());
					rule.setplay(1);
					rule.setdrawrule(cards.getCard(0));
				}
				if(msgbufCards[i]->getNum()/10==4)	
				{
					if(rule.getplay()!=1)
						droppeddeck.push_back(&rule.getplayrule());
					rule.setdraw(1);
					rule.setdrawrule(cards.getCard(1));
				}
				if(msgbufCards[i]->getNum()/10==5)	
				{
					if(msgbufCards[i]->getNum()%10==1)
					{
						if(rule.getplay()!=1)
							droppeddeck.push_back(&rule.getplayrule());
						rule.setplay(1);
						rule.setplayrule(cards.getCard(1));
					}
					if(msgbufCards[i]->getNum()%10==2)
					{
						rule.setrichbonus(false);
						droppeddeck.push_back(&rule.getCrichbonus());
					}
					if(msgbufCards[i]->getNum()%10==3)
					{
						rule.setpoorbonus(false);
						droppeddeck.push_back(&rule.getCpoorbonus());
					}
					if(msgbufCards[i]->getNum()%10==4)
					{
						rule.setinflation(false);
						droppeddeck.push_back(&rule.getCinflation());
					}
					if(msgbufCards[i]->getNum()%10==5)
					{
						rule.setorderreverse(false);
						droppeddeck.push_back(&rule.getCorderreverse());
					}
					if(msgbufCards[i]->getNum()%10==6)
					{
						rule.setnohandbonus(false);
						droppeddeck.push_back(&rule.getCnohandbonus());
					}
					if(msgbufCards[i]->getNum()%10==7)
					{
						rule.setrandomstart(false);
						droppeddeck.push_back(&rule.getCrandomstart());
					}
					if(msgbufCards[i]->getNum()%10==8)
					{
						rule.setdoublegoals(false);
						droppeddeck.push_back(&rule.getCdoublegoals());
					}
				}
			}
		}
		
		for(unsigned int i = 0;i<players.size();i++)
		{
			msgbufMsgtype=RULE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=RULE;
			msgbufCards.clear();
			_updateRules();
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
		}
		msgbufCards.clear();
		break;
	}
	case 13:
	{
		std::vector<const Card*>& player_cards_tmp=presentPlayer->gethand();
		hand_cards_buf.push_back(player_cards_tmp);
		player_cards_tmp=hand_cards_buf[0];

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();
		
		msgbufMsgtype=CHOOSE_PLAYER_C;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_PLAYER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufAdditional);
		
		tmp=msgbufAdditional;
		//std::vector<const Card*>& player_cards_tmp=presentPlayer->gethand();
		std::vector<const Card*>& player_cards_tmp2=players[tmp]->gethand();
		player_cards=player_cards_tmp;
		player_cards_tmp=player_cards_tmp2;
		player_cards_tmp2=player_cards;
		
		for(unsigned int i = 0;i<players.size();i++)
		{
			if(i==clientNum)	
			{
				
				msgbufMsgtype=CARD_UPDATE;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_UPDATE;
				msgbufCards=players[i]->gethand();
				msgbufAdditional=2;
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
				
				msgbufMsgtype=CARD_NUM;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[tmp]->getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,tmp,msgbufAdditional);
			}
			else if(i==tmp)	
			{
				
				msgbufMsgtype=CARD_UPDATE;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_UPDATE;
				msgbufCards=players[i]->gethand();
				msgbufAdditional=2;
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
                Sleep(300);
				
				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[clientNum]->getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,clientNum,msgbufAdditional);
				
			}
			else        
			{
				
				msgbufMsgtype=CARD_NUM;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[clientNum]->getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,clientNum,msgbufAdditional);
                Sleep(300);

				msgbufMsgtype=CARD_NUM;
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[tmp]->getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,tmp,msgbufAdditional);
				
			}
		}
		msgbufCards.clear();

		hand_cards_buf[0]=player_cards_tmp;
		player_cards_tmp=hand_cards_buf.back();
		hand_cards_buf.pop_back();

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		
		msgbufCards.clear();

		break;
	}
	case 14:
	{
		std::vector<const Card*>& player_cards_tmp=presentPlayer->gethand();
		hand_cards_buf.push_back(player_cards_tmp);
		player_cards_tmp=hand_cards_buf[0];

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();
		
		tmp=clientNum;
		for(unsigned int i = 0;i<players.size();i++)
		{
			clientNum=i;
			dealCard(1);
		}
		clientNum=tmp;

		hand_cards_buf[0]=player_cards_tmp;
		player_cards_tmp=hand_cards_buf.back();
		hand_cards_buf.pop_back();

		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();

		break;
	}
	case 15:
	{
		
		msgbufMsgtype=CHOOSE_PLAYER_C;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_PLAYER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufAdditional);
		
		tmp=msgbufAdditional;
		srand(int(time(NULL)));
		tmp_card=players[tmp]->gethand()[rand()%(players[tmp]->getHandcnt())];
		players[tmp]->removeHand(*tmp_card);
		
		for(unsigned int i = 0;i<players.size();i++)
		{
			if(i==tmp)
			{
				msgbufMsgtype=CARD_UPDATE;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_UPDATE;
				msgbufCards.clear();
				msgbufCards.push_back(tmp_card);
				msgbufAdditional=0;
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
			}
			else
			{
				msgbufMsgtype=CARD_NUM;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=CARD_NUM;
				msgbufAdditional=players[tmp]->getHandcnt();
				msgBox.createMsg(i,msgbufMsgtype,tmp,msgbufAdditional);
			}
		}
		msgbufCards.clear();
		
		presentPlayer->addHand(*tmp_card);
		playCard(tmp_card);
		break;
	}
	case 16:
	{
		
		while(deck.size()>0)
		{
			droppeddeck.push_back(deck.back());
			deck.pop_back();
		}
		_shuffleCard();
		break;
	}
	case 17:
	{
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
		
		for(unsigned int i = 0;i<players.size();i++)
		{
			msgbufMsgtype=RULE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=RULE;
			msgbufCards.clear();
			_updateRules();
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards);
		}
		msgbufCards.clear();
		break;
	}
	case 18:
	{
		
		tmp=0;
		for(unsigned int i=0;i<players.size();i++)
		{
			if(players[i]->getKeepercnt()>0)
				tmp++;
		}
		if(tmp<2)
			break;
		
		for(unsigned int i = 0;i<players.size();i++)
		{
			std::vector<const Card*>& player_cards_tmp=players[i]->getkeeper();
			cnt.push_back(player_cards_tmp.size());
			while(player_cards_tmp.size()>0)
			{
				player_cards.push_back(player_cards_tmp.back());
				player_cards_tmp.pop_back();
			}
		}
		
		for(unsigned int i = 0;i<player_cards.size();i++)
		{
			tmp=rand()%(player_cards.size());
			tmp_card=player_cards[i];
			player_cards[i]=player_cards[tmp];
			player_cards[tmp]=tmp_card;
		}
		
		for(unsigned int i = 0;i<players.size();i++)
		{
			for(int j=0;j<cnt[i];j++)
			{
				players[i]->addKeeper(*player_cards.back());
				player_cards.pop_back();
			}
		}
		
		for(unsigned int i = 0;i<players.size();i++)
			for(unsigned int j=0;j<players.size();j++)
			{
				msgbufMsgtype=KEEPER_UPDATE;
                Sleep(300);
				msgBox.createMsg(i,msgbufMsgtype);
                Sleep(300);
				msgbufMsgtype=KEEPER_UPDATE;
				msgbufCards=players[j]->getkeeper();
				msgbufAdditional=2;
				msgBox.createMsg(i,msgbufMsgtype,msgbufCards,j,msgbufAdditional);
			}
			msgbufCards.clear();
		break;
	}
	case 19:
	{
		
		tmp=0;
		for(unsigned int i=0;i<players.size();i++)
		{
			if(players[i]->getKeepercnt()>0)
				tmp++;
		}
		if(tmp==0)
			break;
		
		msgbufMsgtype=CHOOSE_KEEPER_C;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
		msgbufMsgtype=CHOOSE_KEEPER_I;
		msgBox.getMsg(clientNum,msgbufMsgtype,msgbufCards);
		
		tmp=-1;
		for(unsigned int i = 0;i<players.size();i++)
		{
			for(unsigned int j=0;j<players[i]->getkeeper().size();j++)
				if(players[i]->getkeeper()[j]==msgbufCards[0])		
				{
					players[i]->removeKeeper(*msgbufCards[0]);
					droppeddeck.push_back(msgbufCards[0]);
					tmp=i;
					break;
				}
			if(tmp!=-1)
				break;
		}
		
		for(unsigned int i = 0;i<players.size();i++)
		{
			msgbufMsgtype=KEEPER_UPDATE;
            Sleep(300);
			msgBox.createMsg(i,msgbufMsgtype);
            Sleep(300);
			msgbufMsgtype=KEEPER_UPDATE;
			msgbufCards=players[tmp]->getkeeper();
			msgbufAdditional=2;
			msgBox.createMsg(i,msgbufMsgtype,msgbufCards,tmp,msgbufAdditional);
		}
		msgbufCards.clear();
		break;
	}
	case 20:
	{
		
		hand_cards_buf.push_back(presentPlayer->gethand());
		std::vector<const Card*>& player_cards_tmp4=presentPlayer->gethand();
		player_cards_tmp4=hand_cards_buf.front();
		
		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		msgbufCards.clear();
		
        Sleep(2000);
		
		if(rule.isorderreverse())
		{
			std::vector<const Card*>& player_cards_tmp=players[0]->gethand();
			player_cards=player_cards_tmp;
			for(unsigned int i = 0;i<players.size()-1;i++)
			{
				std::vector<const Card*>& player_cards_tmp1=players[i]->gethand();
				std::vector<const Card*>& player_cards_tmp2=players[i+1]->gethand();
				player_cards_tmp1=player_cards_tmp2;
			}
			std::vector<const Card*>& player_cards_tmp3=players[players.size()-1]->gethand();
			player_cards_tmp3=player_cards;
		}
		else
		{
			std::vector<const Card*>& player_cards_tmp=players[players.size()-1]->gethand();
			player_cards=player_cards_tmp;
			for(int i=players.size()-1;i>0;i--)
			{
				std::vector<const Card*>& player_cards_tmp1=players[i]->gethand();
				std::vector<const Card*>& player_cards_tmp2=players[i-1]->gethand();
				player_cards_tmp1=player_cards_tmp2;
			}
			std::vector<const Card*>& player_cards_tmp3=players[0]->gethand();
			player_cards_tmp3=player_cards;
		}
		
		for(unsigned int i = 0;i<players.size();i++)
		{
			for(unsigned int j=0;j<players.size();j++)
			{
				if(j==i)	
				{
					msgbufMsgtype=CARD_UPDATE;
                    Sleep(300);
					msgBox.createMsg(i,msgbufMsgtype);
                    Sleep(300);
					msgbufMsgtype=CARD_UPDATE;
					msgbufCards=players[i]->gethand();
					msgbufAdditional=2;
					msgBox.createMsg(i,msgbufMsgtype,msgbufCards,msgbufAdditional);
					
				}
				else     
				{
					msgbufMsgtype=CARD_NUM;
                    Sleep(300);
					msgBox.createMsg(i,msgbufMsgtype);
                    Sleep(300);
					msgbufMsgtype=CARD_NUM;
					msgbufAdditional=players[j]->getHandcnt();
					msgBox.createMsg(i,msgbufMsgtype,j,msgbufAdditional);
					
				}
			}
		}
		
        Sleep(2000);
		
		hand_cards_buf[0]=player_cards_tmp4;
		player_cards_tmp4=hand_cards_buf.back();
		hand_cards_buf.pop_back();
		
		msgbufMsgtype=CARD_UPDATE;
        Sleep(300);
		msgBox.createMsg(clientNum,msgbufMsgtype);
        Sleep(300);
		msgbufMsgtype=CARD_UPDATE;
		msgbufCards=presentPlayer->gethand();
		msgbufAdditional=2;
		msgBox.createMsg(clientNum,msgbufMsgtype,msgbufCards,msgbufAdditional);
		
		msgbufCards.clear();
	}
	}
}
