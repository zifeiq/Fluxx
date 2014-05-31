#ifndef FLUXXCONTROL_H
#define FLUXXCONTROL_H

#include <vector>
#include "fluxxrules.h"
#include "card.h"

class Player;
class Msgmanager;

typedef int ruleMsg;
typedef int cardNum;

class fluxxControl {
public:
	fluxxControl ();
	void initcards();//初始化手牌，从牌库获取引用过来，在默认构造函数中使用
	void initrules();//初始化规则，在默认构造函数中使用
	void addplayers(const Player&);//添加玩家，在默认构造函数和其他适当的时机使用
	void shuffleCard();//洗牌
	void dealCard(ruleMsg); //发牌,入口参数为从规则中得到的信息
	void playCard(ruleMsg);//出牌阶段控制函数，入口参数为从规则中得到的信息
	void settleCard(cardNum); //单张出牌与结算，入口参数为卡牌编码
	void dropCard(ruleMsg);//弃牌阶段控制函数，口参数为从规则中得到的信息
	~fluxxControl ();

private:
	fluxxRules rule;
	std::vector<Player> players;
	std::vector<Card>& cards;
};

#endif