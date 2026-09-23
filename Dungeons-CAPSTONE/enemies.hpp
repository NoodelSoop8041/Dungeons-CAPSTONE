#pragma once
#include <string>

class Enemy {

public:
	Enemy();

	std::string getName() const;
	void setName(const std::string& name);

	int getHealth() const;
	void takeDamage(int amount);

	int getStrength() const;
	void decreaseStrength(int amount);
	void increaseStrength(int amount);

	int getDefense() const;
	void decreaseDefense(int amount);
	void increaseDefense(int amount);

private:
	std::string name;

	int health;
	int maxHealth;
	int strength;
	int defense;

};