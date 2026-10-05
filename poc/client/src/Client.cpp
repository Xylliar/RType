/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Client
*/

#include "Client.hpp"

rType::Client::Client(const std::string &ip, std::size_t port, const std::string &username)
    : _networkClient(std::make_shared<UDPClient>(ip, port, username)), _window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type"),
    _inRoom(std::make_shared<std::atomic<bool>>(false)), _gameEnded(std::make_shared<std::atomic<bool>>(false)), _menu(_networkClient, _inRoom, _gameEnded)
{
    if (!_textures[0x15].first.loadFromFile("./assets/sphere.png")) {
        throw rType::TextureError("Error loading textures");
    }

    if (!_gameFont.loadFromFile("./assets/arial.ttf")) {
        throw rType::TextureError("Error loading game font");
    }

    command_map_t commands = {
        {0x01, std::bind(&Client::confirmConnection, this, std::placeholders::_1)},
        {0x02, std::bind(&Client::confirmDisconnection, this, std::placeholders::_1)},
        {0x03, std::bind(&Client::getRoomList, this, std::placeholders::_1)},
        {0x05, std::bind(&Client::getPlayersList, this, std::placeholders::_1)},
        {0x06, std::bind(&Client::getGameCountdown, this, std::placeholders::_1)},
        {0x08, std::bind(&Client::changePort, this, std::placeholders::_1)},
        {0x14, std::bind(&Client::getEntities, this, std::placeholders::_1)},
        {0x51, std::bind(&Client::handleEndGame, this, std::placeholders::_1)},
    };
    _networkClient->start(commands);
}

rType::Client::~Client()
{
    if (_window.isOpen())
        _window.close();
}

void rType::Client::confirmConnection(const Datatype &)
{
    _connected = true;
}

void rType::Client::confirmDisconnection(const Datatype &)
{
    _connected = false;
    *_gameEnded = false;
    _gameStarted = false;
    *_inRoom = false;
    _networkClient->updatePort(0);
}

void rType::Client::getPlayersList(const Datatype &data)
{
    std::string username;
    uint8_t id = data.payload[0];

    *_inRoom = true;
    _playersList.clear();
    for (std::size_t i = 1; i < data.length; i++) {
        if (data.payload[i] == '\0') {
            _playersList[id].name = username;
            _playersList[id].score = 0;
            username.clear();
            id = data.payload[i + 1];
            i++;
        } else {
            username += data.payload[i];
        }
    }
    std::vector<std::string> playerNames;
    for (const auto& player : _playersList) {
        playerNames.emplace_back(player.second.name);
    }
    _menu.updatePlayersText(playerNames);
}

void rType::Client::getGameCountdown(const Datatype &data)
{
    std::cout << "Game starting in " << std::hex << static_cast<int>(data.payload[0]) << std::endl;
    if (data.payload[0] == 0) {
        _gameStarted = true;
    }
    _menu.updateGameStatus(true);
}

void rType::Client::changePort(const Datatype &data)
{
    std::size_t port = data.payload[0] << 8 | data.payload[1];
    _networkClient->updatePort(port);
    _networkClient->connect();
}

void rType::Client::getEntities(const Datatype &data)
{
    if (data.length % 5 != 0) {
        return;
    }
    _entities.clear();
    for (size_t i = 0; i < data.length; i += 5) {
        if (data.payload[i] == 0x16) {
            uint16_t id = (data.payload[i + 1] << 8) | data.payload[i + 2];
            uint16_t score = (data.payload[i + 3] << 8) | data.payload[i + 4];
            _playersList[id].score = score;
        } else {
            Displayable entity;
            entity.id = data.payload[i];
            entity.x = (data.payload[i + 1] << 8) | data.payload[i + 2];
            entity.y = (data.payload[i + 3] << 8) | data.payload[i + 4];
            _entities.emplace_back(entity);
        }
    }
}

void rType::Client::handleEndGame(const Datatype &data)
{
    _gameStarted = false;
    *_gameEnded = true;
    _entities.clear();
    std::vector<std::pair<uint16_t, uint16_t>> scores;
    if (data.length % 4 != 0) {
        return;
    }
    for (size_t i = 0; i < data.length; i += 4) {
        uint16_t id = (data.payload[i] << 8) | data.payload[i + 1];
        uint16_t score = (data.payload[i + 2] << 8) | data.payload[i + 3];
        _playersList[id].score = score;
    }
    _menu.updateScores(_playersList);
}

void rType::Client::getRoomList(const Datatype &data)
{
    std::vector<uint8_t> rooms;

    *_inRoom = false;
    if (data.length != 0) {
        for (size_t i = 0; i < data.length; i++) {
            rooms.push_back(data.payload[i]);
        }
    }
    _menu.updateRoomList(rooms);
}

void rType::Client::sendInput(const sf::Vector2i &position)
{
    Datatype data;
    data.id = 0x79;
    data.length = 4;

    data.payload.emplace_back((position.x >> 8) & 0xFF);
    data.payload.emplace_back(position.x & 0xFF);
    data.payload.emplace_back((position.y >> 8) & 0xFF);
    data.payload.emplace_back(position.y & 0xFF);
    _networkClient->send(data);
}

void rType::Client::handleEvents()
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _window.close();
        } if (_event.type == sf::Event::MouseButtonPressed && _event.mouseButton.button == sf::Mouse::Left) {
            sendInput(sf::Mouse::getPosition(_window));
        }
    }
}

void rType::Client::drawEntities()
{
    for (const auto &entity : _entities) {
        try {
            auto textureIt = _textures[0x15];
            sf::Sprite sprite(textureIt.first, textureIt.second);
            sprite.setPosition(entity.x, entity.y);
            _window.draw(sprite);
        } catch (const std::exception& e) {
            std::cerr << "Error drawing entity: " << e.what() << std::endl;
        }
    }
    int i = 0;
    for (const auto &player : _playersList) {
        sf::Text playerText;
        playerText.setFont(_gameFont);
        playerText.setCharacterSize(20);
        playerText.setString(player.second.name + ": " + std::to_string(player.second.score));
        playerText.setFillColor(sf::Color::Green);
        playerText.setPosition(i * 200.f, 10.f);
        _window.draw(playerText);
        i++;
    }
}

void rType::Client::resetClient()
{
    _connected = false;
    *_gameEnded = false;
    _gameStarted = false;
    *_inRoom = false;
    _entities.clear();
    _playersList.clear();
    _networkClient->updatePort(0);
    _networkClient->connect();
}

void rType::Client::displayLoop()
{
    while (_window.isOpen()) {
        while (_window.isOpen() && !_connected) {
            _menu.renderConnectionMenu(_window);
        }
        while (_window.isOpen() && !*_inRoom) {
            _menu.renderRoomMenu(_window);
        }
        while (_window.isOpen() && *_inRoom && !_gameStarted) {
            _menu.renderMainMenu(_window);
        }
        while (_window.isOpen() && _gameStarted) {
            handleEvents();
            _window.clear();

            drawEntities();
            _window.display();
        }
        while (_window.isOpen() && *_gameEnded) {
            _menu.renderEnd(_window);
        }
        if (_window.isOpen()) {
            resetClient();
        }
    }
}
