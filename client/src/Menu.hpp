/*
** EPITECH PROJECT, 2024
** r-type [WSL: Ubuntu]
** File description:
** Menu
*/

#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include "UDPClient.hpp"
#include "Config.hpp"
#include "Scene.hpp"
#include <vector>
#include <memory>
#include <mutex>
#include <functional>

namespace rType {
    struct KeyBinding {
        sf::Keyboard::Key up = sf::Keyboard::Up;
        sf::Keyboard::Key down = sf::Keyboard::Down;
        sf::Keyboard::Key left = sf::Keyboard::Left;
        sf::Keyboard::Key right = sf::Keyboard::Right;
        sf::Keyboard::Key shoot = sf::Keyboard::Space;
    };

    class Client;
    class Menu {
        public:
            Menu(std::shared_ptr<UDPClient> client, std::shared_ptr<std::atomic<bool>> inRoom, std::shared_ptr<std::atomic<bool>> gameEnded);
            ~Menu();

            void renderConnectionMenu(sf::RenderWindow &window);
            void updateRoomList(const std::vector<uint8_t>& rooms);
            void renderRoomMenu(sf::RenderWindow &window);
            void updatePlayersText(const std::vector<std::string> &players);
            void renderMainMenu(sf::RenderWindow &window);
            void updateEndText(uint8_t win);
            void renderEnd(sf::RenderWindow &window);
            uint8_t getSelectedLevel() const { return _selectedLevel; }
            void toggleSettings() { _showSettings = !_showSettings; }
            bool isShowingSettings() const { return _showSettings; }
            int getVolume() const { return _volume; }
            using VolumeCallback = std::function<void(int)>;
            void setVolumeChangeCallback(VolumeCallback callback) { _volumeCallback = callback; }
            void setKeyBindCallback(std::function<void(const KeyBinding&)> callback) { _keyBindCallback = callback; }
            const KeyBinding& getKeyBindings() const { return _keyBindings; }

        private:
            void setupText(sf::Text& text, const std::string& content, int size, const sf::Vector2f& position, bool center);
            void initializeTextComponents();
            void handleMouseHover(sf::Text& text, const sf::Event::MouseMoveEvent& mouse);
            void drawTextWithBorder(sf::RenderWindow& window, sf::Text& text);
            void updateLevelText();
            void updateVolumeCursor();
            void drawKeyBindings(sf::RenderWindow& window);
            std::string getKeyName(sf::Keyboard::Key key);
            void handleKeyBinding(sf::Event::KeyEvent key);

            std::shared_ptr<UDPClient> _client;
            std::shared_ptr<std::atomic<bool>> _inRoom;
            std::shared_ptr<std::atomic<bool>> _gameEnded;

            std::mutex _mutex;

            sf::Event _event;
            sf::Font _font;
            sf::RectangleShape _textBorder;
            sf::Texture _menuBackground;
            sf::Sprite _menuBackgroundSprite;

            sf::Text _titleText;
            sf::Text _usernameText;
            sf::Text _playersText;
            sf::Text _startGameText;
            sf::Text _connectText;
            sf::Text _leaveText;
            sf::Text _createRoomText;
            sf::Text _roomListTitle;
            sf::Text _backToMenuText;
            sf::Text _listRoomsText;
            sf::Text _returnToLobbyText;
            sf::Text _endText;
            sf::Text _levelText;
            sf::Text _levelLeftArrow;
            sf::Text _levelRightArrow;
            sf::Text _settingsButton;
            sf::Text _settingsTitle;
            sf::Text _volumeText;
            sf::Text _volumeValue;
            sf::Text _volumeDown;
            sf::Text _volumeUp;
            sf::Text _backFromSettings;

            sf::RectangleShape _volumeSlider;
            sf::RectangleShape _volumeCursor;
            bool _isDraggingVolume{false};
            const float VOLUME_SLIDER_WIDTH = 300.f;
            const float VOLUME_SLIDER_HEIGHT = 10.f;
            const float VOLUME_CURSOR_SIZE = 20.f;

            std::vector<sf::Text> _playerTexts;
            std::vector<sf::Text> _roomTexts;
            std::vector<std::size_t> _roomIds;

            bool _showRoomList{false};
            bool _isListingRooms{false};
            bool _showSettings{false};
            uint8_t _selectedLevel{1};
            int _volume{50};  // 0-100
            VolumeCallback _volumeCallback;

            KeyBinding _keyBindings;
            std::function<void(const KeyBinding&)> _keyBindCallback;
            sf::Text _keyBindTitle;
            sf::Text _upKeyText;
            sf::Text _downKeyText;
            sf::Text _leftKeyText;
            sf::Text _rightKeyText;
            sf::Text _shootKeyText;
            int _selectedBinding = -1;
            sf::Text _backFromSettingsText;
    };
}

#endif // MENU_HPP
