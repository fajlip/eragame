#ifndef STONES_H
#define STONES_H

#include <bitset>
#include <ctime> //for srand
#include <vector>
#include <string>

class Stones
{
private:
	std::vector<std::string> freeStones;

public:
	short int static const stoneStringSize = 4;

	Stones(void);

	bool checkIfFreeStone(const std::string &);
	void reduceFreeStones(const std::string &);
	void printFreeStones(void);
	std::string generateRandomStone(void);
};

#endif
