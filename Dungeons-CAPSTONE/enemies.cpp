#include "enemies.hpp"

Enemy::Enemy() {
	name = "basic enemy";
	health = 10;
	maxHealth = 10;
	strength = 1;
	defense = 1;

}

std::string Enemy::getName() const {
	return name;
}

void Enemy::setName(const std::string& name) {
	this->name = name;
}

int Enemy::getHealth() const {
	return health;
}

void Enemy::takeDamage(int amount) {
	health -= amount;

	if (health < 0) {
		health = 0;
	}
}

int Enemy::getStrength() const {
	return strength;
}

void Enemy::decreaseStrength(int amount) {
	strength -= amount;

	if (strength < 0) {
		strength = 0;
	}
}

void Enemy::increaseStrength(int amount) {
	strength += amount;
}

int Enemy::getDefense() const {
	return defense;
}

 void Enemy::decreaseDefense(int amount) {
	defense -= amount;

	if (defense < 0) {
		defense = 0;
	}
}

void Enemy::increaseDefense(int amount) {
	defense += amount;
}
