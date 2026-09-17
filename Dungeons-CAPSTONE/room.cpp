#include "room.hpp"
#include  <iostream>


Room::Room(const std::string& name, const std::string& description) 
	: name(name), description(description), 
	north(nullptr), south(nullptr), east(nullptr), west(nullptr),
	layout(8, std::vector<TileType>(10, TileType::Floor)) {
	
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 10; x++) {
			if (y == 0 || y == 7 || x == 0 || x == 9) {
				layout[y][x] = TileType::Wall;
			}
		}
	}
}

void Room::setNorth(Room* room) { north = room; }
void Room::setSouth(Room* room) { south = room; }
void Room::setEast(Room* room) { east = room; }
void Room::setWest(Room* room) { west = room; }

Room* Room::getNorth() const { return north; }
Room* Room::getSouth() const { return south; }
Room* Room::getEast() const { return east; }
Room* Room::getWest() const { return west; }

std::string Room::getName() const { return name; }

const std::vector<std::vector<TileType>>& Room::getLayout() const {
	return layout;
}

void Room::inspect() const {
	std::cout << "You are in " << name << ". " << description << std::endl;
}
