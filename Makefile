all:
	g++ -std=c++11 eraGame.cpp Board.cpp Stones.cpp gameLogic.cpp JSONParser.cpp -o eragame

#-Wall -Wextra -pedantic