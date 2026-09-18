#include "game.hpp"
#include <SFML/Graphics.hpp>


Game::Game() {
    player.setCurrentRoom(map.getStartingRoom());
};

void Game::run() {

    const float tileSize = 64.f;

    sf::RenderWindow window(
        sf::VideoMode({ 1920, 1080 }),
        "Dungeon Crawler"
    );

    sf::RectangleShape playerSprite({ 40.f, 40.f });
    playerSprite.setPosition({ 80.f, 80.f });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Movement
        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= 2.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += 2.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= 2.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += 2.f;

        //Collision check before moving
        sf::FloatRect newBounds = playerSprite.getGlobalBounds();

        newBounds.position += movement;

        Room* currentRoom = player.getCurrentRoom();
        
        if (canMove(newBounds, currentRoom)) {
            playerSprite.move(movement);
        }

        // Draw
        window.clear();

        const auto& layout = currentRoom->getLayout();

        for (std::size_t y = 0; y < layout.size(); y++) {
            for (std::size_t x = 0; x < layout[y].size(); x++) {
                
                sf::RectangleShape tile({ tileSize - 2.f, tileSize - 2.f });

                tile.setPosition({
                    x * tileSize,
                    y * tileSize
                    });
                
                if (layout[y][x] == TileType::Wall) {
                    tile.setFillColor(sf::Color(80, 80, 80));
                }
                else if (layout[y][x] == TileType::Floor) {
                    tile.setFillColor(sf::Color(150, 150, 150));
                }
                else if (layout[y][x] == TileType::Door) {
                    tile.setFillColor(sf::Color(150, 100, 50));
                }

                window.draw(tile);

            }
        }

        window.draw(playerSprite);

        window.display();
    }
};

bool Game::canMove(const sf::FloatRect& playerBounds, Room* room) {
        const auto& layout = room->getLayout();
        const float tileSize = 64.f;

        //Find tiles occupied by player
        int leftTile = 
            static_cast<int>(playerBounds.position.x / tileSize);

        int rightTile = 
            static_cast<int>((playerBounds.position.x + playerBounds.size.x) / tileSize);

        int topTile =
            static_cast<int>(playerBounds.position.y / tileSize);

        int bottomTile =
            static_cast<int>((playerBounds.position.y + playerBounds.size.y) / tileSize);

        //Is player outside of map?
        if (leftTile < 0 || rightTile >= static_cast<int>(layout[0].size()) ||
            topTile < 0 || bottomTile >= static_cast<int>(layout.size())) {

            return false;
        }

        //Check if tile is a wall
        for (int y = topTile; y <= bottomTile; y++) {
            for (int x = leftTile; x <= rightTile; x++) {
                if (layout[y][x] == TileType::Wall) {
                    return false;
                }
            }
        }

        return true;
}