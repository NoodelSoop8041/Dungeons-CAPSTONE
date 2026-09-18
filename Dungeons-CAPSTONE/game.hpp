#pragma once

#include "character.hpp"
#include "map.hpp"
#include <SFML/Graphics.hpp>
#include  <iostream>


class Game {

private:
    Character player;
    Map map;

public:

    Game();

    void run();

private:
    
    bool canMove(const sf::FloatRect& playerBounds, Room* room);

	void processEvents();
    void update(float deltaTime);
    void render();


};