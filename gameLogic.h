#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <string>

class JSONParser;
class Board;
class Stones;

namespace gameLogic
{
	std::string placeStonePlayer(JSONParser &, Board &, Stones &, std::string);
	std::string placeStoneAI(Board &, Stones &, std::string);
	std::string chooseStonePlayer(JSONParser &, Stones &);
	std::string chooseStoneAI(Stones &);
}

#endif