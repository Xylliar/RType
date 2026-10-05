/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** GameRoom
*/

#include "GameRoom.hpp"
#include <algorithm>
#include <iostream>
#include "ECSLoader.hpp"

namespace rType {
    GameRoom::GameRoom(std::size_t roomId, asio::io_context &context, std::shared_ptr<std::mutex> mutex, std::shared_ptr<std::atomic<bool>> running)
    : _lobby(context, mutex, roomId), _roomId(roomId), _running(running)
    {
        command_map_t cmd = {
            {0x65, std::bind(&GameRoom::acceptClient, this, std::placeholders::_1)},
            {0x66, std::bind(&GameRoom::removeClient, this, std::placeholders::_1)},
            {0x6A, std::bind(&GameRoom::removeClient, this, std::placeholders::_1)},
            {0x6B, std::bind(&GameRoom::receiveStartGame, this, std::placeholders::_1)},
            {0x79, std::bind(&GameRoom::checkPlayerShoot, this, std::placeholders::_1)},
        };
        _lobby.start(std::make_unique<command_map_t>(cmd));
    }

    GameRoom::~GameRoom()
    {
        Datatype data;
        data.id = 0x02;
        data.length = 0;

        _lobby.sendToEveryone(data);
        if (_gameThread.joinable()) {
            _gameThread.join();
        }
    }

    std::size_t GameRoom::getRoomId() const
    {
        return _roomId;
    }

    std::size_t GameRoom::getPort() const
    {
        return _lobby.getPort();
    }

    std::size_t GameRoom::getNbPlayers() const
    {
        return _lobby.getNbClients();
    }

    bool GameRoom::getGameStatus() const
    {
        return _gameStarted;
    }

    void GameRoom::sendPlayersList()
    {
        if (_gameStarted) {
            return;
        }

        Datatype data;
        data.id = 0x05;
        data.length = 0;

        for (const auto &client : _lobby.getClients()) {
            data.payload.emplace_back(client.id);
            for (char c : client.username) {
                data.payload.emplace_back(static_cast<uint8_t>(c));
                data.length++;
            }
            data.payload.emplace_back(0);
            data.length += 2;
        }
        _lobby.sendToEveryone(data);
    }


    void GameRoom::acceptClient(const Datatype &data)
    {
        if (_gameStarted || data.length == 0 || data.length > 12) {
            return;
        }
        std::string username(data.payload.begin(), data.payload.end());

        _lobby.acceptClient(username);
        sendPlayersList();
    }

    void GameRoom::removeClient(const Datatype &)
    {
        _lobby.removeClient();
        sendPlayersList();
    }

    void GameRoom::receiveStartGame(const Datatype &)
    {
        if (_gameStarted) {
            return;
        }
        _gameThread = std::thread([this] {
            Datatype data;
            data.id = 0x06;
            data.length = 1;
            data.payload.emplace_back(0);
            _lobby.sendToEveryone(data);
            startGame();
        });
    }

    void GameRoom::checkPlayerShoot(const Datatype &data)
    {
        if (data.length != 4) {
            return;
        }
        int id = _lobby.getCurrentClientId();
        std::size_t posX = (data.payload[0] << 8) | data.payload[1];
        std::size_t posY = (data.payload[2] << 8) | data.payload[3];

        if (id != -1) {
            std::lock_guard<std::mutex> lock(_mutex);
            _commandsQueue.push(Command{static_cast<std::size_t>(id), posX, posY});
        }
    }

    void GameRoom::loadRegistry()
    {
        std::shared_ptr<ECSLoader> loader = nullptr;
        const std::string libraryPath = ECS_LIBRARY_PATH;

        try {
            loader = std::make_shared<ECSLoader>(libraryPath);
            registry* raw_registry = loader->getInstance<registry>("create_registry");
            if (raw_registry == nullptr) {
                throw std::runtime_error("Failed to create registry instance");
            }
            _registry = std::shared_ptr<registry>(raw_registry);
        } catch (const std::exception &e) {
            std::cerr << "Error loading ECS library (" << libraryPath << "): " << e.what() << std::endl;
            throw std::runtime_error("Problem with ECS Loader or entry point");
        }
    }

    void GameRoom::init_components()
    {
        _registry->register_component<component::position>();
        _registry->register_component<component::player>();
        _registry->register_component<component::drawable>();
    }

    void GameRoom::initGame()
    {
        std::vector<ClientStruct> clients = _lobby.getClients();
        std::size_t i = 0;
        loadRegistry();
        init_components();

        for (const auto &client : clients) {
            Entity player = _registry->spawn_entity();
            _registry->add_component<component::player>(player, {client.id, false, 0.0f, 0.0f});
            i++;
        }

        init_spawn_targets(_registry);
    }

    void GameRoom::startGame()
    {
        try {
            _gameStarted = true;
            sf::Clock gameClock;
            initGame();
            while (*_running && _gameStarted && getNbPlayers() > 0) {
                if (gameClock.getElapsedTime().asSeconds() >= 30) {
                    Datatype data = end_system(_registry);
                    _lobby.sendToEveryone(data);
                    break;
                }
                while (!_commandsQueue.empty()) {
                    std::lock_guard<std::mutex> lock(_mutex);
                    Command cmd = _commandsQueue.front();
                    control_system(_registry, cmd);
                    _commandsQueue.pop();
                }
                _registry->run_systems();
                hit_system(_registry);
                target_system(_registry);
                if (_clock.getElapsedTime().asMilliseconds() > 1000 / 60) {
                    Datatype data = network_system(_registry);
                    _lobby.sendToEveryone(data);
                    _clock.restart();
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in startGame: " << e.what() << std::endl;
        }
    }
}
