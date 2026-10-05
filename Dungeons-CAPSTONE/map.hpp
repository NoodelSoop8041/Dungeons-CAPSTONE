#pragma once
#include "room.hpp"
#include <vector>

class Map {


private:
	std::vector<Room> rooms;
	Room* startingRoom;

public:

	Map();

	void createMap();

	Room* getStartingRoom(); 

};