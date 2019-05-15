apps: CheckerBoard.o main.o Game.o
	g++ CheckerBoard.o main.o Game.o -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system -o apps

CheckerBoard.o: CheckerBoard.cpp
	g++ -c -std=c++11 CheckerBoard.cpp
main.o: main.cpp
	g++ -c -std=c++11 main.cpp
Game.o: Game.cpp
	g++ -c -std=c++11 Game.cpp
clean: 
	rm -rf *.o app

