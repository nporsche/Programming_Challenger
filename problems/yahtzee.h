#include <iostream>
#include <sstream>
#include <numeric>
#include <stack>
using namespace std;

const int COMBINATION_COUNT = 8192;//1 << 13;

enum cardtype
{
    numbers_of_1 = 1,
    numbers_of_2,
    numbers_of_3,
    numbers_of_4,
    numbers_of_5,
    numbers_of_6,
    chance,
    three_kind,
    four_kind,
    all_kind,
    low_sequence,
    high_sequence,
    full_house
};

int number_count(int* p, int x)
{
    int count = 0;

    for (int i = 0; i< 5; ++i)
    {
        if(p[i] == x)
            ++count;
    }
    
    return count;
    
}

int getScore(int* p, cardtype t)
{
    sort(p, p + 5);
    int score = 0;
    if(t <= 6)
    {
        score = (t) * number_count(p, t);
    }
    switch (t) {
        case chance:
            score = accumulate(p, p + 5, 0);
            break;
        case three_kind:
            {
                bool r = false;
                for(int i = 0; i < 3;++i)
                {
                    r = (p[i] == p[i + 2]);
                    if(r)
                    {
                        score = accumulate(p, p + 5, 0);
                        break;
                    }
                }
            }
            break;
        case four_kind:
            {
                bool r = false;
                for(int i = 0; i < 2;++i)
                {
                    r = (p[i] == p[i + 3]);
                    if(r)
                    {
                        score = accumulate(p, p + 5, 0);
                        break;
                    }
                }
            }
            break;
            
        case all_kind:
            {
                if(p[0] == p[4])
                    score = 50;
            }
            break;
        case low_sequence:
            {
                bool r = false;
                for(int i = 0; i < 2;++i)
                {
                    r = (p[i + 3] - p[i]  == 3);
                    if(r)
                    {
                        score = 25;
                        break;
                    }
                }
            }
            break;
        case high_sequence:
            {
                if(p[4] - p[0] == 4)
                    score = 35;
            }
            break;
        case full_house:
            {
                bool r = false;
                for(int i = 0; i < 4;++i)
                {
                    r = (p[i] == p[i + 1]);
                    if(r && getScore(p, three_kind))
                    {
                        score = 40;
                    }
                }

            }
            break;
        default:
            break;
    }
    
    return score;
}

struct slot
{
    slot()
    {
        cur_score = -1;
        total = -1;
    }
    int cur_score;
    int cur_round;
    int total;
};

int bit_1_count(int number)
{
    int count = 0;
    while(number)
    {
        if(number % 2 == 1)
            ++count;
        
        number = number >> 1;
    }
    
    return  count;
}

void process_max_point(int inputs[][5])
{
    int score_mem[14][14];  //score_mem[i][j] i种计分方式用在第j组骰子上
    for(int i = 1; i<= 13;++i)
    {
        for(int j = 1; j<= 13; ++j)
        {
            score_mem[i][j] = getScore(inputs[j], (cardtype)i);
        }
    }
    
    //s[i][j] 前i种计分使用 j 的骰子组合的最大分数， j时一个13位的二进制数，每一个位代表组合是否被使用，
    //1 代表已经被使用，0代表该组骰子没有用
    slot s[14][COMBINATION_COUNT];
    for (int i = 1; i <= 13; ++i)
    {
        for(int x = 1; x <= 13; ++x)    //第i种计分方式用在第x轮上
        {
            int bit_1_in_position_x = 1 << (x-1);
            if(i == 1)
            {
                s[i][bit_1_in_position_x].total = score_mem[i][x];
                s[i][bit_1_in_position_x].cur_score = score_mem[i][x];
                s[i][bit_1_in_position_x].cur_round = bit_1_in_position_x;
            }
            else
            {
                //寻找前i-1种计分方式的所有的combination
                for(int sub_j = 0; sub_j < COMBINATION_COUNT; ++sub_j)
                {
                    if(i - 1 != bit_1_count(sub_j)) //i-1种计分方式种，combination中使用的组合bit数必须为i-1
                        continue;
                    if((sub_j & bit_1_in_position_x) > 0) //准备用的x轮在已经被使用，所以跳过
                        continue;
                    
                    int combination_new = (sub_j | bit_1_in_position_x);
                    
                    int total_score_in_i = s[i-1][sub_j].total + score_mem[i][x];
                    
                    if(i == 7 && s[i-1][sub_j].total >= 63)
                        total_score_in_i += 35;
                        
                    if(s[i][combination_new].total < total_score_in_i)
                    {
                        s[i][combination_new].total = total_score_in_i;
                        s[i][combination_new].cur_score = score_mem[i][x];
                        s[i][combination_new].cur_round = bit_1_in_position_x;
                    }
                }
            }
        }
    }
    int combination = COMBINATION_COUNT-1;
    int combination_in_6;
    stack<int> score_stack;
    for (int i = 13; i >= 1; --i)
    {
        if(i == 6)
            combination_in_6 = combination;
        
        score_stack.push(s[i][combination].cur_score);
        combination = ((~s[i][combination].cur_round) & combination);
    }
    
    while(!score_stack.empty())
    {
        cout << score_stack.top() << " ";
        score_stack.pop();
    }
    
    if(s[6][combination_in_6].total >= 63)
    {
        cout << 35 << " ";
    }
    else
        cout << 0 << " ";
    
    cout << s[13][COMBINATION_COUNT-1].total << endl;
}

int main()
{
    int points[14][5];  //start from [1][..]
    cardtype ct[14]; //start from [1]
    for(int i = 1; i <= 13; ++i)
    {
        ct[i] = (cardtype)i;
    }
    
    string line;
    int i = 1;
    while(getline(cin, line))
    {
        istringstream iss(line);
        for(int j = 0; j < 5;++j)
        {
            iss >> points[i][j];
        }
        
        if(i == 13) //points has been filled completely
        {
            i = 1;
            process_max_point(points);  //from 1 - 13
        }
        else
            ++i;
    }
    
    return 0;
}


