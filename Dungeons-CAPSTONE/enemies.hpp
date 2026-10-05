#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Character; // Forward declaration

class Enemy {

public:

	Enemy();

	Enemy(const std::string name, int health, int strength, int defense, int xpReward);

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

	int getXpReward() const;

	bool isAlive() const;

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& position);

	sf::Vector2f getMoveTowards(const sf::Vector2f& target, float speed) const;
	void move(const sf::Vector2f& movement);

	sf::FloatRect getBounds() const;

	//Attack members - these should be moved into their own combat class later
	void attack(Character& target, int damage);
	
	bool canAttack() const;
	void updateAttackCooldown(float deltaTime);
	void resetAttackCooldown();


private:

	std::string name;

	int health;
	int maxHealth;
	int strength;
	int defense;

	int xpReward;

	sf::Vector2f position;

	float attackCooldown;
	float attackTimer;
};