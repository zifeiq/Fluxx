#include <vector>
#include <fstream>

class Card  //���ƵĻ���
{
public:
	Card() :_type(BASIC_RULE), _num(0){} //Ĭ�Ϲ��캯��
	enum Type{BASIC_RULE, NEW_RULE, KEEPER, GOAL, ACTION};
	Type getType() const{ return _type; } //���ر���������
	int getNum() const{ return _num; }  //���ر����ı���
	virtual ~Card(){}
	bool operator==(const Card& c) const;
protected:
	Type _type;
	int _num; //�����Ƶı��ţ���λ��
};

class RuleCard :public Card  //������
{
public:
	RuleCard() :Card(){}
	RuleCard(int num){ _type = NEW_RULE; _num = num; }
};

class BasicRuleCard : public RuleCard //����������
{
public:
	BasicRuleCard() :RuleCard(){}
};

class NewRuleCard :public RuleCard  //�¹�����
{
public:
	NewRuleCard(int num) :RuleCard(num){}
};

class KeeperCard :public Card  //�����￨��
{
public:
	KeeperCard(int num) { _type = KEEPER; _num = num; }
};

class GoalCard :public Card  //Ŀ����
{
public:
	GoalCard(int num) { _type = GOAL; _num = num; }
};

class ActionCard :public Card  //�ж���
{
public:
	ActionCard(int num){ _type = ACTION; _num = num; }
};

class CardLib  //�ƿ⣬singletonģʽ
{
public:
	static CardLib& getLib()
	{
		static CardLib allcards;
		return allcards;
	}
	Card& getCard(int i) const{ return *_cards[i]; }
	int getCardNum() const { return _cardNum; }

protected:
	CardLib();
	~CardLib();
	std::vector<Card*> _cards;  //������Ϊ63��, �ж������û���չ��(How????)  ���ڿɼ�
	int _cardNum;
};
