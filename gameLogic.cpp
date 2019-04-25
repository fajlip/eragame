#include "gameLogic.h"
#include "JSONParser.h"
#include "Board.h"
#include "Stones.h"

#include <iostream>
#include <string>

namespace gameLogic
{

	std::string placeStonePlayer(JSONParser &eraJSON, Board &eraBoard, Stones &eraStones, std::string generatedStone)
	{
		const short int KEY = 0;
		const short int VALUE = 1;

		std::string playerChosenString;
		std::cout << "placing stone" << std::endl; //mozna odmazat
		
		//getline because std::cin would stop on space otherwise
		std::getline(std::cin, playerChosenString);

		eraJSON.setInput(playerChosenString);
		if (!eraJSON.parse())
		{
			std::cerr << "JSON parsing error occured, your input is not correct." << std::endl;
			exit(EXIT_FAILURE);
		}

		if (eraJSON.getResult().size() > 2)
		{
			std::cerr << "More than two key : value JSON inputs, not possible in this application." << std::endl;
			exit(EXIT_FAILURE);
		}

		//check if chosen_stone KEY was entered
		std::string stoneValue, fieldValue;
		if (std::get<KEY>(eraJSON.getResult().at(0)) == "stone")
			stoneValue = std::get<VALUE>(eraJSON.getResult().at(0));
		else if (std::get<KEY>(eraJSON.getResult().at(1)) == "stone")
			stoneValue = std::get<VALUE>(eraJSON.getResult().at(1));
		else
		{
			std::cerr << "Input: chosen_stone as JSON Key is not correct." << std::endl;
			exit(EXIT_FAILURE);
		} 

		//check if field KEY was entered
		if (std::get<KEY>(eraJSON.getResult().at(0)) == "field")
			fieldValue = std::get<VALUE>(eraJSON.getResult().at(0));	
		else if (std::get<KEY>(eraJSON.getResult().at(1)) == "field")
			fieldValue = std::get<VALUE>(eraJSON.getResult().at(1));
		else
		{
			std::cerr << "Input: Field as JSON Key is not correct." << std::endl;
			exit(EXIT_FAILURE);
		}

		//once more inputed stone (for control) is not the same
		if (stoneValue != generatedStone)
		{
			std::cerr << "Repeated stone is not the same as previous chosen one." << std::endl;
			exit(EXIT_FAILURE);
		} 

		//check if input field value is correct one in range of <1;4>
		else if (!eraJSON.checkIfFieldString(fieldValue))
		{
			std::cerr << "Field value is not in range of <1;4>." << std::endl;
			exit(EXIT_FAILURE);
		}

		//check if input field value is free to use
		if (!eraBoard.checkIfFieldFree(fieldValue))
		{
			std::cerr << "Chosen field is not free." << std::endl;
			exit(EXIT_FAILURE);
		}

		//everything is ok
		//place chosen stone on chosen field
		eraBoard.placeStone(stoneValue, fieldValue);
		eraStones.reduceFreeStones(stoneValue);
		eraBoard.reduceFreeFields(fieldValue);

		//for gameStatus function, it does not check the whole matrix (not necessary)
		return fieldValue;
	}

	std::string placeStoneAI(Board &eraBoard, Stones &eraStones, std::string stoneValue)
	{
		std::string fieldValue = eraBoard.generateRandomField();

		std::cout << "placing stone" << std::endl;
		std::cout << "{\"stone\": " << "\"" << stoneValue << "\", \"field\": " << fieldValue << "}\n";

		eraBoard.placeStone(stoneValue, fieldValue);
		eraStones.reduceFreeStones(stoneValue);
		eraBoard.reduceFreeFields(fieldValue);

		//for gameStatus function, it does not check the whole matrix (not necessary)
		return fieldValue;
	}

	std::string chooseStonePlayer(JSONParser &eraJSON, Stones &eraStones)
	{
		std::cout << "giving chosen stone" << std::endl;
		std::string playerChosenString;

		//getline because std::cin would stop on space otherwise
		std::getline(std::cin, playerChosenString);

		const short int KEY = 0;
		const short int VALUE = 1;

		eraJSON.setInput(playerChosenString);
		if (!eraJSON.parse())
		{
			std::cerr << "JSON parsing error occured, your input is not correct." << std::endl;
			exit(EXIT_FAILURE);
		}

		//more than one JSON KEY input, not possible this time
		else if (eraJSON.getResult().size() > 1)
		{
			std::cerr << "More than two key : value JSON inputs, not possible in this application." << std::endl;
			exit(EXIT_FAILURE);
		}
		
		else if (std::get<KEY>(eraJSON.getResult().at(0)) != "chosen_stone")
		{
			std::cerr << "Input: chosen_stone as JSON Key is not correct." << std::endl;
			exit(EXIT_FAILURE);
		}

		else if (!eraJSON.checkIfBinaryString(std::get<VALUE>(eraJSON.getResult().at(0))))
		{
			std::cerr << "Stone value is not binary string." << std::endl;
			exit(EXIT_FAILURE);
		}

		else if (!eraStones.checkIfFreeStone(std::get<VALUE>(eraJSON.getResult().at(0))))
		{
			std::cerr << "Stone value has already been used. This is not free stone." << std::endl;
			exit(EXIT_FAILURE);
		}

		return (std::get<VALUE>(eraJSON.getResult().at(0)));
	}

	std::string chooseStoneAI(Stones &eraStones)
	{
		std::string generatedStone;
		std::cout << "giving chosen stone" << std::endl; //mozna odmazat
		generatedStone = eraStones.generateRandomStone();
		std::cout << "{\"chosen_stone\": \"" << generatedStone << "\"}\n";

		return generatedStone;
	}

}