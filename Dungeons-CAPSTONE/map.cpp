#include "map.hpp"


Map::Map() : startingRoom(nullptr), bossRoom(nullptr) {

	createMap();
}

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
    Room* bossArena = &rooms[4];

    auto addGoblin = [](Room* room, float x, float y) {
        Enemy g("Goblin", 4, 2, 2, 5);
        g.setPosition(sf::Vector2f(x, y));
        room->addEnemy(g);
    };


    addGoblin(library, 200.f, 300.f);
    addGoblin(library, 350.f, 350.f);

    addGoblin(storage, 300.f, 300.f);
    addGoblin(storage, 250.f, 150.f);

    //Protype enemy addition
    Enemy goblin("Goblin", 4, 2, 0, 5);
    goblin.setPosition(sf::Vector2f(300.f, 200.f));
    armory->addEnemy(goblin);

    Enemy goblin2("Goblin", 4, 2, 0, 5);
    goblin2.setPosition(sf::Vector2f(450.f, 250.f));
    armory->addEnemy(goblin2);

	Enemy boss("Boss", 30, 6, 2, 0, true);
	boss.setPosition(sf::Vector2f(400.f, 300.f));
	bossArena->addEnemy(boss);

    entrance->setNorth(armory);
    entrance->setWest(library);
    entrance->setEast(storage);
    entrance->setSouth(bossArena);

    armory->setSouth(entrance);
    library->setEast(entrance);
    storage->setWest(entrance);
    bossArena->setNorth(entrance);

    startingRoom = entrance;
    bossRoom = bossArena;
}

Room* Map::getStartingRoom() {
    return startingRoom;
}

Room* Map::getBossRoom() const {
    return bossRoom;
}

bool Map::isBossUnlocked() const {
    for (const Room& room : rooms) {
        if (&room == bossRoom) {
            continue;
        }

        if (!room.isCleared()) {
            return false;
        }
    }
    
    return true;
}