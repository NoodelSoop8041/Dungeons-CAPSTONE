#include "game.hpp"
#include <algorithm>


Game::Game() {
    player.setCurrentRoom(map.getStartingRoom());

    window.create(
		sf::VideoMode({ 1920, 1080 }),
		"Dungeon Crawler"
	);

	playerSprite.setSize({ 40.f, 40.f });

    playerSprite.setPosition({ 2.f * tileSize, 2.f * tileSize });
};

void Game::run() {

    sf::Clock clock;

    while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
};

//Closing window (eventually will include open inventory, interaction, attack, pausing)
void Game::processEvents() {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
	}
}

void Game::update(float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);

    float speed = playerSpeed * deltaTime * 60.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { movement.y -= speed; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { movement.y += speed; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { movement.x -= speed; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { movement.x += speed; }

    Room* currentRoom = player.getCurrentRoom();

    sf::FloatRect newBounds = playerSprite.getGlobalBounds();
    newBounds.position += movement;

    if (canMove(newBounds, currentRoom)) {
        playerSprite.move(movement);
    }

	sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
    
    if (checkDoor(playerBounds, currentRoom)) {

        std::string direction = getDoorDirection(playerBounds, currentRoom);
		Room* nextRoom = getNextRoom(playerBounds, currentRoom);

        if (nextRoom != nullptr) {

            changeRoom(nextRoom, direction);
        }
    }
}

void Game::render() {
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

bool Game::checkDoor(const sf::FloatRect& playerBounds, Room* room) {
    const auto& layout = room->getLayout();
    const float tileSize = 64.f;

	int leftTile =
		static_cast<int>(playerBounds.position.x / tileSize);

    int rightTile =
		static_cast<int>((playerBounds.position.x + playerBounds.size.x) / tileSize);

    int topTile =
        static_cast<int>(playerBounds.position.y / tileSize);

    int bottomTile =
        static_cast<int>((playerBounds.position.y + playerBounds.size.y) / tileSize);

	leftTile = std::max(0, leftTile);
    rightTile = std::min(static_cast<int>(layout[0].size()) - 1, rightTile);
    topTile = std::max(0, topTile);
    bottomTile = std::min(static_cast<int>(layout.size()) - 1, bottomTile);

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {

            if (layout[y][x] == TileType::Door) {
                return true;
            }

        }
    }

    return false;
}

Room* Game::getNextRoom(const sf::FloatRect& playerBounds, Room* room) {

    std::string direction = getDoorDirection(playerBounds, room);

    //North door
    if (direction == "north") { return room->getNorth(); }
    //South door
	if (direction == "south") { return room->getSouth(); }
    //West door
	if (direction == "west") { return room->getWest(); }
    //East door
	if (direction == "east") { return room->getEast(); }

	return nullptr;
}

void Game::changeRoom(Room* newRoom, const std::string& direction) {
    if (newRoom != nullptr) {

        std::cout << "Changing room to: "
            << newRoom->getName()
            << std::endl;

        std::cout << "Direction: "
            << direction
            << std::endl;

        player.setCurrentRoom(newRoom);

        const float tileSize = 64.f;

        if (direction == "north") {
			playerSprite.setPosition({ 4 * tileSize + 12.f, 6 * tileSize });
        }
        else if (direction == "south") {
            playerSprite.setPosition({ 4 * tileSize + 12.f, 1 * tileSize });
        }
		else if (direction == "east") {
			playerSprite.setPosition({ 1 * tileSize, 3 * tileSize + 12.f });
		}
		else if (direction == "west") {
			playerSprite.setPosition({ 8 * tileSize, 3 * tileSize + 12.f });
		}
    }
}

std::string Game::getDoorDirection(const sf::FloatRect& playerBounds, Room* room) {
    
    const auto& layout = room->getLayout();
    const float tileSize = 64.f;

    int leftTile = static_cast<int>(playerBounds.position.x / tileSize);
    int rightTile = static_cast<int>((playerBounds.position.x + playerBounds.size.x) / tileSize);
    int topTile = static_cast<int>(playerBounds.position.y / tileSize);
    int bottomTile = static_cast<int>((playerBounds.position.y + playerBounds.size.y) / tileSize);
    
    leftTile = std::max(0, leftTile);
    rightTile = std::min(rightTile, static_cast<int>(layout[0].size()) - 1);
    topTile = std::max(0, topTile);
    bottomTile = std::min(bottomTile, static_cast<int>(layout.size()) - 1);

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {

            if (layout[y][x] != TileType::Door) {
                continue;
            }

            if (y == 0) { return "north"; }
            if (y == static_cast<int>(layout.size()) - 1) { return "south"; }
            if (x == 0) { return "west"; }
            if (x == static_cast<int>(layout[y].size()) - 1) { return "east"; }
        }
    }

    return "";
}
