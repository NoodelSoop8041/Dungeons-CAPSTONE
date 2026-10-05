#include "character.hpp"

//Character class constructor
Character::Character() {
    name = "Default Name";
    health = 10;
    maxHealth = 10;
    strength = 1;
    defense = 1;
    weapon = "Fists";

    experience = 0;
    levelThreshold = 10;
    level = 1;

    attackCooldown = 0.5f;
    attackTimer = 0.0f;

    inventory = {};
    currentRoom = nullptr;

}


void Character::moveChar(const std::string& direction) {}

void Character::inspectRoom() {}

void Character::attack() {}

void Character::pickUpItem(const std::string& item) {}

void Character::useItem(const std::string& item) {}

void Character::viewStats() const {
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "Strength: " << strength << std::endl;
    std::cout << "Defense: " << defense << std::endl;
    std::cout << "Weapon: " << weapon << std::endl;
    std::cout << "Experience: " << experience << std::endl;
    std::cout << "Level: " << level << "/" << levelThreshold << std::endl;
}

void Character::viewMap() {}

int Character::getHealth() const { 
    return health; 
} 

void Character::takeDamage(int damage) { 
    health -= damage; 
    if (health < 0) {
        health = 0; 
    } 
} 

void Character::heal(int amount) { 
    health += amount; 

    if (health >= maxHealth) {
        health = maxHealth;
    }
} 

int Character::getMaxHealth() const {
	return maxHealth;
}

int Character::getStrength() const {
    return strength; 
} 

void Character::decreaseStrength(int amount) { 
    strength -= amount; 

    if (strength < 0) { 
        strength = 0; 
    } 
} 

void Character::increaseStrength(int amount) { 
    strength += amount; 
} 

//Defense 
int Character::getDefense() const { 
    return defense; 
} 

void Character::decreaseDefense(int amount) { 
    defense -= amount; 

    if (defense < 0) { 
        defense = 0; 
    } 
} 

void Character::increaseDefense(int amount) { 
    defense += amount; 
} //Weapon 

std::string Character::getWeapon() const { 
    return weapon; 
} 

void Character::setWeapon(const std::string& weapon) {
    this->weapon = weapon; 
} 

//Experience 
int Character::getExperience() const { 
    return experience; 
} 

//Used as part of levelUp? This would need amount changed to a relevant var. 
void Character::decreaseExperience(int amount) { 
    experience -= amount; 

    if (experience < 0) {
        experience = 0;
    }
} 

void Character::increaseExperience(int amount) {
    experience += amount;

    while (experience >= levelThreshold) { 
        experience -= levelThreshold; 
        levelUp(); 
    } 
} 

//Level 
int Character::getLevel() const { 
    return level; 
} 

//Prototype level up 
void Character::levelUp() { 

    level++;
    maxHealth += 5;
    heal(maxHealth - health);
    increaseDefense(1); 
    increaseStrength(1);
}

int Character::getLevelThreshold() const {
	return levelThreshold;
}

bool Character::canAttack() const {
    return attackTimer <= 0.f;
}

void Character::updateAttackCooldown(float deltaTime) {

    if (attackTimer > 0.f) {

        attackTimer -= deltaTime;

        if (attackTimer < 0.f) {
            attackTimer = 0.f;
        }
    }
}

void Character::resetAttackCooldown() {
    attackTimer = attackCooldown;
}

float Character::getAttackTimer() const
{
    return attackTimer;
}




void Character::setCurrentRoom(Room* room) { currentRoom = room; }
Room* Character::getCurrentRoom() const { return currentRoom; }