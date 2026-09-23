#pragma once
#include <vector>
#include <string>

class Room;

class Character {

    public:

        // Constructor
        Character();


        void moveChar(const std::string& direction);

        void inspectRoom();
        void attack();
        void pickUpItem(const std::string& item);
        void useItem(const std::string& item);
        void viewStats();
        void viewMap();

        //Health
        int getHealth() const;
        void takeDamage(int damage);
        void heal(int amount);

        //Strength
        int getStrength() const;
        void decreaseStrength(int amount);
        void increaseStrength(int amount);

        //Defense
        int getDefense() const;
        void decreaseDefense(int amount);
        void increaseDefense(int amount);

        //Weapon
        std::string getWeapon() const;
        void setWeapon(const std::string& weapon);

        //Experience
        int getExperience() const;
        void decreaseExperience(int amount);
        void increaseExperience(int amount);

        //Level
        int getLevel() const;
        void levelUp();

        //Current room set/get
        void setCurrentRoom(Room* room);
        Room* getCurrentRoom() const;

    private:

        std::string name;

        int health;
        int maxHealth;
        int strength;
        int defense;

        std::string weapon;

        int experience;
        int levelThreshold;
        int level;

        std::vector<std::string> inventory;
        std::vector<std::string> lootTable;

		Room* currentRoom;
};
