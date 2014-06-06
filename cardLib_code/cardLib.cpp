<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#include "..\cardLib_code\cardLib.h"
#include <sstream>
=======
=======
>>>>>>> FETCH_HEAD
=======
>>>>>>> FETCH_HEAD
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
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> FETCH_HEAD
=======
>>>>>>> FETCH_HEAD
=======
>>>>>>> FETCH_HEAD

CardLib::CardLib()  //暂规定个数为63个，后期需修改
{
	int num_tmp;
	std::ifstream carddata(FILENAME);
	if(carddata)
	{
		for(_cardNum=0;!carddata.eof();_cardNum++)
		{
			carddata >> num_tmp;
			string s;
			stringstream ss;
			ss << num_tmp;
			ss >> s;
			if(num_tmp/100==0)
			{
				_cards.push_back((Card*)new BasicRuleCard);
			}
			else if(num_tmp/100==1)
			{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
				_cards.push_back((Card*)new NewRuleCard(num_tmp));
				_cards.[_cards.size()].pixmapName = num_tmp.
=======
				_cards.push_back((Card*)new NewRuleCard(num_tmp-100));
>>>>>>> FETCH_HEAD
=======
				_cards.push_back((Card*)new NewRuleCard(num_tmp-100));
>>>>>>> FETCH_HEAD
=======
				_cards.push_back((Card*)new NewRuleCard(num_tmp-100));
>>>>>>> FETCH_HEAD
			}
			else if(num_tmp/100==2)
			{
				_cards.push_back((Card*)new KeeperCard(num_tmp-200));
			}
			else if(num_tmp/100==3)
			{
				_cards.push_back((Card*)new GoalCard(num_tmp-300));
			}
			_cards.back()->pixmapName = s + ".png" ;
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
