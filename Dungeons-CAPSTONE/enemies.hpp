#include "includes.hpp"

class Enemy {

public:

	Enemy();

	Enemy(const std::string name, int health, int strength, int defense);

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

	bool isAlive() const;

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& position);

	void moveTowards(const sf::Vector2f& target, float speed);

private:

	std::string name;

	int health;
	int maxHealth;
	int strength;
	int defense;

	sf::Vector2f position;

};