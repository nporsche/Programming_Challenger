#include <iostream>
#include <vector>
#include <algorithm>  
#include <cmath>

using namespace std;

int NCARDS = 52;	//number of cards
int NSUITS = 4;	//number of suits
int VALUE_COUNT = NCARDS/NSUITS;
char values[] = "23456789TJQKA";
char suits[] = "CDHS";
const int BASE = 15;

class cardsType
{
public:
	enum e 
	{
		normal = 0,
		pair = 1,
		two_pair = 2,
		three_kind = 3,
		straight = 4,	//顺子
		flush = 5,	//同花
		full_house = 6,	//葫芦 3带2
		four_kind = 7, //炸弹
		straight_flush = 8	//同花顺
	};
};

class pokor_card
{
public:
	pokor_card(char v, char s)
	{
		m_value = v;
		m_suit = s;
	}

	int getValue() const
	{
		int v = 0;
		for(int i = 0; i < VALUE_COUNT; ++i)
		{
			if(values[i] == m_value)
			{
				v = i + 2;
				break;
			}
		}

		return v;
	}

	char getSuit() const
	{
		return m_suit;
	}

	bool operator > (const pokor_card& p) const
	{
		return getValue() > p.getValue();
	}
	
	bool operator < (const pokor_card& p) const
	{
		return getValue() < p.getValue();
	}

private:
	char m_value;
	char m_suit;
};

long long  TryScore(vector<pokor_card>& cards, int t)
{
	long long score = 0;

	switch(t)
	{
		case cardsType::straight_flush:
			{
				if(TryScore(cards, cardsType::straight) && TryScore(cards, cardsType::flush))
					score += TryScore(cards, cardsType::normal);
			}
			break;
		case cardsType::four_kind:
			{
				for(int i = 0; i < cards.size() - 3;++i)
				{
					if(cards[i].getValue() == cards[i + 3].getValue())
					{
						score += cards[i].getValue() * pow(BASE, 4);
						score += cards[i].getValue() * pow(BASE, 3);
						score += cards[i].getValue() * pow(BASE, 2);
						score += cards[i].getValue() * pow(BASE, 2);
					}
				}
			}
			break;
		case cardsType::full_house:
			{
				vector<pokor_card> remained(cards.begin(), cards.end());
				for(int i = 0; i < cards.size() - 2; ++i)
				{
					if(cards[i].getValue() == cards[i+2].getValue())
					{
						remained.erase(remained.begin() + i, remained.begin() + i+2);
						if(TryScore(remained, cardsType::pair))
						{
							score += TryScore(cards, cardsType::three_kind);
						}
					}	
				}

			}
			break;
		case cardsType::flush:
			{
				char suit = cards[0].getSuit();
				bool bFlush = true;
				for(int i = 1; i < cards.size(); ++i)
				{
					if(suit != cards[i].getSuit())
					{
						bFlush = false;
						break;
					}
				}
				if(bFlush)
					score += TryScore(cards, cardsType::normal);
			}
			break;
		case cardsType::straight:
			{
				bool bstraight = true;
				for(int i = 0; i < cards.size() - 1; ++i)
				{
					if(cards[i].getValue()-1 != cards[i+1].getValue())
					{
						bstraight = false;
						break;
					}
				}
				if(bstraight)
				{
					score += cards[0].getValue()*pow(BASE, 4);
				}
			}
			break;
		case cardsType::three_kind:
			{
				for(int i = 0; i < cards.size() - 2; ++i)
				{
					if(cards[i].getValue() == cards[i+2].getValue())
					{
						score += cards[i].getValue() * pow(BASE, 4);
						score += cards[i].getValue() * pow(BASE, 3);
						score += cards[i].getValue() * pow(BASE, 2);
					}	
				}
			}
			break;
		case cardsType::two_pair:
			{
				vector<pokor_card> remained(cards.begin(), cards.end());

				for(int i = 0; i < cards.size() - 1; ++i)
				{
					if(cards[i].getValue() == cards[i+1].getValue())
					{
						remained.erase(remained.begin() + i, remained.begin() + i+1);					
						score += cards[i].getValue() * pow(BASE, 4);
						score += cards[i].getValue() * pow(BASE, 3);
						score += TryScore(remained, cardsType::pair);
					}
				}
			}
			break;
		case cardsType::pair:
			{
				vector<pokor_card> remained(cards.begin(), cards.end());
				for(int i = 0; i < cards.size() - 1; ++i)
				{
					if(cards[i].getValue() == cards[i+1].getValue())
					{
						remained.erase(remained.begin() + i,remained.begin() + i + 1);
						score += cards[i].getValue() * pow(BASE, 4);
						score += cards[i].getValue() * pow(BASE, 3);
						score += TryScore(remained, cardsType::normal);
					}
					
				}
			}
			break;
		case cardsType::normal:
			{
				for(int i = 0; i < cards.size();++i)
				{
					score += cards[i].getValue() * pow(BASE, cards.size() - i - 1);
				}
			}	
			break;
	}

	return score;
}

long long  getScore(vector<pokor_card>& cards)
{
	sort(cards.begin(),cards.end(), greater<pokor_card>());
	//get cards type first
	for(int t = 8; t>=0; --t)
	{
		long long  score = TryScore(cards, t);
		if(score)
		{
			return score * pow(BASE, t);
		}
	}

	return 0;	//should never arrive here
}

long long Judge(vector<pokor_card>& black, vector<pokor_card>& white)	// >0 means black wins, <0 means white wins, 0 means tie 
{
	long long  blackScore = getScore(black);
	long long  whiteScore = getScore(white);


	return blackScore - whiteScore;
}

int main()
{
	string line;
	while(getline(cin,line))
	{
		vector<pokor_card> black;
		vector<pokor_card> white;
		for(int i = 0; i < line.length(); i=i+3)
		{
			if(black.size() < 5)
				black.push_back(pokor_card(line[i], line[i+1]));
			else
				white.push_back(pokor_card(line[i], line[i+1]));
		}
		long long  result = Judge(black, white);
		if(result == 0)	//equal
		{
			cout << "Tie." << endl;
		}
		else if(result > 0)
			cout << "Black wins." << endl;
		else if(result < 0)
			cout << "White wins." << endl;
	}
	return 0;
}
