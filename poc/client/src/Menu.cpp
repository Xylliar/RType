/*
** EPITECH PROJECT, 2024
** r-type [WSL: Ubuntu]
** File description:
** Menu
*/

#include "Menu.hpp"
#include <iostream>

rType::Menu::Menu(std::shared_ptr<UDPClient> client, std::shared_ptr<std::atomic<bool>> inRoom, std::shared_ptr<std::atomic<bool>> gameEnded)
    : _client(client), _inRoom(inRoom), _gameEnded(gameEnded)
{
    if (!_font.loadFromFile("assets/arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    _textBorder.setFillColor(sf::Color::Transparent);
    _textBorder.setOutlineColor(sf::Color(0, 255, 255, 128));
    _textBorder.setOutlineThickness(2);
    initializeTextComponents();
}

rType::Menu::~Menu()
{
}

void rType::Menu::setupText(sf::Text& text, const std::string& content,
    int size, const sf::Vector2f& position)
{
    text.setFont(_font);
    text.setFillColor(sf::Color(0, 255, 255));
    text.setOutlineColor(sf::Color::Blue);
    text.setOutlineThickness(1);
    text.setStyle(sf::Text::Bold);
    text.setString(content);
    text.setCharacterSize(size);
    text.setPosition(position);
}

void rType::Menu::initializeTextComponents()
{
    setupText(_usernameText, "Connected as: " + _client->getUsername(),
        15, {10.f, 10.f});

    setupText(_connectText, "CONNECT", 40,
        {WINDOW_WIDTH / 2.f - 100.f, 200.f});

    setupText(_titleText, "AIM-BOX", 80,
        {WINDOW_WIDTH / 2.f - 120.f, 30.f});

    setupText(_listRoomsText, "JOIN ROOM", 40,
        {WINDOW_WIDTH / 2.f - 80.f, 300.f});

    setupText(_createRoomText, "CREATE ROOM", 40,
        {WINDOW_WIDTH / 2.f - 100.f, 400.f});

    setupText(_backToMenuText, "BACK", 30,
        {50.f, WINDOW_HEIGHT - 50.f});

    setupText(_roomListTitle, "AVAILABLE ROOMS:", 30,
        {WINDOW_WIDTH / 2.f - 130.f, 150.f});

    setupText(_startGameText, "START GAME", 40,
        {WINDOW_WIDTH / 2.f - 100.f, 450.f});

    setupText(_leaveText, "LEAVE ROOM", 40,
        {WINDOW_WIDTH / 2.f - 100.f, 550.f});

    setupText(_returnToLobbyText, "RETURN TO LOBBY", 30,
        {WINDOW_WIDTH / 2.f - 100.f, 450.f});
}

void rType::Menu::handleMouseHover(sf::Text& text, const sf::Event::MouseMoveEvent& mouse)
{
    if (text.getGlobalBounds().contains(mouse.x, mouse.y)) {
        text.setFillColor(sf::Color::Yellow);
        text.setOutlineColor(sf::Color(255, 165, 0));
    } else {
        text.setFillColor(sf::Color(0, 255, 255));
        text.setOutlineColor(sf::Color::Blue);
    }
}

void rType::Menu::drawTextWithBorder(sf::RenderWindow& window, sf::Text& text)
{
    sf::FloatRect bounds = text.getGlobalBounds();
    _textBorder.setPosition(bounds.left - 10, bounds.top - 5);
    _textBorder.setSize(sf::Vector2f(bounds.width + 20, bounds.height + 10));
    window.draw(_textBorder);
    window.draw(text);
}

void rType::Menu::renderConnectionMenu(sf::RenderWindow &window)
{
    while (window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            window.close();
        } else if (_event.type == sf::Event::MouseButtonPressed) {
            if (_connectText.getGlobalBounds().contains(_event.mouseButton.x, _event.mouseButton.y)) {
                _client->connect();
            }
        } else if (_event.type == sf::Event::MouseMoved) {
            if (_connectText.getGlobalBounds().contains(_event.mouseMove.x, _event.mouseMove.y)) {
                _connectText.setFillColor(sf::Color::Yellow);
            } else {
                _connectText.setFillColor(sf::Color::White);
            }
        }
    }
    window.clear();
    window.draw(_titleText);
    window.draw(_connectText);
    window.display();
}

void rType::Menu::updateRoomList(const std::vector<uint8_t>& rooms)
{
    try {
        _roomTexts.clear();
        _roomIds.clear();
        _showRoomList = true;
        if (rooms.size() == 0) {
            return;
        }
        for (size_t i = 0; i < rooms.size(); i++) {
            sf::Text roomText;
            setupText(roomText, "ROOM #" + std::to_string(rooms[i]),
                     24, {WINDOW_WIDTH / 2.f - 30.f, 200.f + i * 40});
            _roomTexts.emplace_back(roomText);
            _roomIds.emplace_back(rooms[i]);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error updating room list: " << e.what() << std::endl;
    }
}

void rType::Menu::renderRoomMenu(sf::RenderWindow &window)
{
    while (window.pollEvent(_event)) {
        switch (_event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed: {
                auto mouse = _event.mouseButton;
                if (_listRoomsText.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    _isListingRooms = true;
                    Datatype data;
                    data.id = 0x68;
                    data.length = 0;
                    _client->send(data);
                } else if (_createRoomText.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    _client->createRoom();
                } else if (_backToMenuText.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    _isListingRooms = false;
                } else if (_isListingRooms) {
                    for (std::size_t i = 0; i < _roomTexts.size(); i++) {
                        if (_roomTexts[i].getGlobalBounds().contains(mouse.x, mouse.y)) {
                            _client->joinRoom(_roomIds[i]);
                            break;
                        }
                    }
                }
                break;
            }
            case sf::Event::MouseMoved:
                if (_isListingRooms) {
                    handleMouseHover(_backToMenuText, _event.mouseMove);
                    for (auto& roomText : _roomTexts) {
                        handleMouseHover(roomText, _event.mouseMove);
                    }
                } else {
                    handleMouseHover(_listRoomsText, _event.mouseMove);
                    handleMouseHover(_createRoomText, _event.mouseMove);
                }
                break;
            default:
                break;
        }
    }

    window.clear();
    window.draw(_titleText);
    window.draw(_usernameText);

    if (_isListingRooms) {
        window.draw(_roomListTitle);
        for (auto& roomText : _roomTexts) {
            drawTextWithBorder(window, roomText);
        }
        drawTextWithBorder(window, _backToMenuText);
    } else {
        drawTextWithBorder(window, _listRoomsText);
        drawTextWithBorder(window, _createRoomText);
    }
    window.display();
}

void rType::Menu::updatePlayersText(const std::vector<std::string> &players)
{
    try {
        _playerTexts.clear();
        for (size_t i = 0; i < players.size(); i++) {
            sf::Text playerText;
            setupText(playerText, players[i],
                24, {WINDOW_WIDTH / 2.f - 50.f, 250.f + i * 40});
            _playerTexts.push_back(playerText);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error updating player text: " << e.what() << std::endl;
    }
}

void rType::Menu::updateGameStatus(bool status)
{
    if (status) {
        _startGameText.setString("CANCEL GAME");
        _gameStarting = true;
    } else {
        _startGameText.setString("START GAME");
        _gameStarting = false;
    }
}

void rType::Menu::renderMainMenu(sf::RenderWindow &window)
{
    while (window.pollEvent(_event)) {
        switch (_event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (_startGameText.getGlobalBounds().contains(_event.mouseButton.x, _event.mouseButton.y)) {
                    _gameStarting ? _client->cancelGame() : _client->startGame();
                } else if (_leaveText.getGlobalBounds().contains(_event.mouseButton.x, _event.mouseButton.y)) {
                    _client->leaveRoom();
                    *_inRoom = false;
                }
                break;
            case sf::Event::MouseMoved:
                handleMouseHover(_startGameText, _event.mouseMove);
                handleMouseHover(_leaveText, _event.mouseMove);
                break;
            default:
                break;
        }
    }

    window.clear();
    try {
        window.clear(sf::Color(30, 30, 30));
        window.draw(_titleText);
        window.draw(_usernameText);
        for (const auto &text : _playerTexts) {
            window.draw(text);
        }
        drawTextWithBorder(window, _startGameText);
        drawTextWithBorder(window, _leaveText);
    } catch (const std::exception& e) {
        std::cerr << "Error rendering main menu: " << e.what() << std::endl;
        window.clear(sf::Color(30, 30, 30));
    }
    window.display();
}

void rType::Menu::updateEndText(bool win)
{
    if (win) {
        setupText(_endText, "VICTORY", 60,
            {WINDOW_WIDTH / 2.f - 150.f, 250.f});
    } else {
        setupText(_endText, "GAME OVER", 60,
            {WINDOW_WIDTH / 2.f - 150.f, 250.f});
    }
}

void rType::Menu::renderEnd(sf::RenderWindow &window)
{
    while (window.pollEvent(_event)) {
        switch (_event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (_returnToLobbyText.getGlobalBounds().contains(_event.mouseButton.x, _event.mouseButton.y)) {
                    *_gameEnded = false;
                    _isListingRooms = false;
                    updateGameStatus(false);
                }
                break;
            case sf::Event::MouseMoved:
                handleMouseHover(_returnToLobbyText, _event.mouseMove);
                break;
            default:
                break;
        }
    }

    window.clear();
    drawTextWithBorder(window, _endText);
    drawTextWithBorder(window, _returnToLobbyText);
    drawPlayerScores(window);
    window.display();
}

void rType::Menu::updateScores(const std::map<uint8_t, PlayerScore> &scores)
{
    _playerScores.clear();
    for (const auto &scorePair : scores) {
        PlayerScore playerScore;
        playerScore.name = scorePair.second.name;
        playerScore.score = scorePair.second.score;
        _playerScores.push_back(playerScore);
    }
}

void rType::Menu::drawPlayerScores(sf::RenderWindow &window)
{
    int yOffset = 100;
    for (const auto &playerScore : _playerScores) {
        sf::Text scoreText;
        scoreText.setFont(_font);
        scoreText.setString(playerScore.name + ": " + std::to_string(playerScore.score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Green);
        sf::FloatRect textRect = scoreText.getLocalBounds();
        scoreText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        scoreText.setPosition(window.getSize().x / 2.0f, yOffset);
        drawTextWithBorder(window, scoreText);
        yOffset += 30;
    }
}