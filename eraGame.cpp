#include "eraGame.h"
#include "Board.h"
#include "Stones.h"
#include "JSONParser.h"

#include <cstdlib> //EXIT_SUCCESS; EXIT_FAILURE

int charIntToInt(char charInt)
{	
	const short int START_ASCII_NUMBERS = 48;
	return (charInt - START_ASCII_NUMBERS);
}

PlayersEnum checkArguments(int argc, char **argv)
{

	if (argc == 1)
		return PlayersEnum::computer;

	else if (std::string(argv[1]) == "--player1")
		return PlayersEnum::player;

	else if (std::string(argv[1]) == "--help")
	{
		std::cout << "ERA Game is turn-based strategy game between two players.\nThere are 16 unique stones with 4 characteristics, which are placed into 4x4 board. All stones are visible to both players. Player, who completes four stones with at least one same characteristic in row, column or diagonal, is winner." << std::endl;
		exit(EXIT_SUCCESS);
	}

	else
	{
		std::cerr << "Wrong program parameters." << std::endl;
		exit(EXIT_FAILURE);
	}

}


int main(int argc, char **argv)
{
	PlayersEnum chosenPlayer = checkArguments(argc, argv);

	Board eraBoard;
	Stones eraStones;
	JSONParser eraJSON;

	std::string chosenStone, chosenField;
	eraBoard.printBoard();
	eraStones.printFreeStones();
	
	//step 1
	if (chosenPlayer == PlayersEnum::player)
		chosenStone = gameLogic::chooseStoneAI(eraStones);
	//
	else
		chosenStone = gameLogic::chooseStonePlayer(eraJSON, eraStones);

	while (true)
	{

		//step 2
		//player plays
		if (chosenPlayer == PlayersEnum::player)
			chosenField = gameLogic::placeStonePlayer(eraJSON, eraBoard, eraStones, chosenStone);
		else
			chosenField = gameLogic::placeStoneAI(eraBoard, eraStones, chosenStone);

		eraBoard.printBoard();
		eraStones.printFreeStones();

		//step 3
		//kdyz hraju ja, tak nehlede na kdo zacal -> vyhra
		eraBoard.gameStatus(chosenField, chosenPlayer);

		//step 4
		if (chosenPlayer == PlayersEnum::player)
			chosenStone = gameLogic::chooseStonePlayer(eraJSON, eraStones);
		else
			chosenStone = gameLogic::chooseStoneAI(eraStones);
		
		//step 5
		if (chosenPlayer == PlayersEnum::player)
			chosenField = gameLogic::placeStoneAI(eraBoard, eraStones, chosenStone);
		else
			chosenField = gameLogic::placeStonePlayer(eraJSON, eraBoard, eraStones, chosenStone);

		eraBoard.printBoard();
		eraStones.printFreeStones();

		//step 6
		if (chosenPlayer == PlayersEnum::player)
			eraBoard.gameStatus(chosenField, PlayersEnum::computer);
		else
			eraBoard.gameStatus(chosenField, PlayersEnum::player);
		
		//step 7
		if (chosenPlayer == PlayersEnum::player)
			chosenStone = gameLogic::chooseStoneAI(eraStones);
		else
			chosenStone = gameLogic::chooseStonePlayer(eraJSON, eraStones);
	}

	
	return EXIT_SUCCESS;
}