#pragma once
 
#include "character.hpp"
#include  "map.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>


enum class GameState {
    Playing,
    Won,
    Lost,
    MainMenu
};


class Game {

private:
    Character player;
    Map map;

    sf::RenderWindow window;
	sf::RectangleShape playerSprite;
    std::vector<sf::CircleShape> enemySprites;

	const float tileSize = 64.f;
    const float playerSpeed = 2.f;

public:

    Game();
    void run();

    bool isInRange(const sf::Vector2f first, const sf::Vector2f second, float range);

private:
    
    bool canMove(const sf::FloatRect& bounds, Room* room);
    bool checkDoor(const sf::FloatRect& playerBounds, Room* room);

    Room* getNextRoom(const sf::FloatRect& playerBounds, Room* room);

	std::string getDoorDirection(const sf::FloatRect& playerBounds, Room* room);
    void changeRoom(Room* nextRoom, const std::string& direction);



	void processEvents();
    void update(float deltaTime);
    void render();


};