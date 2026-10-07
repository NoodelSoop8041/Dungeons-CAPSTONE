#include "game.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <optional>
#include <filesystem>
#include <stdexcept>


namespace {
    int calculateDamage(int attackerStrength, int defenderDefense) {
		int damage = attackerStrength - defenderDefense;
        if (damage < 1) {
            damage = 1;
        }
        return damage;
    }
}


Game::Game() {

    if (!font.openFromFile("assets/font.ttf")) {
		throw std::runtime_error("Failed to load assets/font.ttf (working directory: " +
			std::filesystem::current_path().string() + ")");
    }

    player.setCurrentRoom(map.getStartingRoom());

    window.create(
		sf::VideoMode({ roomWidth, roomHeight + hudHeight }),
		"Dungeon Crawler"
	);

    window.setFramerateLimit(60);

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
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape && state != GameState::Playing) {
                reset();
            }
        }
    }
}

void Game::reset() {
	player = Character();
    map = Map();
	player.setCurrentRoom(map.getStartingRoom());
	playerSprite.setPosition({2.f * tileSize, 2.f * tileSize});
    state = GameState::Playing;
}

//Combat helpers
bool Game::isInRange(const sf::Vector2f first, const sf::Vector2f second, float range) const {
    sf::Vector2f difference = second - first;

    float distance = std::sqrt(
        difference.x * difference.x +
        difference.y * difference.y
    );
    
    return distance <= range;
}



void Game::update(float deltaTime) {
    if (state != GameState::Playing) { return; }

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

    //Enemies

    Room* room = player.getCurrentRoom();
    std::vector<Enemy>& enemies = room->getEnemies();

    sf::Vector2f playerCenter = playerSprite.getGlobalBounds().getCenter();

    const float enemySpeed = 50.0f;
    const float enemyStep = enemySpeed * deltaTime;
    const float enemyAttackRange = 40.0f;


    //Enemy attack

    for (Enemy& enemy : enemies) {
        if (enemy.isAlive()) {

            sf::Vector2f enemyMove = enemy.getMoveTowards(playerCenter, enemyStep);

            sf::FloatRect enemyBounds = enemy.getBounds();
            enemyBounds.position += enemyMove;


            if (canMove(enemyBounds, room)) {
                enemy.move(enemyMove);
            }


            sf::Vector2f enemyCenter = enemy.getBounds().getCenter();
            enemy.updateAttackCooldown(deltaTime);

            if (enemy.canAttack() && isInRange(enemyCenter, playerCenter, enemyAttackRange)) {

				const int damage = calculateDamage(enemy.getStrength(), player.getDefense());
                enemy.attack(player, damage);
                enemy.resetAttackCooldown();
            }


        }
    }

    const float playerAttackRange = 50.f;
    const float knockbackDistance = 20.f;

    player.updateAttackCooldown(deltaTime);

    bool attackPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    if (attackPressed && player.canAttack()) {

        player.resetAttackCooldown();

        for (Enemy& enemy : enemies) {
            if (!enemy.isAlive()) { continue; }

            sf::Vector2f enemyCenter = enemy.getBounds().getCenter();


            if (isInRange(playerCenter, enemyCenter, playerAttackRange)) {
                
                const int playerDamage = 
                    calculateDamage(player.getStrength(), enemy.getDefense());

                enemy.takeDamage(playerDamage);

                if (!enemy.isAlive()) {
                    player.increaseExperience(enemy.getXpReward());
                    continue;
                }

                sf::Vector2f dir = enemyCenter - playerCenter;
                float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

                if (length > 0.f) {
                    dir /= length;
                    sf::Vector2f offset = dir * knockbackDistance;

                    sf::FloatRect proposed = enemy.getBounds();
                    proposed.position += offset;

                    if (canMove(proposed, room)) {
                        enemy.move(offset);
                    }
                }
            }
        }
    }

    enemySprites.clear();


    for (const Enemy& enemy : enemies) {

        if (enemy.isAlive())
        {
            sf::CircleShape sprite(15.f);
			if (enemy.isBoss()) {
				sprite.setFillColor(sf::Color::Magenta);
			}
            else {
                sprite.setFillColor(sf::Color::Red);
            }
            sprite.setPosition(enemy.getPosition());

            enemySprites.push_back(sprite);
        }
    }

    if (player.getHealth() <= 0) {
		state = GameState::Lost;
    }
}

void Game::drawHud() {

    sf::RectangleShape bar({ static_cast<float>(roomWidth), static_cast<float>(hudHeight) });
	bar.setPosition({ 0.f, static_cast<float>(roomHeight) });
    bar.setFillColor(sf::Color(20, 20, 20));
    window.draw(bar);

    sf::Text text(font, "", 20);
	text.setPosition({ 10.f, roomHeight + 10.f });

	std::string hudString = "Health: " + std::to_string(player.getHealth()) + "/" + std::to_string(player.getMaxHealth()) +
		" | Level: " + std::to_string(player.getLevel()) +
		" | Experience: " + std::to_string(player.getExperience()) + "/" + std::to_string(player.getLevelThreshold());

	text.setString(hudString);
    window.draw(text);
}

void Game::drawOverlay(const std::string& message) {

    sf::RectangleShape dim({ static_cast<float>(roomWidth), static_cast<float>(roomHeight) });
    dim.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(dim);
    sf::Text text(font, message, 30);
    text.setFillColor(sf::Color::White);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ roomWidth / 2.f, roomHeight / 2.f });
    window.draw(text);
}

void Game::render() {

    window.clear();

    Room* room = player.getCurrentRoom();

    const auto& layout = room->getLayout();

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

    //Enemy render
    for (const sf::CircleShape& sprite : enemySprites)
    {
        window.draw(sprite);
    }

    drawHud();

    if (state == GameState::Lost) { drawOverlay("YOU DIED : press Esc to Restart"); }

    window.display();
}



bool Game::canMove(const sf::FloatRect& bounds, Room* room) const {
        const auto& layout = room->getLayout();

        //Find tiles occupied by player/enemy
        int leftTile = 
            static_cast<int>(bounds.position.x / tileSize);

        int rightTile = 
            static_cast<int>((bounds.position.x + bounds.size.x) / tileSize);

        int topTile =
            static_cast<int>(bounds.position.y / tileSize);

        int bottomTile =
            static_cast<int>((bounds.position.y + bounds.size.y) / tileSize);

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

bool Game::checkDoor(const sf::FloatRect& playerBounds, Room* room) const {
    const auto& layout = room->getLayout();

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

Room* Game::getNextRoom(const sf::FloatRect& playerBounds, Room* room) const {

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

        player.setCurrentRoom(newRoom);

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

std::string Game::getDoorDirection(const sf::FloatRect& playerBounds, Room* room) const {
    
    const auto& layout = room->getLayout();

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


