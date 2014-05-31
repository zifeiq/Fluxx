#include "cardLib.h"

CardLib::CardLib()  //暂规定个数为63个，后期需修改
{
	_cardNum = 63;
	_cards.push_back(new RuleCard); //创建基本规则牌
	for (int i = 1; i <= 21;i++)	//创建新规则牌
		_cards.push_back(new RuleCard(i));
	for (int i = 1; i <= 18;i++)//创建所有物牌
		_cards.push_back(new KeeperCard(i));
	for (int i = 1; i <= 23; i++)//创建所有物牌
		_cards.push_back(new GoalCard(i));
}

CardLib::~CardLib()
{
	for (int i = 0; i < _cardNum; i++)
		delete _cards[i];
	_cards.clear();
}