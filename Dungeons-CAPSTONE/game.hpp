#pragma once
 
#include "character.hpp"
#include  "map.hpp"
#include "room.hpp"

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>


enum class GameState {
    Playing,
    Won,
    Lost
};


class Game {

private:
    Character player;
    Map map;

	GameState state = GameState::Playing;

    sf::RenderWindow window;
	sf::RectangleShape playerSprite;
    std::vector<sf::CircleShape> enemySprites;

	const float tileSize = 64.f;
    const float playerSpeed = 2.f;

    sf::Font font;

public:

    Game();
    void run();

    bool isInRange(const sf::Vector2f first, const sf::Vector2f second, float range) const;

private:
    
    bool canMove(const sf::FloatRect& bounds, Room* room) const;
    bool checkDoor(const sf::FloatRect& playerBounds, Room* room) const;

    Room* getNextRoom(const sf::FloatRect& playerBounds, Room* room) const;

	std::string getDoorDirection(const sf::FloatRect& playerBounds, Room* room) const;
    void changeRoom(Room* nextRoom, const std::string& direction);



	void processEvents();
    void update(float deltaTime);
    void render();

    void reset();

	void drawHud();
    void drawOverlay(const std::string& message);


	//Duplicate of layout size, known coupling, but it works for v1.0
    static constexpr unsigned roomWidth = 640; //10 tiles * 64 pixels
    static constexpr unsigned roomHeight = 512; //8 tiles * 64 pixels
    static constexpr unsigned hudHeight = 80;

};
