#ifndef CARDLIB_H
#define CARDLIB_H

#include <vector>
#include <fstream>
const std::string FILENAME = "carddata.txt";

class Card  //卡牌的基类
{
public:
	Card() :_type(BASIC_RULE), _num(0){}//基类默认构造函数
	Card(int num) :_type(BASIC_RULE), _num(num){} 
	enum Type{BASIC_RULE, NEW_RULE, KEEPER, GOAL, ACTION};
	Type getType() const{ return _type; } //返回本卡的属性
	int getNum() const{ return _num; }  //返回本卡的编号
	virtual ~Card(){}
	bool operator==(const Card& c) const;
	Card& operator =(const Card& c);
protected:
	Type _type;
	int _num; //本卡牌的编号（两位）
};

class RuleCard :public Card  //规则牌
{
public:
	RuleCard(): Card(){}
	RuleCard(int num) : Card(num){}
};

class BasicRuleCard : public RuleCard //基本规则牌
{
public:
	BasicRuleCard(int num) :RuleCard(num){}
};

class NewRuleCard :public RuleCard  //新规则牌
{
public:
	NewRuleCard(int num){ _type = NEW_RULE; _num = num;}
};

class KeeperCard :public Card  //所有物卡牌
{
public:
	KeeperCard(int num) { _type = KEEPER; _num = num; }
};

class GoalCard :public Card  //目标牌
{
public:
	GoalCard(int num) { _type = GOAL; _num = num; }
};

class ActionCard :public Card  //行动牌
{
public:
	ActionCard(int num){ _type = ACTION; _num = num; }
};

class CardLib  //牌库，singleton模式
{
public:
	static CardLib& getLib()
	{
		static CardLib allcards;
		return allcards;
	}
	Card* getCard(int i) const{ return _cards[i]; }
	int getCardNum() const { return _cardNum; }
	void getInfo(const Card* goal, std::vector<const Card*>& keepers) const;//查询目标卡对应的所有物卡
protected:
	CardLib();
	~CardLib();
	std::vector<Card*> _cards;  //基本卡为63个, 行动卡和用户拓展卡(How????)  后期可加
	int _cardNum;
};

#endif
