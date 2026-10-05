/*
** EPITECH PROJECT, 2024
** r-type [WSL: Ubuntu]
** File description:
** Menu
*/

#include "Menu.hpp"

rType::Menu::Menu(std::shared_ptr<UDPClient> client, std::shared_ptr<std::atomic<bool>> inRoom, std::shared_ptr<std::atomic<bool>> gameEnded)
    : _client(client), _inRoom(inRoom), _gameEnded(gameEnded)
{
    if (!_font.loadFromFile("assets/arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    try {
        if (!_menuBackground.loadFromFile("assets/menu_background.jpg")) {
            std::cerr << "Warning: Failed to load menu background, using fallback color" << std::endl;
        } else {
            _menuBackgroundSprite.setTexture(_menuBackground);
            sf::Vector2u textureSize = _menuBackground.getSize();
            _menuBackgroundSprite.setScale(
                static_cast<float>(WINDOW_WIDTH) / textureSize.x,
                static_cast<float>(WINDOW_HEIGHT) / textureSize.y
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << std::endl;
        throw std::runtime_error("Failed to load menu background");
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
    int size, const sf::Vector2f& position, bool center)
{
    if (text.getFont() == nullptr) {
        text.setFont(_font);
        text.setFillColor(sf::Color(0, 255, 255));
        text.setOutlineColor(sf::Color::Blue);
        text.setOutlineThickness(1);
        text.setStyle(sf::Text::Bold);
    }
    text.setString(content);
    text.setCharacterSize(size);
    if (center) {
        float averageCharWidth = size * 0.6f;
        float textWidth = content.length() * averageCharWidth;
        float textHeight = size;
        text.setOrigin(textWidth / 2.f, textHeight / 2.f);
    }
    text.setPosition(position);
}

void rType::Menu::initializeTextComponents()
{
    setupText(_titleText, "R-TYPE", FONT_SIZE_TITLE,
        {WINDOW_WIDTH / 2.f, TITLE_Y}, true);

    setupText(_usernameText, "Connected as: " + _client->getUsername(),
        FONT_SIZE_XSMALL, {PADDING_X, PADDING_Y}, false);

    setupText(_connectText, "CONNECT", FONT_SIZE_LARGE,
        {WINDOW_WIDTH / 2.f, CONTENT_CENTER_Y - BUTTON_SPACING}, true);

    setupText(_listRoomsText, "JOIN ROOM", FONT_SIZE_LARGE,
        {WINDOW_WIDTH / 2.f, CONTENT_CENTER_Y - PADDING_Y * 2}, true);

    setupText(_createRoomText, "CREATE ROOM", FONT_SIZE_LARGE,
        {WINDOW_WIDTH / 2.f, CONTENT_CENTER_Y + BUTTON_SPACING}, true);

    setupText(_backToMenuText, "BACK", FONT_SIZE_MEDIUM,
        {PADDING_X, FOOTER_Y}, false);

    setupText(_roomListTitle, "AVAILABLE ROOMS:", FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f, TITLE_Y + 100.f}, true);

    setupText(_startGameText, "START GAME", FONT_SIZE_LARGE,
        {WINDOW_WIDTH / 2.f, CONTENT_CENTER_Y + BUTTON_SPACING}, true);

    setupText(_leaveText, "LEAVE ROOM", FONT_SIZE_LARGE,
        {WINDOW_WIDTH / 2.f, CONTENT_CENTER_Y + BUTTON_SPACING * 2}, true);

    setupText(_returnToLobbyText, "RETURN TO LOBBY", FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f, CONTENT_CENTER_Y}, true);

    setupText(_levelLeftArrow, "<", FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f - 100.f, CONTENT_CENTER_Y + BUTTON_SPACING * 3}, true);

    setupText(_levelText, "lvl: 1", FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f + 50.f, CONTENT_CENTER_Y + BUTTON_SPACING * 3}, true);

    setupText(_levelRightArrow, ">", FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f + 130.f, CONTENT_CENTER_Y + BUTTON_SPACING * 3}, true);

    setupText(_settingsButton, "SETTINGS", FONT_SIZE_LARGE,
        {WINDOW_WIDTH / 2.f, CONTENT_CENTER_Y + BUTTON_SPACING * 2}, true);

    _volumeSlider.setSize(sf::Vector2f(VOLUME_SLIDER_WIDTH, VOLUME_SLIDER_HEIGHT));
    _volumeSlider.setFillColor(sf::Color(50, 50, 50));
    _volumeSlider.setOutlineColor(sf::Color::Cyan);
    _volumeSlider.setOutlineThickness(2.f);
    _volumeSlider.setPosition(
        WINDOW_WIDTH / 2.f - VOLUME_SLIDER_WIDTH / 2.f,
        CONTENT_CENTER_Y
    );

    _volumeCursor.setSize(sf::Vector2f(VOLUME_CURSOR_SIZE, VOLUME_CURSOR_SIZE * 2));
    _volumeCursor.setFillColor(sf::Color::White);
    _volumeCursor.setOutlineColor(sf::Color::Cyan);
    _volumeCursor.setOutlineThickness(2.f);
    updateVolumeCursor();

    const float KEY_START_Y = CONTENT_CENTER_Y - BUTTON_SPACING;
    const float KEY_SPACING = 50.0f;

    setupText(_keyBindTitle, "KEYBINDINGS", FONT_SIZE_LARGE,
        {WINDOW_WIDTH / 2.f, KEY_START_Y - KEY_SPACING * 3}, true);

    setupText(_upKeyText, "UP: " + getKeyName(_keyBindings.up), FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f, KEY_START_Y}, true);

    setupText(_downKeyText, "DOWN: " + getKeyName(_keyBindings.down), FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f, KEY_START_Y + KEY_SPACING}, true);

    setupText(_leftKeyText, "LEFT: " + getKeyName(_keyBindings.left), FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f, KEY_START_Y + KEY_SPACING * 2}, true);

    setupText(_rightKeyText, "RIGHT: " + getKeyName(_keyBindings.right), FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f, KEY_START_Y + KEY_SPACING * 3}, true);

    setupText(_shootKeyText, "SHOOT: " + getKeyName(_keyBindings.shoot), FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f, KEY_START_Y + KEY_SPACING * 4}, true);

    const float VOLUME_SECTION_Y = KEY_START_Y + (KEY_SPACING * 7);

    setupText(_volumeText, "VOLUME", FONT_SIZE_MEDIUM,
        {WINDOW_WIDTH / 2.f, VOLUME_SECTION_Y}, true);

    _volumeSlider.setPosition(
        WINDOW_WIDTH / 2.f - VOLUME_SLIDER_WIDTH / 2.f,
        VOLUME_SECTION_Y + KEY_SPACING * 1.5f
    );

    _volumeCursor.setSize(sf::Vector2f(VOLUME_CURSOR_SIZE, VOLUME_CURSOR_SIZE * 2));
    _volumeCursor.setFillColor(sf::Color::White);
    _volumeCursor.setOutlineColor(sf::Color::Cyan);
    _volumeCursor.setOutlineThickness(2.f);
    updateVolumeCursor();

    setupText(_backFromSettingsText, "BACK", FONT_SIZE_MEDIUM,
        {PADDING_X, WINDOW_HEIGHT - PADDING_Y * 4}, false);
}

