/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** GameRoom
*/

#ifndef GAMEROOM_HPP_
    #define GAMEROOM_HPP_

    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <mutex>
    #include <memory>
    #include <atomic>
    #include <thread>
    #include <queue>
    #include "Lobby.hpp"
    #include "../../../ecs/include/Registry.hpp"
    #include "Component.hpp"

    using asio::ip::udp;

struct Command {
    std::size_t playerId;
    std::size_t posX;
    std::size_t posY;
};

void control_system(std::shared_ptr<registry> r, Command &cmd);
void init_spawn_targets(std::shared_ptr<registry> r);
void spawn_target(std::shared_ptr<registry> r);
void hit_system(std::shared_ptr<registry> r);
void target_system(std::shared_ptr<registry> r);
Datatype end_system(std::shared_ptr<registry> r);
Datatype network_system(std::shared_ptr<registry> r);

namespace rType {
    class GameRoom {
        public:
            GameRoom(std::size_t roomId, asio::io_context &context, std::shared_ptr<std::mutex> mutex, std::shared_ptr<std::atomic<bool>> running);
            ~GameRoom();
            std::size_t getRoomId() const;
            std::size_t getPort() const;
            std::size_t getNbPlayers() const;
            bool getGameStatus() const;

            void sendPlayersList();
            void acceptClient(const Datatype &);
            void removeClient(const Datatype &);
            void receiveStartGame(const Datatype &);
            void checkMovePlayer(const Datatype & data);
            void checkPlayerShoot(const Datatype &);
            void init_components();

        private:
            void loadRegistry();
            void startGame();
            void initGame();

            Lobby _lobby;
            std::size_t _roomId = 0;
            std::thread _gameThread;
            std::shared_ptr<std::atomic<bool>> _running;
            std::atomic<bool> _gameStarted{false};
            std::shared_ptr<registry> _registry;
            sf::Clock _clock;
            std::queue<Command> _commandsQueue;
            std::mutex _mutex;
    };
};

#endif // GAMEROOM_HPP_
