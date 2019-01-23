// Example program
#pragma once
#include <iostream>

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;


class String{
    public:
    // Usage String::Split("a", "hola chao"); -> [hola, chao]
	static vector<string> Split(string delim, string str)
	{
        vector<string> tokens;
        size_t prev = 0, pos = 0;
        do
        {
            pos = str.find(delim, prev);
            if (pos == string::npos) {pos = str.length();}
            string token = str.substr(prev, pos-prev);
            if (!token.empty()) {tokens.push_back(token);}
            prev = pos + delim.length();
        }
        while (pos < str.length() && prev < str.length());
        return tokens;
	}


	// trim from start (in place)
	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}
	static bool EndsWith(std::string const &fullString, std::string const &ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		}
		else {
			return false;
		}
	}
};

class Math {
public:
	template <typename T>
	static float Average(std::initializer_list<T> nums) {
		float _re;

		for (const val : nums) {
			_re += val;
		}

		return _re/nums.size();
	}
};