std::string rType::Menu::getKeyName(sf::Keyboard::Key key)
{
    switch (key) {
        case sf::Keyboard::Up: return "Up";
        case sf::Keyboard::Down: return "Down";
        case sf::Keyboard::Left: return "Left";
        case sf::Keyboard::Right: return "Right";
        case sf::Keyboard::Space: return "Space";
        case sf::Keyboard::A: return "A";
        case sf::Keyboard::B: return "B";
        case sf::Keyboard::C: return "C";
        case sf::Keyboard::D: return "D";
        case sf::Keyboard::E: return "E";
        case sf::Keyboard::F: return "F";
        case sf::Keyboard::G: return "G";
        case sf::Keyboard::H: return "H";
        case sf::Keyboard::I: return "I";
        case sf::Keyboard::J: return "J";
        case sf::Keyboard::K: return "K";
        case sf::Keyboard::L: return "L";
        case sf::Keyboard::M: return "M";
        case sf::Keyboard::N: return "N";
        case sf::Keyboard::O: return "O";
        case sf::Keyboard::P: return "P";
        case sf::Keyboard::Q: return "Q";
        case sf::Keyboard::R: return "R";
        case sf::Keyboard::S: return "S";
        case sf::Keyboard::T: return "T";
        case sf::Keyboard::U: return "U";
        case sf::Keyboard::V: return "V";
        case sf::Keyboard::W: return "W";
        case sf::Keyboard::X: return "X";
        case sf::Keyboard::Y: return "Y";
        case sf::Keyboard::Z: return "Z";
        case sf::Keyboard::BackSpace: return "BackSpace";
        case sf::Keyboard::Tab: return "Tab";
        case sf::Keyboard::Return: return "Return";
        case sf::Keyboard::LShift: return "Left Shift";
        case sf::Keyboard::RShift: return "Right Shift";
        case sf::Keyboard::LControl: return "Left Ctrl";
        case sf::Keyboard::RControl: return "Right Ctrl";
        case sf::Keyboard::LAlt: return "Left Alt";
        case sf::Keyboard::RAlt: return "Right Alt";
        default: return "Unknown";
    }
}

