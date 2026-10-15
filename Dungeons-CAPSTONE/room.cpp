#include "room.hpp"
#include "enemies.hpp"
#include <string>
#include <vector>
#include <iostream>


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

void Room::setNorth(Room* room) { 
	north = room; 
	if (room != nullptr) {
		layout[0][4] = TileType::Door;
	}
}

void Room::setSouth(Room* room) { 
	south = room; 
	if (room != nullptr) {
		layout[7][4] = TileType::Door;
	}
}

void Room::setEast(Room* room) { 
	east = room;
	if (room != nullptr) {
		layout[3][9] = TileType::Door;
	}
}

void Room::setWest(Room* room) { 
	west = room; 
	if (room != nullptr) {
		layout[3][0] = TileType::Door;
	}
}

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

//Enemies
void Room::addEnemy(const Enemy& enemy) {
	enemies.push_back(enemy);
}

const std::vector<Enemy>& Room::getEnemies() const {
	return enemies;
}

std::vector<Enemy>& Room::getEnemies() {

	return enemies;
}

bool Room::isCleared() const {
	
	for (const Enemy& enemy : enemies) {
		if (enemy.isAlive()) {
			return false;
		}
	}
	return true;
}

void Room::sealUnlinkedDoors() {
	
}