#include <sstream>
#include "cardLib.h"

bool Card::operator==(const Card& c) const
{
	return((_type == c.getType()) && (_num == c.getNum()));
}

Card& Card::operator=(const Card& c)
{
	_type = c.getType();
	_num = c.getNum();
	return *this;
}

CardLib::CardLib()  //暂规定个数为63个，后期需修改
{
	int num_tmp;
	std::ifstream carddata(FILENAME);
	if(carddata)
	{
		for(_cardNum=0;!carddata.eof();_cardNum++)
		{
			carddata >> num_tmp;
			std::string s;
			std::stringstream ss;
			ss << num_tmp;
			ss >> s;
			if(num_tmp/100==0)
			{
				_cards.push_back((Card*)new BasicRuleCard);
			}
			else if(num_tmp/100==1)
			{
				_cards.push_back((Card*)new NewRuleCard(num_tmp-100));
			}
			else if(num_tmp/100==2)
			{
				_cards.push_back((Card*)new KeeperCard(num_tmp-200));
			}
			else if(num_tmp/100==3)
			{
				_cards.push_back((Card*)new GoalCard(num_tmp-300));
			}
			_cards.back()->setPixmap(s + ".png");
		}
		carddata.close();
	}
	else
		_cardNum=0;
}

CardLib::~CardLib()
{
	for (int i = 0; i < _cardNum; i++)
		delete _cards[i];
	_cards.clear();
}
