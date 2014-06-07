#ifndef FLUXXRULES_H
#define FLUXXRULES_H

#include <vector>
#include "..\cardLib_code\cardLib.h"
//#define CLOCKWISE false
//#define ANTTCLOCKWISE true

class fluxxRules {
public:
	fluxxRules ();
	//设定参数
	void setdraw(int i) { cntDraw = i; }
	void setplay(int i) { cntPlay = i; }
	void setcardlimitation(int i) { cntCardlimit = i; }
	void setkeeperlimitation(int i) { cntKeeperlimit = i; }
	void setorderreverse(bool b) { orderReverse = b; }
	void setrichbonus(bool b) { richBonus = b; }
	void setpoorbonus(bool b) { poorBonus = b; }
	void setinflation(bool b) { Inflation = b; }
	void setnohandbonus(bool b) { noHandbonus = b; }
	void setrandomstart(bool b) { randomStart = b; }
	//设定有关卡牌
	void setdrawrule(const Card&);
	void setplayrule(const Card&);
	void sethandlimrule(const Card&);
	void setkeeperlimrule(const Card&);
	void setfirstgoal(const GoalCard&);
	void setsecondgoal(const GoalCard&);
	//获取参数
	const int getdraw()const;
	const int getplay () const;
	const int getcardlimitation() const;
	const int getkeeperlimitation() const;
	const bool isorderreverse() const{ return orderReverse; }
	const bool isdoublegoals() const {return doubleGoles;}
	const bool isrichBonus() const { return richBonus; }
	const bool ispoorBonus() const { return poorBonus; }
	const bool isinflation() const { return Inflation; }
	const bool isnohandbonus() const { return noHandbonus; }
	const bool israndomstart() const { return randomStart; }
	//获取卡牌
	const Card& firstgoal() const;
	const Card& secondgoal() const;
	const std::vector<RuleCard&>& getrulecard();
	
	~fluxxRules ();

private:
	//规则的标志位和对应的卡牌
	int cntDraw;//摸牌数
	Card& DrawRule;
	int cntPlay;//出牌数
	Card& PlayRule;
	int cntCardlimit;//手牌上限
	Card& CardlimitRule;
	int cntKeeperlimit;//所有物上限
	Card& Cardlimiton;
	bool orderReverse;//出牌方向
	Card& c_orderReverse;
	bool doubleGoles;//双重目标
	Card& firstGoal;
	Card& secondGoal;
	bool richBonus;//富人奖励
	Card& c_richbonus;
	bool poorBonus;//穷人奖励
	Card& c_poorBonus;
	bool Inflation;//通货膨胀
	Card& c_Inflation;
	bool noHandbonus;//无手牌奖励
	Card& c_noHandbonus;
	bool randomStart;//随机开场
	Card& c_noRandomstart;
};

#endif