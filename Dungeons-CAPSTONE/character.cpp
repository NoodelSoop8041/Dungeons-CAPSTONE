#include "character.hpp"
#include <iostream>

// Functions of the dungeons:
// 1. Character creation (includes weapons and stats)
// 2. Character movement (creating a map and moving between rooms)
// 3. Character combat (boss creation and attacks)
// 4. Character inventory (inherited from creation / includes leveling)
// 5. Visuals (this will be a work in progress)

//What can the charcter do?
// Move (up, down, left, right), inspect rooms, attack, pick up items, inventory,
// use items, view stats, view map (maybe get to this)

//Character class constructor
Character::Character() {
    name = "Default Name";
    health = 10;
    strength = 1;
    defense = 1;
    weapon = "Fists";
    experience = 0;
    level = 1;
    inventory = {};
    currentRoom = nullptr;

}


void Character::moveChar(const std::string& direction) {};


void Character::inspectRoom() {};

void Character::attack() {};

void Character::pickUpItem(const std::string& item) {};

void Character::useItem(const std::string& item) {};

void Character::viewStats() {};

void Character::viewMap() {};

void Character::setCurrentRoom(Room* room) { currentRoom = room; };
Room* Character::getCurrentRoom() const { return currentRoom; };