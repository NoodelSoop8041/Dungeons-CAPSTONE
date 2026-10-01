#include "includes.hpp"

Enemy::Enemy() :
	name("basic enemy"),
	health(10), maxHealth(10),
	strength(1), defense(1),
	position(0.f, 0.f),
	attackCooldown(1.0f),
	attackTimer(0.0f) {}

Enemy::Enemy(const std::string name, int health, int strength, int defense) :
	name(name), 
	health(health), maxHealth(health), 
	strength(strength), defense(defense), 
	position(0.f, 0.f),
	attackCooldown(1.0f),
	attackTimer(0.0f) {}

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

bool Enemy::isAlive() const {
	return health > 0;
}

sf::Vector2f Enemy::getPosition() const {
	return position;
}

void Enemy::setPosition(const sf::Vector2f& position) {
	this->position = position;
}


sf::Vector2f Enemy::getMoveTowards(const sf::Vector2f& target, float speed) const {

	sf::Vector2f direction = target - getBounds().getCenter();

	float length = std::sqrt(
		direction.x * direction.x +
		direction.y * direction.y);

	if (length > 0.f) {
		direction /= length;
	}

	return direction * speed;
}

void Enemy::move(const sf::Vector2f& movement) {
	position += movement;
}

sf::FloatRect Enemy::getBounds() const {
	return sf::FloatRect(position, sf::Vector2f(30.f, 30.f)); 
}

void Enemy::attack(Character& target, int damage) {
	target.takeDamage(damage);
}

bool Enemy::canAttack() const {
	return attackTimer <= 0.0f;
}

void Enemy::updateAttackCooldown(float deltaTime) {
	if (attackTimer > 0.f) {

		attackTimer -= deltaTime;

		if (attackTimer < 0.f) {

			attackTimer = 0.f;
		}
	}
}

void Enemy::resetAttackCooldown() {
	attackTimer = attackCooldown;
}