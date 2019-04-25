#include "JSONParser.h"
#include "Stones.h"

bool JSONParser::checkIfBinaryString(const std::string &chosenString)
{
	//chosenString should be equal to stoneStringSize, which is 4
	if (chosenString.length() != Stones::stoneStringSize)
		return false;
	else
	{
		for (int i = 0; i < chosenString.length(); ++i)
		{
			if ((chosenString[i] != '0') && (chosenString[i] != '1'))
				return false;
		}
	}

	return true;
}

void JSONParser::setInput(const std::string &chosenInput)
{
	input = chosenInput;
	
	//reset
	parserIndex = 0;
	resultPairs.clear();
}

std::vector<std::pair<std::string, std::string>> JSONParser::getResult(void)
{
	return resultPairs;
}

bool JSONParser::checkIfFieldString(const std::string &chosenString)
{
	//chosenString should be equal to fieldStringSize, which is 2
	if (chosenString.length() != Board::fieldStringSize)
		return false;
	else
	{
		//check if first and second char of string is number
		if (!std::isdigit(chosenString[0]) || !std::isdigit(chosenString[1]))
			return false;
		//check if number <0 - 4> as boardSize [board valid indexes]
		else
			return
			(
				charIntToInt(chosenString[0]) >= 1 &&
				charIntToInt(chosenString[0]) <= 4 && 
				charIntToInt(chosenString[1]) >= 1 &&
				charIntToInt(chosenString[1]) <= 4
			) ? true : false;		
	}
}

bool JSONParser::isTokenString(char tokenString)
{
	//tokenString small letter alphabet
	if ((tokenString >= 'a') && (tokenString <= 'z'))
		return true;
	//tokenString big letter alphabet
	else if ((tokenString >= 'A') && (tokenString <= 'Z'))
		return true;
	else if (std::isdigit(tokenString))
		return true;
	//lower dash for "chosen_stone" option
	else if (tokenString == '_')
		return true;
	
	return false;
}




void JSONParser::getToken(void)
{
	Token token;
	token.inputTokenType = tokenType::eof;
	token.inputTokenValue.clear();

	char character = 0;
	while (parserIndex < input.length())
	{
		character = input [parserIndex];

		//skip white spaces \t and ' '
		if (character == '\t')
		{
			parserIndex++;
			continue;
		}
		
		else if (character == ' ')
		{
			parserIndex++;
			continue;
		}
		
		else if (character == '{')
		{
			token.inputTokenType = tokenType::bracketOpen;
			token.inputTokenValue = '{';
			parserIndex++;
			break;
		}
		
		else if (character == '}')
		{
			token.inputTokenType = tokenType::bracketClose;
			token.inputTokenValue = '}';
			parserIndex++;
			break;
		}

		else if (character == ':')
		{
			token.inputTokenType = tokenType::colon;
			token.inputTokenValue = ':';
			parserIndex++;
			break;
		}

		else if (character == '"')
		{
			token.inputTokenType = tokenType::quote;
			token.inputTokenValue = '"';
			parserIndex++;
			break;
		}

		else if (character == ',')
		{
			token.inputTokenType = tokenType::comma;
			token.inputTokenValue = ',';
			parserIndex++;
			break;
		}

		else if (isTokenString(character))
		{
			token.inputTokenType = tokenType::string;
			token.inputTokenValue += character;
			parserIndex++;
			character = input [parserIndex];
			
			while (isTokenString(character))
			{
				token.inputTokenValue += character;
				parserIndex++;
				character = input [parserIndex];
			}
			break;
		}

		else
		{
			token.inputTokenType = tokenType::error;
			token.inputTokenValue.clear();
			break;
		}
	}

	currentToken = token;
}


bool JSONParser::parse(void)
{

	getToken();
	//JSON request starts with open bracket
	if (currentToken.inputTokenType != tokenType::bracketOpen)
		return false;


	std::string key, value;
	//Parse pair key - value
	while (true)
	{
		key.clear();
		value.clear();

		getToken();

		//key part starts with quote
		//if not found, json is wrong
		if (currentToken.inputTokenType != tokenType::quote)
			return false;

		getToken();

		//Value of key
		if (currentToken.inputTokenType != tokenType::string)
			return false;

		key = currentToken.inputTokenValue;

		getToken();

		//if not second quote after value then error
		if (currentToken.inputTokenType != tokenType::quote)
			return false;

		//key already parsed, now value is coming
		getToken();

		//value is separated from key by colon :
		if (currentToken.inputTokenType != tokenType::colon)
			return false;

		getToken();

		//KEY field does not expect quotes 
		if (key != "field")
		{	
			//start of value part, quote expected
			if (currentToken.inputTokenType != tokenType::quote)
				return false;
			getToken();
		}

		//value
		if (currentToken.inputTokenType != tokenType::string)
			return false;

		//save value
		value = currentToken.inputTokenValue;

		//KEY field does not expect quotes 	
		if (key != "field")
		{	
			getToken();
			//second quote expected
			if (currentToken.inputTokenType != tokenType::quote)
				return false;
		}

		//make pair oficially into std::pair result, which is output of parser
		std::pair<std::string, std::string> key_value_pair(key, value);
		//push pair to the table of result pairs
		resultPairs.push_back(key_value_pair);

		getToken();

		//but ending of JSON has to be also checked
		//if bracket closed here, parse was succesful
		if (currentToken.inputTokenType == tokenType::bracketClose)
			break;
		//maybe next pair of values; field + stone situation
		else if (currentToken.inputTokenType != tokenType::comma)
			return false;
	
	}

	getToken();

	//line has to be ended by EOF character
	if (currentToken.inputTokenType != tokenType::eof)
		return false;

	return true;
}