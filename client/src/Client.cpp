/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Client
*/

#include "Client.hpp"
#include "Menu.hpp"

rType::Client::Client(const std::string &ip, std::size_t port, const std::string &username)
    : _networkClient(std::make_shared<UDPClient>(ip, port, username)),
    _window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type"),
    _inRoom(std::make_shared<std::atomic<bool>>(false)),
    _gameEnded(std::make_shared<std::atomic<bool>>(false)),
    _menu(std::make_unique<Menu>(_networkClient, _inRoom, _gameEnded)),
    _scene("./config/background_scenes.config"),
    _explosionSoundPool("./assets/explosion.mp3", 5),
    _playerShootSoundPool("./assets/shootPlayer.mp3", 4, 40.0f),
    _enemyShootSoundPool("./assets/shootBoss.mp3", 3),
    _baseEnemyShootSoundPool("./assets/shotgun.mp3", 5, 40.0f),
    _powerUpSoundPool("./assets/shieldFortnite.mp3", 1)
{
    _window.setFramerateLimit(60);

    if (!_textures[0x15].first.loadFromFile("./assets/player1.gif")
    || !_textures[0x16].first.loadFromFile("./assets/ship.gif")
    || !_textures[0x17].first.loadFromFile("./assets/runningEnemy.gif")
    || !_textures[0x18].first.loadFromFile("./assets/bullet.gif")
    || !_textures[0x19].first.loadFromFile("./assets/enemy_bullet.gif")
    || !_textures[0x20].first.loadFromFile("./assets/boss1.gif")
    || !_textures[0x21].first.loadFromFile("./assets/r-typesheet5.gif")
    || !_textures[0x22].first.loadFromFile("./assets/boss_bullet.gif")
    || !_textures[0x23].first.loadFromFile("./assets/boss2.gif")
    || !_textures[0x24].first.loadFromFile("./assets/boss2_bullet.gif")
    || !_textures[0x25].first.loadFromFile("./assets/powerUpDamage.png")
    || !_textures[0x26].first.loadFromFile("./assets/powerUpSpeed.png")
    || !_textures[0x27].first.loadFromFile("./assets/powerUpHealth.png")
    || !_textures[0x41].first.loadFromFile("./assets/explosion.gif")
    || !_textures[0x42].first.loadFromFile("./assets/animation_shoot.gif")
    || !_textures[0x43].first.loadFromFile("./assets/animation_shoot_enemy.gif")
    || !_textures[0x44].first.loadFromFile("./assets/animation_power_up.gif")
    || !_topAndBottomTexture.loadFromFile("./assets/top_bottom_level_one.gif")
    || !_backgroundTexture.loadFromFile("./assets/background.jpg")) {
        throw rType::TextureError("Error loading textures");
    }

    if (!_gameFont.loadFromFile("./assets/arial.ttf")) {
        throw rType::TextureError("Error loading game font");
    }
    _noticeText.setFont(_gameFont);
    _noticeText.setCharacterSize(30);
    _noticeText.setFillColor(sf::Color(255, 200, 80));
    _noticeText.setOutlineColor(sf::Color::Black);
    _noticeText.setOutlineThickness(2);

    try {
        if (!loadSoundBuffer(_menuBuffer, "./assets/menu.wav")) {
            std::cerr << "Warning: Failed to load menu sound" << std::endl;
        } else {
            _menuSound.setBuffer(_menuBuffer);
            _menuSound.setVolume(50.0f);
            _menuSound.setLoop(true);
        }
        if (!loadSoundBuffer(_victoryBuffer, "./assets/victory.mp3")) {
            std::cerr << "Warning: Failed to load victory sound" << std::endl;
        } else {
            _victorySound.setBuffer(_victoryBuffer);
            _victorySound.setVolume(20.0f);
        }
        if (!loadSoundBuffer(_defeatBuffer, "./assets/lose.mp3")) {
            std::cerr << "Warning: Failed to load victory sound" << std::endl;
        } else {
            _defeatSound.setBuffer(_defeatBuffer);
            _defeatSound.setVolume(20.0f);
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << std::endl;
    }

    for (int i = 0; i < 2; i++) {
        _backgroundSprites[i].setTexture(_backgroundTexture);
        sf::Vector2u textureSize = _backgroundTexture.getSize();
        float scaleX = static_cast<float>(WINDOW_WIDTH) / textureSize.x;
        float scaleY = static_cast<float>(WINDOW_HEIGHT) / textureSize.y;
        _backgroundSprites[i].setScale(scaleX, scaleY);
        _backgroundPositions[i] = i * WINDOW_WIDTH;
        _topAndBottomSprites[i].setTexture(_topAndBottomTexture);
        sf::Vector2u textureSize2 = _topAndBottomTexture.getSize();
        float scaleX2 = static_cast<float>(WINDOW_WIDTH) / textureSize2.x;
        float scaleY2 = static_cast<float>(WINDOW_HEIGHT) / textureSize2.y;
        _topAndBottomSprites[i].setScale(scaleX2, scaleY2);
        _topAndBottomPositions[i] = i * WINDOW_WIDTH;
    }

    _chargeBar.setFillColor(sf::Color::Yellow);
    _chargeBar.setOutlineColor(sf::Color::White);
    _chargeBar.setOutlineThickness(1);
    _chargeBar.setSize(sf::Vector2f(0, CHARGE_BAR_HEIGHT));

    command_map_t commands = {
        {0x01, std::bind(&Client::confirmConnection, this, std::placeholders::_1)},
        {0x02, std::bind(&Client::confirmDisconnection, this, std::placeholders::_1)},
        {0x03, std::bind(&Client::getRoomList, this, std::placeholders::_1)},
        {0x05, std::bind(&Client::getPlayersList, this, std::placeholders::_1)},
        {0x06, std::bind(&Client::getGameCountdown, this, std::placeholders::_1)},
        {0x08, std::bind(&Client::changePort, this, std::placeholders::_1)},
        {0x14, std::bind(&Client::getEntities, this, std::placeholders::_1)},
        {0x40, std::bind(&Client::getAnimations, this, std::placeholders::_1)},
        {0x51, std::bind(&Client::handleEndGame, this, std::placeholders::_1)},
        {0x52, std::bind(&Client::handlePlayerLeft, this, std::placeholders::_1)},
    };
    _networkClient->start(commands);

    _menu->setVolumeChangeCallback([this](int volume) {
        _menuSound.setVolume(static_cast<float>(volume));
    });

    _menu->setKeyBindCallback([this](const KeyBinding& bindings) {
        _keyBindings = bindings;
    });
}

rType::Client::~Client()
{
    _networkClient->stop();
    if (_window.isOpen())
        _window.close();
}

namespace {
    sf::Color playerColor(uint8_t playerId)
    {
        static const sf::Color colors[] = {
            sf::Color::White,
            sf::Color(255, 120, 120),
            sf::Color(120, 255, 120),
            sf::Color(255, 230, 100),
        };
        return colors[(playerId + 3) % 4];
    }
}

void rType::Client::confirmConnection(const Datatype &)
{
    _connected = true;
}

void rType::Client::setTextureBackground(const uint8_t level)
{
    const auto &backgrounds = _scene.getScenes();
    if (level == 0 || level > backgrounds.size()) {
        return;
    }

    _backgroundTexture.loadFromFile(backgrounds[level - 1]);
    for (int i = 0; i < 2; i++) {
        _backgroundSprites[i].setTexture(_backgroundTexture);
        sf::Vector2u textureSize = _backgroundTexture.getSize();
        float scaleX = static_cast<float>(WINDOW_WIDTH) / textureSize.x;
        float scaleY = static_cast<float>(WINDOW_HEIGHT) / textureSize.y;
        _backgroundSprites[i].setScale(scaleX, scaleY);
        _backgroundPositions[i] = i * WINDOW_WIDTH;

    }
};

void rType::Client::confirmDisconnection(const Datatype &)
{
    _connected = false;
    *_gameEnded = false;
    _gameStarted = false;
    *_inRoom = false;
    _networkClient->updatePort(0);
    std::cout << "Disconnected from " << _networkClient->getIp() << ":" << _networkClient->getPort() << std::endl;
}

void rType::Client::getPlayersList(const Datatype &data)
{
    std::map<uint8_t, std::string> players;
    const auto &payload = data.payload;
    std::size_t i = 0;
    while (i < payload.size()) {
        uint8_t id = payload[i++];
        std::string username;
        while (i < payload.size() && payload[i] != '\0') {
            username += static_cast<char>(payload[i++]);
        }
        i++;
        players[id] = username;
    }

    *_inRoom = true;
    std::vector<std::string> playerNames;
    {
        std::lock_guard<std::mutex> lock(_stateMutex);
        _playersList = players;
        _healthBars.clear();
        _playerLabels.clear();
        for (std::size_t i = 0; i < _playersList.size(); ++i) {
            sf::RectangleShape healthBar;
            healthBar.setSize(sf::Vector2f(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT));
            healthBar.setFillColor(sf::Color::Green);
            healthBar.setOutlineColor(sf::Color::White);
            healthBar.setOutlineThickness(2);
            healthBar.setPosition(10.0f + (i * HEALTH_BAR_SPACING), HEALTH_BAR_Y_POSITION);
            _healthBars.emplace_back(healthBar);

            sf::Text playerLabel;
            playerLabel.setFont(_gameFont);
            playerLabel.setCharacterSize(16);
            playerLabel.setFillColor(sf::Color::White);
            playerLabel.setPosition(10.0f + (i * HEALTH_BAR_SPACING), HEALTH_BAR_Y_POSITION - 20.0f);
            _playerLabels.emplace_back(playerLabel);
        }
        for (const auto& player : _playersList) {
            playerNames.emplace_back(player.second);
        }
    }
    _menu->updatePlayersText(playerNames);
}

void rType::Client::getGameCountdown(const Datatype &data)
{
    if (data.payload.empty()) {
        return;
    }
    _level = data.payload[0];
    _gameStarted = true;
}

void rType::Client::changePort(const Datatype &data)
{
    if (data.payload.size() < 2) {
        return;
    }
    std::size_t port = data.payload[0] << 8 | data.payload[1];
    _networkClient->updatePort(port);
    _networkClient->connect();
}

void rType::Client::getEntities(const Datatype &data)
{
    std::vector<Displayable> entities;
    const auto &payload = data.payload;
    std::size_t i = 0;
    entities.reserve(payload.size() / 5);
    while (i + 5 <= payload.size()) {
        Displayable entity{};
        entity.id = payload[i];
        entity.x = (payload[i + 1] << 8) | payload[i + 2];
        entity.y = (payload[i + 3] << 8) | payload[i + 4];
        i += 5;
        if (entity.id == 0x15) {
            if (i + 2 > payload.size()) {
                break;
            }
            entity.health = payload[i];
            entity.playerId = payload[i + 1];
            i += 2;
        }
        entities.emplace_back(entity);
    }
    std::lock_guard<std::mutex> lock(_stateMutex);
    _entities = std::move(entities);
}

void rType::Client::getAnimations(const Datatype &data)
{
    std::vector<Animation> animations;
    for (std::size_t i = 0; i + 6 <= data.payload.size(); i += 6) {
        uint8_t id = data.payload[i];
        uint8_t entityId = data.payload[i + 1];
        uint16_t x = (data.payload[i + 2] << 8) | data.payload[i + 3];
        uint16_t y = (data.payload[i + 4] << 8) | data.payload[i + 5];
        switch (id) {
            case 0x41:
                animations.emplace_back(Animation{id, entityId, x, y, 3});
                _explosionSoundPool.play();
                break;
            case 0x22:
                animations.emplace_back(Animation{0x42, entityId, x, y, 2});
                _enemyShootSoundPool.play();
                break;
            case 0x42:
                animations.emplace_back(Animation{id, entityId, x, y, 2});
                _playerShootSoundPool.play();
                break;
            case 0x43:
                animations.emplace_back(Animation{id, entityId, x, y, 2});
                _baseEnemyShootSoundPool.play();
                break;
            case 0x44:
                animations.emplace_back(Animation{id, entityId, x, y, 2});
                _powerUpSoundPool.play();
                break;
            default:
                break;
        }
    }
    std::lock_guard<std::mutex> lock(_stateMutex);
    _animations.insert(_animations.end(), animations.begin(), animations.end());
}

void rType::Client::handleEndGame(const Datatype &data)
{
    const uint8_t won = data.payload.empty() ? 0 : data.payload[0];

    _gameStarted = false;
    *_gameEnded = true;
    {
        std::lock_guard<std::mutex> lock(_stateMutex);
        _entities.clear();
        _animations.clear();
        _showNotice = false;
    }
    _menu->updateEndText(won);
    _menuSound.stop();
    if (won == 1) {
        _victorySound.play();
    } else {
        _defeatSound.play();
    }
}

void rType::Client::handlePlayerLeft(const Datatype &data)
{
    if (data.payload.empty()) {
        return;
    }
    const uint8_t playerId = data.payload[0];
    const bool connectionLost = data.payload.size() > 1 && data.payload[1] == 1;

    std::lock_guard<std::mutex> lock(_stateMutex);
    auto it = _playersList.find(playerId);
    const std::string name = (it != _playersList.end() && !it->second.empty()) ? it->second : "A player";
    _noticeText.setString(name + (connectionLost ? " lost connection" : " left the game"));
    _noticeClock.restart();
    _showNotice = true;
}

void rType::Client::getRoomList(const Datatype &data)
{
    std::vector<uint8_t> rooms;

    *_inRoom = false;
    if (data.length != 0) {
        for (size_t i = 0; i < data.length; i++) {
            rooms.emplace_back(data.payload[i]);
        }
    }
    _menu->updateRoomList(rooms);
}

void rType::Client::sendInput(bool shoot, uint8_t input)
{
    Datatype data;

    if (shoot) {
        data.id = 0x7A;
        data.length = 0;
        _playerCharge = 0.0f;
    } else {
        data.id = 0x79;
        data.length = 1;
        data.payload.emplace_back(input);
    }
    _networkClient->send(data);
}

void rType::Client::handleEvents()
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window.close();
        } else if (_event.type == sf::Event::KeyPressed) {
            if (_event.key.code == _keyBindings.up) _inputState.up = true;
            else if (_event.key.code == _keyBindings.down) _inputState.down = true;
            else if (_event.key.code == _keyBindings.left) _inputState.left = true;
            else if (_event.key.code == _keyBindings.right) _inputState.right = true;
            else if (_event.key.code == _keyBindings.shoot) _inputState.shoot = true;
        } else if (_event.type == sf::Event::KeyReleased) {
            if (_event.key.code == _keyBindings.up) _inputState.up = false;
            else if (_event.key.code == _keyBindings.down) _inputState.down = false;
            else if (_event.key.code == _keyBindings.left) _inputState.left = false;
            else if (_event.key.code == _keyBindings.right) _inputState.right = false;
            else if (_event.key.code == _keyBindings.shoot) _inputState.shoot = false;
        }
    }

    if (_inputClock.getElapsedTime().asSeconds() >= INPUT_UPDATE_INTERVAL) {
        if (_inputState.up) sendInput(false, 1);
        if (_inputState.down) sendInput(false, 3);
        if (_inputState.left) sendInput(false, 4);
        if (_inputState.right) sendInput(false, 2);
        if (_inputState.shoot) sendInput(true, 0);

        _inputClock.restart();
    }
}

