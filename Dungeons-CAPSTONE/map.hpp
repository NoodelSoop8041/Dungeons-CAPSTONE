#pragma once
#include "room.hpp"
#include <vector>

class Map {


private:
	std::vector<Room> rooms;
	Room* startingRoom;
	Room* bossRoom;

public:

	Map();

	void createMap();

	Room* getStartingRoom(); 

	Room* getBossRoom() const;

	bool isBossUnlocked() const;

};