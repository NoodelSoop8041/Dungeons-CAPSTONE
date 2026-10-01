#include "includes.hpp"


class Map {


private:
	std::vector<Room> rooms;
	Room* startingRoom;

public:

	Map();

	void createMap();

	Room* getStartingRoom(); 

};