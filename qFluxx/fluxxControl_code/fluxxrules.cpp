#include "fluxxrules.h"

fluxxRules::fluxxRules(CardLib& r)
	: cntDraw(1),
	cntPlay(1),
	cntHandlimit(-1), 
	cntKeeperlimit(-1), 
	orderReverse(CLOCKWISE),
	doubleGoals(false),
	richBonus(false),
	poorBonus(false),
	Inflation(false),
	noHandbonus(false),
	randomStart(false),
	r_Cardlib(r),
	DrawRule(*r.getCard(0)),
	PlayRule(*r.getCard(0)),
	HandlimitRule(*r.getCard(0)),
	KeeperlimitRule(*r.getCard(0)),
	firstGoal(*r.getCard(0)),
	secondGoal(*r.getCard(0)),
	c_richbonus(*r.getCard(14)),
	c_poorBonus(*r.getCard(15)),
	c_Inflation(*r.getCard(16)),
	c_orderReverse(*r.getCard(17)),
	c_noHandbonus(*r.getCard(18)),
	c_Randomstart(*r.getCard(19)),
	c_doubleGoals(*r.getCard(20))
	{
		// cntDraw=1;
		// cntPlay=1;
		// cntHandlimit=-1; 
		// cntKeeperlimit=-1; 
		// orderReverse=CLOCKWISE;
		// doubleGoals=false;
		// richBonus=false;
		// poorBonus=false;
		// Inflation=false;
		// noHandbonus=false;
		// randomStart=false;
		// r_Cardlib = CardLib::getLib();
		// DrawRule=*r.getCard(0);
		// PlayRule=*r.getCard(0);
		// HandlimitRule=*r.getCard(0);
		// KeeperlimitRule=*r.getCard(0);
		// firstGoal=*r.getCard(0);
		// secondGoal=*r.getCard(0);
		// c_richbonus=*r.getCard(0);
		// c_poorBonus=*r.getCard(0);
		// c_Inflation=*r.getCard(0);
		// c_orderReverse=*r.getCard(0);
		// c_noHandbonus=*r.getCard(0);
		// c_Randomstart=*r.getCard(0);
		// c_doubleGoals=*r.getCard(0);		
}

