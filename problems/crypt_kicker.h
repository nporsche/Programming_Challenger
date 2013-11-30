

/*
 PC/UVa IDs: 110204/843, Popularity: B, Success rate: low Level: 2
 A common but insecure method of encrypting text is to permute the letters of the alphabet.
 In other words, each letter of the alphabet is consistently replaced in the text by some other letter.
 To ensure that the encryption is reversible, no two letters are replaced by the same letter.
 Your task is to decrypt several encoded lines of text, assuming that each line uses a different set of replacements,
 and that all words in the decrypted text are from a dictionary of known words.
 */
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
using namespace std;

bool comp(const string& s1, const string& s2)
{
	return s1.length() < s2.length();
}

vector<string> parse_to_words(const string& line)
{
	istringstream iss(line);
    string w;
    bool found;
    vector<string> word;
    while (iss >> w)
    {
        found = false;
        for (int i = 0; i < word.size(); i++)
            if (w == word[i])
                found = true;
        if (!found)
            word.push_back(w);
    }
    
    return word;
}

bool process(const vector<string>& dic, vector<string> pending, map<char,char>& words_mapping)
{
	if(pending.empty())
		return true;
    
	//////
	string pending_word = pending.back();
	pending.pop_back();
	for(int j = dic.size()-1; j>= 0; --j)
	{
		string dic_word = dic[j];
		if(dic_word.length() == pending_word.length())
		{
			//check
			bool valid = true;
			map<char,char> tmp_words_mapping(words_mapping);
			vector<string> tmp_dic(dic);
			for(int x = 0; x <= pending_word.length(); ++x)
			{
				map<char,char>::iterator it = words_mapping.find(pending_word[x]);
				if(it != words_mapping.end() && dic_word[x] != it->second)
				{
					valid = false;
					break;
				}
				else
				{
					tmp_words_mapping[pending_word[x]] = dic_word[x];
				}
			}
			if(valid)	//this word is valid
			{
				tmp_dic.erase(tmp_dic.begin() + j);
				if(process(tmp_dic, pending, tmp_words_mapping))
				{
					words_mapping = tmp_words_mapping;
					return true;
				}
			}
		}
	}
    
	return false;
}

int main(int argc, const char * argv[])
{
   	int dic_word_count = 0;
	cin >> dic_word_count;
	vector<string> dictionary;
	for(int i = 0; i < dic_word_count; ++i)
	{
		string word;
		cin >> word;
		dictionary.push_back(word);
	}
	sort(dictionary.begin(), dictionary.end(), comp);
    
	string line;
	while(getline(cin, line))
	{
        if (line.length() == 0)
        {
            cout << endl;
            continue;
        }
		vector<string> pending_for_decipher = parse_to_words(line);
		sort(pending_for_decipher.begin(), pending_for_decipher.end(), comp);
		map<char,char> words_mapping;
		bool b = process(dictionary, pending_for_decipher, words_mapping);
		for(int i = 0; i < line.length(); ++i)
		{
			if(line[i] == ' ')
				cout << ' ';
			else
			{
				if(b)
					cout << words_mapping[line[i]];
				else
					cout << '*';
			}
		}
		cout << endl;
	}
}


