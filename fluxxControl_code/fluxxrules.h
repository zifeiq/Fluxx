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

	const int draw()const;
	const int play () const;
	const int cardlimitation() const;
	const int keeperlimitation() const;
	const bool direction() const;
	const Card& firstgoal() const;
	const Card& secondgoal() const;
	const std::vector<RuleCard&>& getrulecard();
	
	~fluxxRules ();

private:
	int cntDraw;//摸牌数
	int cntPlay;//出牌数
	int cntCardlimit;//手牌上限
	int cntKeeperlimit;//所有物上限
	bool Direction;//出牌方向
	Card& firstGoal;
	Card& secondGoal;
	std::vector<RuleCard&> rulecards;//给gui提供方便
	std::vector<specialRules> curRules; 
};

#endif