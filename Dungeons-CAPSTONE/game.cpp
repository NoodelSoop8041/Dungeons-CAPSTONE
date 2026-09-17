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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            playerSprite.move({ 0.f, -2.f });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            playerSprite.move({ 0.f, 2.f });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            playerSprite.move({ -2.f, 0.f });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            playerSprite.move({ 2.f, 0.f });

        // Draw
        window.clear();

        Room* currentRoom = player.getCurrentRoom();

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