void rType::Menu::handleKeyBinding(sf::Event::KeyEvent key)
{
    if (_selectedBinding == -1) return;

    switch (_selectedBinding) {
        case 0: _keyBindings.up = static_cast<sf::Keyboard::Key>(key.code); break;
        case 1: _keyBindings.down = static_cast<sf::Keyboard::Key>(key.code); break;
        case 2: _keyBindings.left = static_cast<sf::Keyboard::Key>(key.code); break;
        case 3: _keyBindings.right = static_cast<sf::Keyboard::Key>(key.code); break;
        case 4: _keyBindings.shoot = static_cast<sf::Keyboard::Key>(key.code); break;
    }

    _upKeyText.setString("UP: " + getKeyName(_keyBindings.up));
    _downKeyText.setString("DOWN: " + getKeyName(_keyBindings.down));
    _leftKeyText.setString("LEFT: " + getKeyName(_keyBindings.left));
    _rightKeyText.setString("RIGHT: " + getKeyName(_keyBindings.right));
    _shootKeyText.setString("SHOOT: " + getKeyName(_keyBindings.shoot));

    if (_keyBindCallback) {
        _keyBindCallback(_keyBindings);
    }

    _selectedBinding = -1;
}

void rType::Menu::updateVolumeCursor()
{
    float position = (_volume / 100.f) * (VOLUME_SLIDER_WIDTH - VOLUME_CURSOR_SIZE);
    float sliderY = _volumeSlider.getPosition().y;

    _volumeCursor.setPosition(
        _volumeSlider.getPosition().x + position,
        sliderY - (_volumeCursor.getSize().y - VOLUME_SLIDER_HEIGHT) / 2.f
    );
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
    std::lock_guard<std::mutex> lock(_mutex);
    try {
        if (_roomTexts.size() != 0) {
            _roomTexts.clear();
            _roomIds.clear();
        }
        _showRoomList = true;
        if (rooms.size() == 0) {
            return;
        }
        for (size_t i = 0; i < rooms.size(); i++) {
            sf::Text roomText;
            setupText(roomText, "ROOM #" + std::to_string(static_cast<int>(rooms[i])),
                FONT_SIZE_MEDIUM, {WINDOW_WIDTH / 2.f, 250.f + i * LIST_TEXT_SPACING}, true);
            _roomTexts.emplace_back(roomText);
            _roomIds.emplace_back(rooms[i]);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error updating room list: " << e.what() << std::endl;
    }
}

void rType::Menu::renderRoomMenu(sf::RenderWindow &window)
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (window.pollEvent(_event)) {
        switch (_event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed: {
                auto mouse = _event.mouseButton;
                if (_event.mouseButton.button == sf::Mouse::Left) {
                    if (_showSettings) {
                        sf::FloatRect sliderBounds = _volumeSlider.getGlobalBounds();
                        sliderBounds.top -= _volumeCursor.getSize().y / 2.f;
                        sliderBounds.height += _volumeCursor.getSize().y;

                        if (sliderBounds.contains(_event.mouseButton.x, _event.mouseButton.y)) {
                            _isDraggingVolume = true;
                            float relativeX = _event.mouseButton.x - _volumeSlider.getPosition().x;
                            _volume = static_cast<int>((relativeX / VOLUME_SLIDER_WIDTH) * 100);
                            _volume = std::clamp(_volume, 0, 100);
                            if (_volumeCallback) {
                                _volumeCallback(_volume);
                            }
                            updateVolumeCursor();
                        }
                    }
                }
                if (_settingsButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    _showSettings = true;
                } else if (_listRoomsText.getGlobalBounds().contains(mouse.x, mouse.y)) {
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
                if (_showSettings) {
                    if (_upKeyText.getGlobalBounds().contains(mouse.x, mouse.y)) _selectedBinding = 0;
                    else if (_downKeyText.getGlobalBounds().contains(mouse.x, mouse.y)) _selectedBinding = 1;
                    else if (_leftKeyText.getGlobalBounds().contains(mouse.x, mouse.y)) _selectedBinding = 2;
                    else if (_rightKeyText.getGlobalBounds().contains(mouse.x, mouse.y)) _selectedBinding = 3;
                    else if (_shootKeyText.getGlobalBounds().contains(mouse.x, mouse.y)) _selectedBinding = 4;
                    if (_backFromSettingsText.getGlobalBounds().contains(mouse.x, mouse.y)) {
                        _showSettings = false;
                        break;
                    }
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
                if (_event.mouseButton.button == sf::Mouse::Left) {
                    _isDraggingVolume = false;
                }
                break;
            case sf::Event::MouseMoved:
                if (_isDraggingVolume) {
                    float relativeX = _event.mouseMove.x - _volumeSlider.getPosition().x;
                    _volume = static_cast<int>((relativeX / VOLUME_SLIDER_WIDTH) * 100);
                    _volume = std::clamp(_volume, 0, 100);
                    if (_volumeCallback) {
                        _volumeCallback(_volume);
                    }
                    updateVolumeCursor();
                }
                if (_isListingRooms) {
                    handleMouseHover(_backToMenuText, _event.mouseMove);
                    for (auto& roomText : _roomTexts) {
                        handleMouseHover(roomText, _event.mouseMove);
                    }
                } else {
                    handleMouseHover(_listRoomsText, _event.mouseMove);
                    handleMouseHover(_createRoomText, _event.mouseMove);
                    handleMouseHover(_settingsButton, _event.mouseMove);
                }
                if (_showSettings) {
                    handleMouseHover(_backFromSettingsText, _event.mouseMove);
                    handleMouseHover(_upKeyText, _event.mouseMove);
                    handleMouseHover(_downKeyText, _event.mouseMove);
                    handleMouseHover(_leftKeyText, _event.mouseMove);
                    handleMouseHover(_rightKeyText, _event.mouseMove);
                    handleMouseHover(_shootKeyText, _event.mouseMove);
                }
                break;
            case sf::Event::KeyPressed:
                if (_showSettings) {
                    handleKeyBinding(_event.key);
                }
                break;
            default:
                break;
        }
    }

    window.clear();
    window.draw(_menuBackgroundSprite);
    window.draw(_titleText);
    window.draw(_usernameText);

    if (_showSettings) {
        drawTextWithBorder(window, _keyBindTitle);
        sf::Text* bindingTexts[] = {&_upKeyText, &_downKeyText, &_leftKeyText, &_rightKeyText, &_shootKeyText};
        for (int i = 0; i < 5; i++) {
            window.draw(*bindingTexts[i]);
        }
        drawTextWithBorder(window, _volumeText);
        window.draw(_volumeSlider);
        window.draw(_volumeCursor);
        drawTextWithBorder(window, _backFromSettingsText);
    } else if (_isListingRooms) {
        window.draw(_roomListTitle);
        for (auto& roomText : _roomTexts) {
            drawTextWithBorder(window, roomText);
        }
        drawTextWithBorder(window, _backToMenuText);
    } else {
        drawTextWithBorder(window, _listRoomsText);
        drawTextWithBorder(window, _createRoomText);
        drawTextWithBorder(window, _settingsButton);
    }
    lock.unlock();
    window.display();
}

void rType::Menu::drawKeyBindings(sf::RenderWindow& window)
{
    sf::Text* bindingTexts[] = {&_upKeyText, &_downKeyText, &_leftKeyText, &_rightKeyText, &_shootKeyText};

    _upKeyText.setString("UP: " + getKeyName(_keyBindings.up));
    _downKeyText.setString("DOWN: " + getKeyName(_keyBindings.down));
    _leftKeyText.setString("LEFT: " + getKeyName(_keyBindings.left));
    _rightKeyText.setString("RIGHT: " + getKeyName(_keyBindings.right));
    _shootKeyText.setString("SHOOT: " + getKeyName(_keyBindings.shoot));

    for (int i = 0; i < 5; i++) {
        if (i == _selectedBinding) {
            bindingTexts[i]->setFillColor(sf::Color::Yellow);
        } else {
            bindingTexts[i]->setFillColor(sf::Color(0, 255, 255));
        }
        if (i == _selectedBinding) {
            sf::FloatRect bounds = bindingTexts[i]->getGlobalBounds();
            sf::RectangleShape highlight;
            highlight.setSize(sf::Vector2f(bounds.width + 20, bounds.height + 10));
            highlight.setPosition(bounds.left - 10, bounds.top - 5);
            highlight.setFillColor(sf::Color(0, 0, 255, 64));
            window.draw(highlight);
        }
        window.draw(*bindingTexts[i]);
    }
}

void rType::Menu::updatePlayersText(const std::vector<std::string> &players)
{
    std::lock_guard<std::mutex> lock(_mutex);
    try {
        _playerTexts.clear();
        for (size_t i = 0; i < players.size(); i++) {
            sf::Text playerText;
            setupText(playerText, players[i], FONT_SIZE_MEDIUM,
                {WINDOW_WIDTH / 2.f, 250.f + i * LIST_TEXT_SPACING}, true);
            _playerTexts.push_back(playerText);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error updating player text: " << e.what() << std::endl;
    }
}

void rType::Menu::updateLevelText()
{
    _levelText.setString("Lvl: " + std::to_string(_selectedLevel));
}

void rType::Menu::renderMainMenu(sf::RenderWindow &window)
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (window.pollEvent(_event)) {
        switch (_event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed: {
                auto mouse = _event.mouseButton;
                if (_startGameText.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    _client->startGame(_selectedLevel);
                } else if (_leaveText.getGlobalBounds().contains(mouse.x, mouse.y)) {
                    _client->leaveRoom();
                    *_inRoom = false;
                } else if (_levelLeftArrow.getGlobalBounds().contains(mouse.x, mouse.y) && _selectedLevel > 1) {
                    _selectedLevel--;
                    updateLevelText();
                } else if (_levelRightArrow.getGlobalBounds().contains(mouse.x, mouse.y) && _selectedLevel < MAX_LEVELS) {
                    _selectedLevel++;
                    updateLevelText();
                }
                break;
            }
            case sf::Event::MouseMoved:
                handleMouseHover(_startGameText, _event.mouseMove);
                handleMouseHover(_leaveText, _event.mouseMove);
                handleMouseHover(_levelLeftArrow, _event.mouseMove);
                handleMouseHover(_levelRightArrow, _event.mouseMove);
                break;
            default:
                break;
        }
    }

    window.clear();
    try {
        if (_menuBackground.getSize().x > 0) {
            window.draw(_menuBackgroundSprite);
        } else {
            window.clear(sf::Color(30, 30, 30));
        }
        window.draw(_titleText);
        window.draw(_usernameText);
        for (const auto &text : _playerTexts) {
            window.draw(text);
        }
        drawTextWithBorder(window, _startGameText);
        drawTextWithBorder(window, _leaveText);
        drawTextWithBorder(window, _levelText);
        drawTextWithBorder(window, _levelLeftArrow);
        drawTextWithBorder(window, _levelRightArrow);
    } catch (const std::exception& e) {
        std::cerr << "Error rendering main menu: " << e.what() << std::endl;
        window.clear(sf::Color(30, 30, 30));
    }
    lock.unlock();
    window.display();
}

void rType::Menu::updateEndText(uint8_t win)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (win == 1) {
        setupText(_endText, "VICTORY", FONT_SIZE_TITLE,
            {WINDOW_WIDTH / 2.f, TITLE_Y}, true);
    } else {
        setupText(_endText, "GAME OVER", FONT_SIZE_TITLE,
            {WINDOW_WIDTH / 2.f, TITLE_Y}, true);
    }
}

void rType::Menu::renderEnd(sf::RenderWindow &window)
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (window.pollEvent(_event)) {
        switch (_event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (_returnToLobbyText.getGlobalBounds().contains(_event.mouseButton.x, _event.mouseButton.y)) {
                    *_gameEnded = false;
                    _isListingRooms = false;
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
    window.draw(_menuBackgroundSprite);
    window.draw(_endText);
    drawTextWithBorder(window, _returnToLobbyText);
    lock.unlock();
    window.display();
}
