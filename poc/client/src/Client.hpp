/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Client
*/

#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

    #include "UDPClient.hpp"
    #include <iostream>
    #include <SFML/Graphics.hpp>
    #include <vector>
    #include <functional>
    #include <map>
    #include <memory>
    #include <atomic>
    #include "Error.hpp"
    #include "Menu.hpp"
    #include "Config.hpp"

namespace rType {
    struct Displayable {
        uint8_t id;
        uint16_t x;
        uint16_t y;
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
            void handleEndGame(const Datatype &data);
            void getRoomList(const Datatype &data);

            void displayLoop();

        private:
            void sendInput(const sf::Vector2i &position);
            void handleEvents();
            void drawEntities();
            void resetClient();

            std::shared_ptr<UDPClient> _networkClient;
            sf::RenderWindow _window;
            std::shared_ptr<std::atomic<bool>> _inRoom;
            std::shared_ptr<std::atomic<bool>> _gameEnded;
            Menu _menu;
            sf::Event _event;
            std::vector<uint8_t> _playerId;
            std::atomic<bool> _connected{false};
            std::atomic<bool> _gameStarted{false};

            std::vector<Displayable> _entities;
            std::map<uint8_t, PlayerScore> _playersList;

            std::map<uint16_t, std::pair<sf::Texture, sf::IntRect>> _textures = {
                {0x15, {sf::Texture(), sf::IntRect(0, 0, 128, 128)}},
            };

            sf::Font _gameFont;
    };
};

#endif /* !CLIENT_HPP_ */
