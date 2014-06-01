#ifndef FLUXXRULES_H
#define FLUXXRULES_H

#include <vector>
#include "..\cardLib_code\cardLib.h"
#define CLOCKWISE false
#define ANTTCLOCKWISE true

enum specialRules {
	none = 0, rich, pool, inflation, rotate, nohand, random, doublegoals
};
class fluxxRules {
public:
	fluxxRules ();
	void draw(int);
	void play(int);
	void discard(int);
	void cardlimitation(int);
	void keeperlimitation(int);
	void direction(bool);
	void firstgoal(const Card&);
	void secondgoal(const Card&);
	void setspecialrule();

	const int draw()const;
	const int play () const;
	const int discard() const;
	const int cardlimitation() const;
	const int keeperlimitation() const;
	const bool direction() const;
	const Card& firstgoal() const;
	const Card& secondgoal() const;
	void getspecialrule();

	~fluxxRules ();

private:
	int cntDraw;
	int cntPlay;
	int cntCardlimit;
	int cntKeeperlimit;
	bool Direction;
	Card firstGoal;
	Card secondGoal;
	std::vector<specialRules> curRules; 
};

#endif