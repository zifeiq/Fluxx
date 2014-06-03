#ifndef FLUXXRULES_H
#define FLUXXRULES_H

#include <vector>
#include "..\cardLib_code\cardLib.h"
#define CLOCKWISE false
#define ANTTCLOCKWISE true

enum specialRules {
	NONE = 0, RICH, POOR, INFLATION, ROTATE, NOHAND, RANDOM, DOUBLEGOALS
};
class fluxxRules {
public:
	fluxxRules ();

	void draw(int);
	void play(int);
	void cardlimitation(int);
	void keeperlimitation(int);
	void direction(bool);
	void firstgoal(const GoalCard&);
	void secondgoal(const GoalCard&);
	void setspecialrule();
	void addrule(const RuleCard&);
	void removerule(const RuleCard&);
	void getspecialrule();
	void setdrawrule(const Card&);
	void setplayrule(const Card&);
	void sethandlimrule(const Card&);
	void setkeeperlimrule(const Card&);

	const int draw()const;
	const int play () const;
	const int cardlimitation() const;
	const int keeperlimitation() const;
	const bool direction() const;
	const bool isdoublegoals() const {return doubleGoles;}
	const Card& firstgoal() const;
	const Card& secondgoal() const;
	const std::vector<RuleCard&>& getrulecard();
	
	~fluxxRules ();

private:
	int cntDraw;//摸牌数
	Card& DrawRule;
	int cntPlay;//出牌数
	Card& PlayRule;
	int cntCardlimit;//手牌上限
	Card& CardlimitRule;
	int cntKeeperlimit;//所有物上限
	Card& Cardlimiton
	bool Direction;//出牌方向
	bool doubleGoles;
	Card& firstGoal;
	Card& secondGoal;
	//std::vector<RuleCard&> rulecards;//给gui提供方便
	std::vector<specialRules> curRules; 
};

#endif