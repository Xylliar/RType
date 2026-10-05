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
#include <vector>
#include <memory>

struct PlayerScore {
    std::string name;
    int score;
};

namespace rType {
    struct PlayerScore {
        std::string name;
        uint16_t score;
    };

    class Menu {
        public:
            Menu(std::shared_ptr<UDPClient> client, std::shared_ptr<std::atomic<bool>> inRoom, std::shared_ptr<std::atomic<bool>> gameEnded);
            ~Menu();

            void renderConnectionMenu(sf::RenderWindow &window);
            void updateRoomList(const std::vector<uint8_t>& rooms);
            void renderRoomMenu(sf::RenderWindow &window);
            void updateGameStatus(bool status);
            void updatePlayersText(const std::vector<std::string> &players);
            void renderMainMenu(sf::RenderWindow &window);
            void updateEndText(bool win);
            void renderEnd(sf::RenderWindow &window);
            void updateScores(const std::map<uint8_t, PlayerScore> &scores);
            void drawPlayerScores(sf::RenderWindow &window);

        private:
            void setupText(sf::Text& text, const std::string& content, int size, const sf::Vector2f& position);
            void initializeTextComponents();
            void handleMouseHover(sf::Text& text, const sf::Event::MouseMoveEvent& mouse);
            void drawTextWithBorder(sf::RenderWindow& window, sf::Text& text);

            std::shared_ptr<UDPClient> _client;
            std::shared_ptr<std::atomic<bool>> _inRoom;
            std::shared_ptr<std::atomic<bool>> _gameEnded;

            sf::Event _event;
            sf::Font _font;
            sf::RectangleShape _textBorder;

            sf::Text _titleText;
            sf::Text _usernameText;
            sf::Text _playersText;
            sf::Text _startGameText;
            sf::Text _connectText;
            sf::Text _endText;
            sf::Text _leaveText;
            sf::Text _createRoomText;
            sf::Text _roomListTitle;
            sf::Text _backToMenuText;
            sf::Text _listRoomsText;
            sf::Text _returnToLobbyText;

            std::vector<sf::Text> _playerTexts;
            std::vector<sf::Text> _roomTexts;
            std::vector<std::size_t> _roomIds;
            std::vector<std::pair<uint16_t, uint16_t>> _scores;
            std::vector<PlayerScore> _playerScores;


            bool _gameStarting{false};
            bool _showRoomList{false};
            bool _isListingRooms{false};
    };
}

#endif // MENU_HPP