void rType::Client::drawEntities()
{
    for (const auto &entity : _entities) {
        try {
            auto textureIt = _textures.find(entity.id);
            if (textureIt != _textures.end() && textureIt->second.first.getSize().x > 0) {
                sf::Sprite sprite(textureIt->second.first, textureIt->second.second);
                sprite.setPosition(entity.x, entity.y);
                if (entity.id == 0x15) {
                    sprite.setColor(playerColor(entity.playerId));
                }
                _window.draw(sprite);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error drawing entity " << static_cast<int>(entity.id) << ": " << e.what() << std::endl;
        }
    }

    for (const auto &anim : _animations) {
        auto textureIt = _textures.find(anim.id);
        auto textureIt2 = _textures.find(anim.entityId);

        if (textureIt != _textures.end() && textureIt->second.first.getSize().x > 0 && textureIt2 != _textures.end()) {
            sf::Sprite sprite(textureIt->second.first, textureIt->second.second);
            const sf::Vector2u entityTextureSize = textureIt2->second.first.getSize();
            sprite.setScale(
                static_cast<float>(entityTextureSize.x) / textureIt->second.first.getSize().x,
                static_cast<float>(entityTextureSize.y) / textureIt->second.first.getSize().y
            );
            sprite.setPosition(anim.x, anim.y);
            _window.draw(sprite);
        }
    }

    float chargePercent = _playerCharge / CHARGE_TIME;
    _chargeBar.setSize(sf::Vector2f(CHARGE_BAR_WIDTH * chargePercent, CHARGE_BAR_HEIGHT));

    float barX = (WINDOW_WIDTH - CHARGE_BAR_WIDTH) / 2.0f;
    float barY = WINDOW_HEIGHT - CHARGE_BAR_HEIGHT - CHARGE_BAR_Y_OFFSET;
    _chargeBar.setPosition(barX, barY);

    if (chargePercent < 0.3f) {
        _chargeBar.setFillColor(sf::Color::Red);
    } else if (chargePercent < 0.7f) {
        _chargeBar.setFillColor(sf::Color::Yellow);
    } else {
        _chargeBar.setFillColor(sf::Color::Green);
    }
    _window.draw(_chargeBar);
}

void rType::Client::handleAnimations()
{
    if (_animClock.getElapsedTime().asMilliseconds() > 1000 / 10) {
        for (auto &texture : _textures) {
            const auto &textureSize = texture.second.first.getSize();
            auto &rect = texture.second.second;
            rect.left += rect.width;
            if (rect.left + rect.width > static_cast<int>(textureSize.x)) {
                rect.left = 0;
            }
        }
        _animations.erase(
        std::remove_if(
            _animations.begin(), _animations.end(),
            [](auto &animation) {
                return --animation.timer <= 0;
            }),
        _animations.end());
        _animClock.restart();
    }

    if (_backgroundClock.getElapsedTime().asMilliseconds() > 16) {
        for (int i = 0; i < 2; i++) {
            _backgroundPositions[i] -= _scrollSpeed;
            if (_backgroundPositions[i] <= -WINDOW_WIDTH) {
                _backgroundPositions[i] = WINDOW_WIDTH;
            }
            _backgroundSprites[i].setPosition(_backgroundPositions[i], 0);
        }
        _backgroundClock.restart();
        for (int i = 0; i < 2; i++) {
            _topAndBottomPositions[i] -= _scrollSpeed;
            if (_topAndBottomPositions[i] <= -WINDOW_WIDTH) {
                _topAndBottomPositions[i] = WINDOW_WIDTH;
            }
            _topAndBottomSprites[i].setPosition(_topAndBottomPositions[i], 0);
        }
    }

    float deltaTime = _chargeClock.restart().asSeconds();
    _playerCharge = std::min(_playerCharge + deltaTime, CHARGE_TIME);
}

void rType::Client::resetClient()
{
    _connected = false;
    *_gameEnded = false;
    _gameStarted = false;
    *_inRoom = false;
    _inputState = {false, false, false, false, false};
    _networkClient->updatePort(0);
    _networkClient->connect();
}

void rType::Client::drawPlayerInfo()
{
    std::size_t slot = 0;
    for (const auto &entity : _entities) {
        if (entity.id != 0x15 || slot >= _playerLabels.size()) {
            continue;
        }
        sf::Text &label = _playerLabels[slot];
        label.setString(_playersList[entity.playerId]);
        label.setFillColor(playerColor(entity.playerId));
        if (slot < _healthBars.size()) {
            float healthPercent = static_cast<float>(entity.health) / 100.0f;
            _healthBars[slot].setSize(sf::Vector2f(HEALTH_BAR_WIDTH * healthPercent, HEALTH_BAR_HEIGHT));
            sf::Color healthColor;
            if (healthPercent > 0.6f) healthColor = sf::Color::Green;
            else if (healthPercent > 0.3f) healthColor = sf::Color::Yellow;
            else healthColor = sf::Color::Red;
            _healthBars[slot].setFillColor(healthColor);
            _window.draw(_healthBars[slot]);
            label.setPosition(10.0f + (slot * HEALTH_BAR_SPACING), HEALTH_BAR_Y_POSITION - 20.0f);
            _window.draw(label);
        }
        label.setPosition(entity.x, entity.y - 20.0f);
        _window.draw(label);
        slot++;
    }
}

void rType::Client::displayConnectionMenu()
{
    while (_window.isOpen() && !_connected)
        _menu->renderConnectionMenu(_window);
}

void rType::Client::displayRoomMenu()
{
    if (_menuSound.getStatus() != sf::Sound::Playing)
        _menuSound.play();
    while (_window.isOpen() && !*_inRoom)
        _menu->renderRoomMenu(_window);
}

void rType::Client::displayMainMenu()
{
    while (_window.isOpen() && *_inRoom && !_gameStarted)
        _menu->renderMainMenu(_window);
}

void rType::Client::displayGame()
{
    if (_gameStarted) {
        setTextureBackground(_level);
    }
    while (_window.isOpen() && _gameStarted) {
        handleEvents();

        std::unique_lock<std::mutex> lock(_stateMutex);
        handleAnimations();
        _window.clear();

        for (const auto& sprite : _backgroundSprites) {
            _window.draw(sprite);
        }
        for (const auto &sprite: _topAndBottomSprites) {
            _window.draw(sprite);
        }

        drawEntities();
        drawPlayerInfo();
        if (_showNotice) {
            if (_noticeClock.getElapsedTime().asSeconds() < 4.f) {
                const sf::FloatRect bounds = _noticeText.getLocalBounds();
                _noticeText.setPosition((WINDOW_WIDTH - bounds.width) / 2.f, 120.f);
                _window.draw(_noticeText);
            } else {
                _showNotice = false;
            }
        }
        lock.unlock();
        _window.display();
    }
}

void rType::Client::displayEndMenu()
{
    while (_window.isOpen() && *_gameEnded)
        _menu->renderEnd(_window);
}

void rType::Client::updateVolume(int volume)
{
    _menuSound.setVolume(static_cast<float>(volume));
}

void rType::Client::displayLoop()
{
    _inputClock.restart();
    _animClock.restart();
    while (_window.isOpen()) {
        displayConnectionMenu();
        displayRoomMenu();
        displayMainMenu();
        displayGame();
        displayEndMenu();
        if (_window.isOpen()) {
            resetClient();
        }
    }
}
