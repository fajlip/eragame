#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <vector>
#include <string>

#include "eraGame.h"

class Board
{
private:
	//useful for AI generator
	std::vector<std::string> freeFields;

	//Board size was specified by assignment to 4x4 and cannot be changed
	short int const boardSize = 4;//

	//this method is called by checkForWinners
	void compareBinaryStrings(const std::string &, const std::string &, std::vector<int> &);


public:
	std::vector<std::vector<std::string> > gameBoard; //pote dat private
	//this method is called by gameStatus
	bool checkForWinners(const std::string &);

	short int static const fieldStringSize = 2;

	Board(void);

	void gameStatus(const std::string &, PlayersEnum);

	//this method is called by compareBinaryStrings
	void setMaskVectors(std::vector<std::vector<int> > &);
	void clearEmptyMaskVectors(std::vector<std::vector<int> > &);
	bool isBoardFull(void);
	void printBoard(void);
	bool checkIfFieldFree(const std::string &);
	void placeStone(const std::string, const std::string);
	std::string generateRandomField(void);
	void reduceFreeFields(const std::string);
};

#endif