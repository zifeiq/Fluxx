#include "cardLib.h"
using namespace std;
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
			if(num_tmp/100==0)
			{
				_cards.push_back((Card*)new BasicRuleCard(num_tmp));
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
			else if (num_tmp/100 == 4)
			{
				_cards.push_back((Card*)new ActionCard(num_tmp - 400));
			}
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

void CardLib::getInfo(const Card* goal, vector<const Card *>& keepers) const
{
	keepers.clear();
	switch (goal->getNum())
	{
	case 1: //嬉皮士：和平（214） + 爱（201）
		keepers.push_back(getCard(36));
		keepers.push_back(getCard(23));
		break;
	case 2: //牛奶与饼干：牛奶（203） + 饼干（215）
		keepers.push_back(getCard(25));
		keepers.push_back(getCard(37));
		break;
	case 3: //心与智：爱（201） + 头脑（218）
		keepers.push_back(getCard(23));
		keepers.push_back(getCard(40));
		break;
	case 4: //烤面包片：面包（213） + 烤面包机（204）
		keepers.push_back(getCard(35));
		keepers.push_back(getCard(26));
		break;
	case 5://巧克力奶： 巧克力（210） + 牛奶（203）
		keepers.push_back(getCard(32));
		keepers.push_back(getCard(25));
		break;
	case 6://烤制食品：面包（213） + 饼干（215）
		keepers.push_back(getCard(35));
		keepers.push_back(getCard(37));
		break;
	case 7://晒化的巧克力：太阳（206） + 巧克力（210）
		keepers.push_back(getCard(28));
		keepers.push_back(getCard(32));
		break;
	case 8://登月火箭：火箭（207） + 月亮（208）
		keepers.push_back(getCard(29));
		keepers.push_back(getCard(30));
		break;
	case 9://时间就是金钱：时间（202） + 金钱（205）
		keepers.push_back(getCard(24));
		keepers.push_back(getCard(27));
		break;
	case 10://就寝时间：睡眠（216） + 时间（202）
		keepers.push_back(getCard(38));
		keepers.push_back(getCard(24));
		break;
	case 11://家用电器：电视（211） + 烤面包机（204）
		keepers.push_back(getCard(33));
		keepers.push_back(getCard(26));
		break;
	case 12://梦乡：睡眠（216） + 梦（212）
		keepers.push_back(getCard(38));
		keepers.push_back(getCard(34));
		break;
	case 13://巧克力饼干：巧克力（210） + 饼干（215）
		keepers.push_back(getCard(32));
		keepers.push_back(getCard(37));
		break;
	case 14://战争就是死亡：战争（217） + 死亡（209）
		keepers.push_back(getCard(39));
		keepers.push_back(getCard(31));
		break;
	case 15://彩票中奖：梦（212） + 金钱（205）
		keepers.push_back(getCard(34));
		keepers.push_back(getCard(27));
		break;
	case 16://甜蜜的死亡：死亡（209） + 巧克力（210）
		keepers.push_back(getCard(31));
		keepers.push_back(getCard(32));
		break;
	case 17://日与夜：月亮（208） + 太阳（206）
		keepers.push_back(getCard(30));
		keepers.push_back(getCard(28));
		break;
	case 18://火箭科技：火箭（207） + 头脑（218）
		keepers.push_back(getCard(29));
		keepers.push_back(getCard(40));
		break;
	case 19://只需要爱：爱（201） 且无其他所有
		keepers.push_back(getCard(23));
		break;
	case 20://和平（无战争）：和平（214） 且无 战争（217）
		keepers.push_back(getCard(36));
		keepers.push_back(getCard(39));
		break;
	case 21://头脑（无电视）：头脑（218） 且无 电视（211）
		keepers.push_back(getCard(40));
		keepers.push_back(getCard(33));
		break;
	default:
		break;
	}
}