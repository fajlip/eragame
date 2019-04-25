#include "Board.h"
#include "Stones.h"

#include <iostream>

//constructor for class Board
Board::Board(void)
{
	//creating 2D vector of [boardSize x boardSize]
	gameBoard.resize(boardSize);
	for (short int i = 0; i < boardSize; ++i)
		gameBoard[i].resize(boardSize);

	//initialization to default values for gameBoard vector and freeFields vector
	for (short int row = 0; row < boardSize; ++row)
	{
		for (short int col = 0; col < boardSize; ++col)
		{
			//board is filled with "\0" - end of string sequence
			gameBoard[row][col] = "\0";
			//indexes are for user different than for program
			freeFields.push_back(std::to_string(row + 1) + std::to_string(col + 1)); 
		}
	}

}

void Board::printBoard(void)
{
	std::cout << "                                 " << std::endl;
	std::cout << "      1      2      3      4     " << std::endl;
	std::cout << "   +---------------------------+ " << std::endl;
	
	bool firstStonePart;

	for (short int row = 0; row < boardSize; ++row)
	{
		//row names start at 1
		std::cout << " " << row + 1 << " |"; 

		firstStonePart = true;

		for (short int col = 0; col < boardSize; ++col)
		{
			//indexes in array gameBoard starts at 0 as usual; thats why row-1 resp. col-1
			//dividing data string to two logical rows but still contained in one table row
			//format of stone is:	00
			//						00
			if (firstStonePart)
			{
				//if cell on gameBoard does not hold stone, print spaces(nothing)
				if (gameBoard[row][col].empty())
					std::cout << "    ";
				else
					std::cout << "  " << gameBoard[row][col].substr(0,2);
				std::cout << "  |";

				//we are on last col of first stone logical row
				//now lets repeat the process for second logical row
				if (col == boardSize - 1)
				{
					//reset col counter, will start counting next loop at 0 again
					col = -1;

					firstStonePart = false;
					//end of first logical row
					std::cout << std::endl << "   |"; 
				}
			}
			else
			{
				//if cell on gameBoard does not hold stone, print spaces(nothing)
				if (gameBoard[row][col].empty())
					std:: cout << "    ";
				else
					std::cout << "  " << gameBoard[row][col].substr(2,4);
				std::cout << "  |";
			}

		}

		std::cout << std::endl << "   +---------------------------+ " << std::endl;
	}
}


bool Board::checkIfFieldFree(const std::string &chosenField)
{
	return (gameBoard[charIntToInt(chosenField[0]) - 1][charIntToInt(chosenField[1]) - 1] == "\0");
}


void Board::compareBinaryStrings(const std::string &stone1, const std::string &stone2, std::vector<int> &Masks)
{
	for (int i = 0; i < Stones::stoneStringSize; ++i)
	{	
		//XOR operation : ^; returns 1 if numbers are different 
		if (charIntToInt(stone1[i]) ^ charIntToInt(stone2[i]) == 1)
			//delete mask option from vector
			Masks.at(i) = 0;
	}
}

void Board::setMaskVectors(std::vector<std::vector<int> > &Masks)
{
	for (int i = 0; i < Masks.size(); ++i)
	{
		Masks[i] =
		{
			//Masks for every bit for binary string stones 
			//cannot be specified in binary like 0bxxxx, because this is not compiled as -std=c++14
			8,		//0b1000
			4,		//0b0100
			2,		//0b0010
			1		//0b0001	
		};
	}	
}

void Board::clearEmptyMaskVectors(std::vector<std::vector<int> > &vectorOfMasks)
{
	//remove all elements that contain mask equal to zero, it is used with meaning of deleted value
	for (int i = 0; i < vectorOfMasks.size(); ++i)
		vectorOfMasks.at(i).erase(std::remove(vectorOfMasks.at(i).begin(), vectorOfMasks.at(i).end(), 0), vectorOfMasks.at(i).end());
}

