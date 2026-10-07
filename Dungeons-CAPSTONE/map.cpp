#include "map.hpp"
#include <iostream>



Map::Map() : startingRoom(nullptr) {

	createMap();
};

void Map::createMap() {
	
    rooms.emplace_back("Entrance", "The entrance to the dungeon.");
    rooms.emplace_back("Armory", "An old room filled with forgotten weapons.");
    rooms.emplace_back("Library", "A dusty library filled with ancient books.");
    rooms.emplace_back("Storage", "A dark storage room filled with old supplies.");
    rooms.emplace_back("Boss Room", "A large chamber where something dangerous awaits.");

    Room* entrance = &rooms[0];
    Room* armory = &rooms[1];
    Room* library = &rooms[2];
    Room* storage = &rooms[3];
    Room* bossRoom = &rooms[4];

    //Protype enemy addition
    Enemy goblin("Goblin", 4, 2, 0, 5);
    goblin.setPosition(sf::Vector2f(300.f, 200.f));
    armory->addEnemy(goblin);

    Enemy goblin2("Goblin", 4, 2, 0, 5);
    goblin2.setPosition(sf::Vector2f(450.f, 250.f));
    armory->addEnemy(goblin2);

	Enemy boss("Boss", 30, 6, 2, 0, true);
	boss.setPosition(sf::Vector2f(400.f, 300.f));
	bossRoom->addEnemy(boss);

    entrance->setNorth(armory);
    entrance->setWest(library);
    entrance->setEast(storage);
    entrance->setSouth(bossRoom);

    armory->setSouth(entrance);
    library->setEast(entrance);
    storage->setWest(entrance);
    bossRoom->setNorth(entrance);

    startingRoom = entrance;
}

Room* Map::getStartingRoom() {
    return startingRoom;
}