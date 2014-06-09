#ifndef FLUXXRULES_H
#define FLUXXRULES_H

#include <vector>
#include "..\cardLib_code\cardLib.h"
#define CLOCKWISE false
#define ANTTCLOCKWISE true

class fluxxRules {
public:
	fluxxRules (CardLib&);
	//设定参数
	void setdraw(int i) { cntDraw = i; }
	void setplay(int i) { cntPlay = i; }
	void sethandlimitation(int i) { cntHandlimit = i; }
	void setkeeperlimitation(int i) { cntKeeperlimit = i; }
	void setorderreverse(bool b) { orderReverse = b; }
	void setdoublegoals(bool b) { doubleGoals = b; }
	void setrichbonus(bool b) { richBonus = b; }
	void setpoorbonus(bool b) { poorBonus = b; }
	void setinflation(bool b) { Inflation = b; }
	void setnohandbonus(bool b) { noHandbonus = b; }
	void setrandomstart(bool b) { randomStart = b; }
	//设定有关卡牌
	void setdrawrule(const Card& c){ DrawRule = c; }
	void setplayrule(const Card& c) { PlayRule = c; }
	void sethandlimrule(const Card& c){ HandlimitRule = c; }
	void setkeeperlimrule(const Card& c) { KeeperlimitRule = c; }
	void setfirstgoal(const Card& c) { firstGoal = c; }
	void setsecondgoal(const Card& c) { secondGoal = c; }
	//获取参数
	const int getdraw()const { return cntDraw; }
	const int getplay () const {return cntPlay; }
	const unsigned int gethandlimitation() const { return cntHandlimit; }
	const unsigned int getkeeperlimitation() const {return cntKeeperlimit; }
	const bool isorderreverse() const{ return orderReverse; }
	const bool isdoublegoals() const {return doubleGoals;}
	const bool isrichBonus() const { return richBonus; }
	const bool ispoorBonus() const { return poorBonus; }
	const bool isinflation() const { return Inflation; }
	const bool isnohandbonus() const { return noHandbonus; }
	const bool israndomstart() const { return randomStart; }
	//获取卡牌
	const Card& firstgoal() const { return firstGoal; }
	const Card& secondgoal() const { return secondGoal; }
	const Card& getdrawrule() const { return DrawRule; }
	const Card& getplayrule() const { return PlayRule; }
	const Card& getCdoublegoals() const { return c_doubleGoals; }
	const Card& getChandlimit() const { return HandlimitRule; }
	const Card& getCkeeperlimit() const { return KeeperlimitRule; }
	const Card& getCorderreverse() const { return c_orderReverse; }
	const Card& getCrichbonus() const { return c_richbonus; }
	const Card& getCpoorbonus() const { return c_poorBonus; }
	const Card& getCinflation() const { return c_Inflation; }
	const Card& getCnohandbonus() const { return c_noHandbonus; }
	const Card& getCrandomstart() const { return c_Randomstart; }


	
	~fluxxRules (){
	//没有申请过空间，使用自动生成的析构函数
	};

private:
	//规则的标志位和对应的卡牌
	int cntDraw;//摸牌数
	Card& DrawRule;
	int cntPlay;//出牌数
	Card& PlayRule;
	int cntHandlimit;//手牌上限
	Card& HandlimitRule;
	int cntKeeperlimit;//所有物上限
	Card& KeeperlimitRule;
	bool orderReverse;//出牌方向
	Card& c_orderReverse;
	bool doubleGoals;//双重目标
	Card& c_doubleGoals;
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
	Card& c_Randomstart;
	CardLib& r_Cardlib;
};

#endif