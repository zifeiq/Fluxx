#ifndef CARDLIB_H
#define CARDLIB_H

#include <vector>
#include <fstream>
const std::string FILENAME = "carddata.txt";



class Card  //卡牌的基类
{
public:
	Card() :_type(BASIC_RULE), _num(0),_addr(""),_name(""){}//基类默认构造函数
	Card(int num,std::string addr,std::string name) :_type(BASIC_RULE), _num(num), _addr(addr),_name(name){} 
	enum Type{BASIC_RULE, NEW_RULE, KEEPER, GOAL, ACTION};
	Type getType() const{ return _type; } //返回本卡的属性
	int getNum() const{ return _num; }  //返回本卡的编号
    std::string getAddr() const{ return _addr; } //return card file address
	std::string getName() const {return _name;}
	virtual ~Card(){}
	bool operator==(const Card& c) const;
	Card& operator =(const Card& c);

protected:
	Type _type;
	int _num; //本卡牌的编号（两位）
	std::string _addr;
	std::string _name;
};

class RuleCard :public Card  //规则牌
{
public:
	RuleCard(): Card(){}
	RuleCard(int num,std::string addr,std::string name) : Card(num,addr,name){}
};

class BasicRuleCard : public RuleCard //基本规则牌
{
public:
	BasicRuleCard(int num,std::string addr,std::string name) :RuleCard(num,addr,name){}
};

class NewRuleCard :public RuleCard  //新规则牌
{
public:
	NewRuleCard(int num,std::string addr,std::string name){ _type = NEW_RULE; _num = num;_addr = addr;_name = name;}
};

class KeeperCard :public Card  //所有物卡牌
{
public:
	KeeperCard(int num, std::string addr,std::string name) { _type = KEEPER; _num = num; _addr = addr;_name = name;}
};

class GoalCard :public Card  //目标牌
{
public:
	GoalCard(int num, std::string addr,std::string name) { _type = GOAL; _num = num; _addr = addr;_name = name;}
};

class ActionCard :public Card  //行动牌
{
public:
	ActionCard(int num, std::string addr,std::string name){ _type = ACTION; _num = num; _addr = addr;_name = name;}
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
	std::vector<Card*> _cards;  
	int _cardNum;
};
#endif
