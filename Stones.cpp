#include "Stones.h"
#include <iostream>
#include <algorithm>

//definition of constructor of class Stones
Stones::Stones(void)
{
	const short int MAX_FREE_STONES = 16;
	freeStones.resize(MAX_FREE_STONES);

	//generator of all four dimension binary numbers <0000 - 1111>
	for (int i = 0; i < MAX_FREE_STONES; ++i)
		freeStones.at(i) = std::bitset<4>(i).to_string();
}

void Stones::printFreeStones(void)
{
	std::cout << "Free stones are:" << std::endl;
	for (int i = 0; i < freeStones.size(); ++i)
		std::cout << freeStones.at(i) << "\t";

	std::cout << std::endl;
}

bool Stones::checkIfFreeStone(const std::string &chosenStone)
{
	for (int i = 0; i < freeStones.size(); ++i)
	{
		if (freeStones.at(i) == chosenStone)
			return true;
	}	

	return false;
}

void Stones::reduceFreeStones(const std::string &chosenStone)
{
	//remove chosen stone from vector of freeStones via std::remove from <algorithm>
	freeStones.erase(std::remove(freeStones.begin(), freeStones.end(), chosenStone), freeStones.end());
}

std::string Stones::generateRandomStone(void)
{
	srand((int)time(0));
	int randomNumber = (rand() % freeStones.size());

	return freeStones.at(randomNumber);
}
