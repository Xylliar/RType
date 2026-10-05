/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

    #include <iostream>
    #include <SFML/Graphics.hpp>
    #include <SFML/Audio.hpp>
    #include <vector>
    #include <functional>
    #include <map>
    #include <memory>
    #include <atomic>
    #include <mutex>
    #include "Error.hpp"
    #include "Menu.hpp"
    #include "SoundPool.hpp"

namespace rType {
    struct Displayable {
        uint8_t id;
        uint16_t x;
        uint16_t y;
        uint16_t health;
        uint8_t playerId;
    };

    struct Animation {
        uint8_t id;
        uint8_t entityId;
        uint16_t x;
        uint16_t y;
        int timer;
    };

    class Client {
        public:
            Client(const std::string &ip, std::size_t port, const std::string &username);
            ~Client();

            void confirmConnection(const Datatype &data);
            void confirmDisconnection(const Datatype &data);
            void getPlayersList(const Datatype &data);
            void getGameCountdown(const Datatype &data);
            void changePort(const Datatype &data);
            void getEntities(const Datatype &data);
            void getAnimations(const Datatype &data);
            void handleEndGame(const Datatype &data);
            void getRoomList(const Datatype &data);
            void handlePlayerLeft(const Datatype &data);

            void displayLoop();

        private:
            void setTextureBackground(const uint8_t level);
            void sendInput(bool shoot, uint8_t input);
            void handleEvents();
            void drawEntities();
            void handleAnimations();
            void resetClient();
            void drawPlayerInfo();
            void displayConnectionMenu();
            void displayRoomMenu();
            void displayMainMenu();
            void displayGame();
            void displayEndMenu();
            void updateVolume(int volume);

            std::shared_ptr<UDPClient> _networkClient;
            sf::RenderWindow _window;
            std::shared_ptr<std::atomic<bool>> _inRoom;
            std::shared_ptr<std::atomic<bool>> _gameEnded;
            std::unique_ptr<Menu> _menu;
            Scene _scene;

            std::map<uint8_t, std::string> _playersList;
            sf::Event _event;
            sf::Clock _animClock;
            sf::Clock _backgroundClock;
            sf::Texture _backgroundTexture;
            sf::Texture _topAndBottomTexture;
            sf::Sprite _backgroundSprites[2];
            sf::Sprite _topAndBottomSprites[2];
            float _backgroundPositions[2];
            float _topAndBottomPositions[2];
            float _scrollSpeed = 2.0f;
            std::vector<uint8_t> _playerId;
            std::atomic<bool> _connected{false};
            std::atomic<bool> _gameStarted{false};
            std::atomic<uint8_t> _level{1};

            std::mutex _stateMutex;
            sf::Text _noticeText;
            sf::Clock _noticeClock;
            bool _showNotice{false};

            std::vector<Displayable> _entities;
            std::vector<uint8_t> _players;
            std::vector<Animation> _animations;

            std::map<uint16_t, std::pair<sf::Texture, sf::IntRect>> _textures = {
                {0x15, {sf::Texture(), sf::IntRect(0, 0, 66, 34)}},
                {0x16, {sf::Texture(), sf::IntRect(0, 0, 65, 65)}},
                {0x17, {sf::Texture(), sf::IntRect(0, 0, 65, 50)}},
                {0x18, {sf::Texture(), sf::IntRect(0, 0, 34, 32)}},
                {0x19, {sf::Texture(), sf::IntRect(0, 0, 20, 15)}},
                {0x20, {sf::Texture(), sf::IntRect(0, 0, 423, 523)}},
                {0x21, {sf::Texture(), sf::IntRect(0, 0, 48, 54)}},
                {0x22, {sf::Texture(), sf::IntRect(0, 0, 28, 28)}},
                {0x23, {sf::Texture(), sf::IntRect(0, 0, 594, 186)}},
                {0x24, {sf::Texture(), sf::IntRect(0, 0, 134, 43)}},
                {0x25, {sf::Texture(), sf::IntRect(0, 0, 35, 24)}},
                {0x26, {sf::Texture(), sf::IntRect(0, 0, 24, 24)}},
                {0x27, {sf::Texture(), sf::IntRect(0, 0, 24, 24)}},
                {0x41, {sf::Texture(), sf::IntRect(0, 0, 28, 24)}},
                {0x42, {sf::Texture(), sf::IntRect(0, 0, 32, 34)}},
                {0x43, {sf::Texture(), sf::IntRect(0, 0, 32, 34)}},
                {0x44, {sf::Texture(), sf::IntRect(0, 0, 32, 34)}},
            };

            sf::Font _gameFont;
            std::vector<sf::RectangleShape> _healthBars;
            std::vector<sf::Text> _playerLabels;
            const float HEALTH_BAR_WIDTH = 100.0f;
            const float HEALTH_BAR_HEIGHT = 20.0f;
            const float HEALTH_BAR_SPACING = 180.0f;
            const float HEALTH_BAR_Y_POSITION = WINDOW_HEIGHT - 40.0f;
            const float HUD_MARGIN = 10.0f;
            const float HUD_HEIGHT = 70.0f;
            const float HUD_SPACING = 15.0f;

            sf::RectangleShape _chargeBar;
            const float CHARGE_BAR_WIDTH = 200.0f;
            const float CHARGE_BAR_HEIGHT = 15.0f;
            const float CHARGE_BAR_Y_OFFSET = 40.0f;

            sf::Sound _menuSound;
            sf::SoundBuffer _menuBuffer;
            sf::Sound _victorySound;
            sf::SoundBuffer _victoryBuffer;
            sf::Sound _defeatSound;
            sf::SoundBuffer _defeatBuffer;
            SoundPool _explosionSoundPool;
            SoundPool _playerShootSoundPool;
            SoundPool _enemyShootSoundPool;
            SoundPool _baseEnemyShootSoundPool;
            SoundPool _powerUpSoundPool;

            sf::Clock _chargeClock;
            const float CHARGE_TIME = 2.0f;
            float _playerCharge;
            sf::Clock _inputClock;
            const float INPUT_UPDATE_INTERVAL = 1.0f / 60.0f;
            struct {
                bool up = false;
                bool down = false;
                bool left = false;
                bool right = false;
                bool shoot = false;
            } _inputState;

            KeyBinding _keyBindings;
    };
};

#endif /* !CLIENT_HPP_ */
