#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "eraGame.h"
#include "Board.h"

#include <cctype>
#include <utility>

class JSONParser
{
private:

	//enum for type tokens, will be needed for further parsing
	enum class tokenType
	{
		error,
		eof,
		bracketOpen,
		bracketClose,
		quote,
		string,
		comma,
		colon
	};

	struct Token
	{
		tokenType inputTokenType;
		std::string inputTokenValue;
	};

	Token currentToken;
	//input to JSONparser, this will be parsed
	std::string input;

	int parserIndex;

	//result pair, output of this JSONparser
	std::vector<std::pair<std::string, std::string>> resultPairs;

	void getToken(void);

public:	
	JSONParser() {parserIndex = 0;}
	JSONParser(const std::string &chosenInput) {input = chosenInput; parserIndex = 0;}
	
	bool checkIfBinaryString(const std::string &);
	bool checkIfFieldString(const std::string &);

	bool isTokenString(char tokenString);
	bool parse(void);
	void setInput(const std::string &);
	std::vector<std::pair<std::string, std::string> > getResult(void);

	bool getPlacingStone(std::string &);
	bool getChosingField(std::string &);
	bool getGameStatus(std::string &);

};

#endif