bool Board::checkForWinners(const std::string &chosenField)
{
	//valid indexing is from zero to boardSize-1
	int chosenFieldRow = charIntToInt(chosenField[0]) - 1;
	int chosenFieldCol = charIntToInt(chosenField[1]) - 1;
	
	std::vector<std::vector<int> > vectorOfMasks;
	vectorOfMasks.resize(4);

	//set all Masks to default value
	setMaskVectors(vectorOfMasks);

	//vectorOfMasks.at(0) == horizontalMask
	//vectorOfMasks.at(1) == VerticalMask
	//vectorOfMasks.at(2) == MainDiagonalMask
	//vectorOfMasks.at(3) == SecDiagonalMask
	for (int index = 1; index < boardSize; ++index)
	{
		//check Horizontally
		//if vector Masks is empty then not only one of stone characteristics were same
		//rest does not have to be searched
		if (!vectorOfMasks.at(0).empty())
		{
			//not horizontally filled, cannot be succesful later on
			if ((gameBoard[chosenFieldRow][0] == "\0") || (gameBoard[chosenFieldRow][index] == "\0"))
				vectorOfMasks.at(0).clear();
			else
				//compare bit characteristics in stones that are horizontally ordered
				compareBinaryStrings(gameBoard[chosenFieldRow][0], gameBoard[chosenFieldRow][index], vectorOfMasks.at(0));
			
		}

		//check Vertically
		//if vector Masks is empty then not only one of stone characteristics were same
		//rest does not have to be searched
		if (!vectorOfMasks.at(1).empty())
		{
			//not vertically filled, cannot be succesful later on
			if ((gameBoard[0][chosenFieldCol] == "\0") || (gameBoard[index][chosenFieldCol] == "\0"))
				vectorOfMasks.at(1).clear();
			else
				//compare bit characteristics in stones that are horizontally ordered
				compareBinaryStrings(gameBoard[0][chosenFieldCol], gameBoard[index][chosenFieldCol], vectorOfMasks.at(1));
		}

		//check Main Diagonal
		//if vector Masks is empty then not only one of stone characteristics were same
		//rest does not have to be searched
		if (chosenFieldRow == chosenFieldCol)
		{

			//field cells on main diagonal [1,1] [2,2] [3,3] [4,4]
			//represented in C indexing as [0,0] [1,1] [2,2] [3,3]
			if (!vectorOfMasks.at(2).empty())
			{
				//not vertically filled, cannot be succesful later on
				if ((gameBoard[0][0] == "\0") || (gameBoard[index][index] == "\0"))
					vectorOfMasks.at(2).clear();
				else
					compareBinaryStrings(gameBoard[0][0], gameBoard[index][index], vectorOfMasks.at(2));
			}
		}

		if (chosenFieldCol == boardSize - chosenFieldRow + 1)
		{
			//field cells on secondary diagonal [1,4] [2,3] [3,2] [4,1]
			//	   represented in C indexing as [0,3] [1,2] [2,1] [3,0]
			if (!vectorOfMasks.at(3).empty())
			{
				//not vertically filled, cannot be succesful later on
				if ((gameBoard[0][boardSize - 1] == "\0") || (gameBoard[index][boardSize - 1 - index] == "\0"))
					vectorOfMasks.at(3).clear();
				else
					compareBinaryStrings(gameBoard[0][boardSize - 1], gameBoard[index][boardSize - 1 - index], vectorOfMasks.at(3));
			}
		
		}
	}
	
	clearEmptyMaskVectors(vectorOfMasks);

	//some common characteristic was found
	//we have a winner here
	if (!vectorOfMasks.at(0).empty() || !vectorOfMasks.at(1).empty())
		return true;

	if (!vectorOfMasks.at(2).empty() && (chosenFieldRow == chosenFieldCol))
		return true;

	else if (!vectorOfMasks.at(3).empty() && (chosenFieldCol == boardSize - chosenFieldRow + 1))
		return true;

	return false;
}

bool Board::isBoardFull(void)
{
	for (int row = 0; row < boardSize; ++row)
	{
		for (int col = 0; col < boardSize; ++col)
		{
			if (gameBoard[row][col] == "\0")
				return false;
		}
	}

	return true;
}

void Board::gameStatus(const std::string &chosenField, PlayersEnum playerChoice)
{
	//we found winner
	if (checkForWinners(chosenField))
	{
		if (playerChoice == PlayersEnum::computer)
		{
			std::cout << "computer wins" << std::endl;
			exit (EXIT_SUCCESS);
		}
		else
		{
			std::cout << "player wins" << std::endl;
			exit (EXIT_SUCCESS);
		}	
	}

	//no winner and board is full, game also ended here
	else if (isBoardFull())
	{
		std::cout << "no winner" << std::endl;
		exit (EXIT_SUCCESS);
	}

	else
		std::cout << "playing" << std::endl;
}

void Board::placeStone(const std::string chosenStone, const std::string chosenField)
{
	//get string indexes to integer and write stone on that position
	// - 1 because of different indexing for player, he expects 1 - 4
	gameBoard[charIntToInt(chosenField[0]) - 1][charIntToInt(chosenField[1]) - 1] = chosenStone;
}

std::string Board::generateRandomField(void)
{
	srand((int)time(0));
	int randomNumber = (rand() % freeFields.size());

	return freeFields.at(randomNumber);
}

void Board::reduceFreeFields(const std::string chosenField)
{
	//remove chosen stone from vector of freeStones via std::remove from <algorithm>
	freeFields.erase(std::remove(freeFields.begin(), freeFields.end(), chosenField), freeFields.end());
}