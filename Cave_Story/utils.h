#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <algorithm>

class Utils {
public:
	//Split - Splits a string <txt> everywhere a certain character <ch> is found.
	//Stores resulting substring inside vector <strs>
	//Returns vector size
	static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch) {
		int pos = txt.find(ch);
		int initialPos = 0;
		strs.clear();
		//While pos is not npos, meaning the position of the next ch still exists
		while (pos != std::string::npos) {
			strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}
		//Add the last section after that final occurrence of 'ch'.
		strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));

		return strs.size();
	}
};

#endif