#pragma once
#include <string>
#include <vector>
#include "enemies.hpp"

enum class TileType {
	Wall,
	Floor,
	Door,
	
};

class Room {

private:
	std::string name;
	std::string description;

	Room* north;
	Room* south;
	Room* east;
	Room* west;

	std::vector<std::vector<TileType>> layout;	
	std::vector<Enemy> enemies;

public:
	Room(const std::string& name, const std::string& description);

	void setNorth(Room* room);
	void setSouth(Room* room);
	void setEast(Room* room);
	void setWest(Room* room);

	Room* getNorth() const;
	Room* getSouth() const;
	Room* getEast() const;
	Room* getWest() const;

	std::string getName() const;

	const std::vector<std::vector<TileType>>& getLayout() const;

	void inspect() const;

	//Enemies
	void addEnemy(const Enemy& enemy);
	const std::vector<Enemy>& getEnemies() const;
};