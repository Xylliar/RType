/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** GameRoom
*/

#include "GameRoom.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include "ECSLoader.hpp"
#include "Config.hpp"
#include "StringUtils.hpp"

namespace {
    constexpr auto CLIENT_TIMEOUT = std::chrono::seconds(5);
    constexpr auto TICK = std::chrono::microseconds(1000000 / 60);
}

namespace rType {
    GameRoom::GameRoom(std::size_t roomId, asio::io_context &context, std::shared_ptr<std::mutex> mutex, std::shared_ptr<std::atomic<bool>> running)
    : _lobby(context, mutex, roomId), _scene("./config/scenes.config"), _roomId(roomId), _running(running), _livenessTimer(context)
    {
        command_map_t cmd = {
            {0x65, std::bind(&GameRoom::acceptClient, this, std::placeholders::_1)},
            {0x66, std::bind(&GameRoom::removeClient, this, std::placeholders::_1)},
            {0x6A, std::bind(&GameRoom::removeClient, this, std::placeholders::_1)},
            {0x6B, std::bind(&GameRoom::receiveStartGame, this, std::placeholders::_1)},
            {0x79, std::bind(&GameRoom::checkMovePlayer, this, std::placeholders::_1)},
            {0x7A, std::bind(&GameRoom::checkPlayerShoot, this, std::placeholders::_1)},
        };
        _lobby.start(std::make_unique<command_map_t>(cmd));
        scheduleLivenessCheck();
    }

    GameRoom::~GameRoom()
    {
        _livenessTimer.cancel();
        if (_gameThread.joinable()) {
            _gameThread.join();
        }
    }

    void GameRoom::notifyShutdown()
    {
        Datatype data;
        data.id = 0x02;
        data.length = 0;

        _lobby.sendToEveryone(data);
    }

    bool GameRoom::isReclaimable() const
    {
        if (_gameOver) {
            return true;
        }
        return !_gameStarted && getNbPlayers() == 0
            && std::chrono::steady_clock::now() - _createdAt > std::chrono::seconds(30);
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
            data.payload.emplace_back(static_cast<uint8_t>(client.id));
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
        std::string username(data.payload.begin(), data.payload.end());
        if (_gameStarted || !utils::StringUtils::isValidUsername(username)) {
            return;
        }

        _lobby.acceptClient(username);
        sendPlayersList();
    }

    void GameRoom::removeClient(const Datatype &)
    {
        if (auto removed = _lobby.removeClient()) {
            handleDeparture(*removed, false);
        }
    }

    void GameRoom::scheduleLivenessCheck()
    {
        _livenessTimer.expires_after(std::chrono::seconds(1));
        _livenessTimer.async_wait([this](const asio::error_code &error) {
            if (error) {
                return;
            }
            for (const auto &client : _lobby.removeInactiveClients(CLIENT_TIMEOUT)) {
                handleDeparture(client, true);
            }
            scheduleLivenessCheck();
        });
    }

    void GameRoom::handleDeparture(const ClientStruct &client, bool connectionLost)
    {
        if (_gameOver) {
            return;
        }
        std::cout << "Room " << _roomId << ": " << client.username
            << (connectionLost ? " lost connection" : " left") << std::endl;
        if (!_gameStarted) {
            sendPlayersList();
            return;
        }
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _commandsQueue.push(Command{client.id, 2, 0});
        }
        Datatype data;
        data.id = 0x52;
        data.length = 2;
        data.payload.emplace_back(static_cast<uint8_t>(client.id));
        data.payload.emplace_back(connectionLost ? 1 : 0);
        _lobby.sendToEveryone(data);
    }

    void GameRoom::receiveStartGame(const Datatype &data)
    {
        if (data.payload.empty()) {
            return;
        }
        const std::size_t level = data.payload[0];
        if (level == 0 || level > _scene.getScenes().size() || _lobby.getCurrentClientId() == -1) {
            return;
        }
        if (_gameStarted.exchange(true)) {
            return;
        }
        _nbScene = static_cast<int>(level);
        _gameThread = std::thread([this] {
            Datatype data;
            data.id = 0x06;
            data.length = 1;
            data.payload.emplace_back(_nbScene);
            _lobby.sendToEveryone(data);
            startGame();
        });
    }

    void GameRoom::checkMovePlayer(const Datatype &data)
    {
        if (!_gameStarted || data.payload.empty()) {
            return;
        }
        int id = _lobby.getCurrentClientId();

        if (id != -1) {
            std::lock_guard<std::mutex> lock(_mutex);
            _commandsQueue.push(Command{static_cast<std::size_t>(id), 0, data.payload[0]});
        }
    }

    void GameRoom::checkPlayerShoot(const Datatype &)
    {
        if (!_gameStarted) {
            return;
        }
        int id = _lobby.getCurrentClientId();

        if (id != -1) {
            std::lock_guard<std::mutex> lock(_mutex);
            _commandsQueue.push(Command{static_cast<std::size_t>(id), 1, 0});
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

    void GameRoom::spawnEnemy(std::size_t nbPlayers)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> disX(WINDOW_WIDTH - 200, WINDOW_WIDTH - 100);
        static std::uniform_real_distribution<> disY(10, WINDOW_HEIGHT - 100);

        auto& scene = _scene.getScenes()[_nbScene - 1];
        if (scene.at(1) >= 1 && (_clockEnemy.at(0).getElapsedTime().asSeconds() >= scene.at(2))) {
            spawn_enemy(_registry, disX(gen), disY(gen));
            scene.at(1) -= 1;
            _clockEnemy.at(0).restart();
        }
        if (scene.at(3) >= 1 && (_clockEnemy.at(1).getElapsedTime().asSeconds() >= scene.at(4))) {
            spawn_lateral_enemy(_registry, disX(gen), disY(gen));
            scene.at(3) -= 1;
            _clockEnemy.at(1).restart();
        }
        if (scene.at(5) >= 1 && (_clockEnemy.at(2).getElapsedTime().asSeconds() >= scene.at(6))) {
            spawn_wave_enemy(_registry, disX(gen), disY(gen));
            _clockEnemy.at(2).restart();
            scene.at(5) -= 1;
        }
        if (scene.at(3) == 0 && scene.at(1) == 0 && !_timerBoss) {
            if (scene.at(7) == 1)
                spawn_boss(_registry, WINDOW_WIDTH - BOSS_2_WIDTH, 100, nbPlayers);
            else if (scene.at(7) == 2)
                spawn_boss_two(_registry, WINDOW_WIDTH - BOSS_1_WIDTH, 100, nbPlayers);
            sf::Clock clock;
            _clockEnemy.push_back(clock);
            _timerBoss = true;
        }
        if (_timerBoss && _clockEnemy.at(3).getElapsedTime().asSeconds() >= 10 && !_bossSpawned)
            _bossSpawned = true;
    }

    void GameRoom::initComponents()
    {
        _registry->register_component<component::position>();
        _registry->register_component<component::velocity>();
        _registry->register_component<component::controllable>();
        _registry->register_component<component::acceleration>();
        _registry->register_component<component::drawable>();
        _registry->register_component<component::deceleration>();
        _registry->register_component<component::shootable>();
        _registry->register_component<component::bullet>();
        _registry->register_component<component::clock>();
        _registry->register_component<component::cooldown>();
        _registry->register_component<component::enemy>();
        _registry->register_component<component::player>();
        _registry->register_component<component::power_up_damage>();
        _registry->register_component<component::power_up_speed>();
        _registry->register_component<component::power_up_health>();
    }

    void GameRoom::initSystems()
    {
        _registry->add_system<component::position, component::velocity, component::drawable, component::controllable>(player_system);
        _registry->add_system<component::bullet, component::position, component::drawable, component::velocity>(bullet_system);
        _registry->add_system<component::position, component::velocity, component::drawable>(enemy_lateral_system);
        _registry->add_system<component::position, component::velocity, component::drawable>(enemy_wave_system);
        _registry->add_system<component::position, component::velocity, component::drawable, component::power_up_damage, component::player>(power_up_damage_system);
        _registry->add_system<component::power_up_damage>(spawn_randomly_power_up_damage);
        _registry->add_system<component::power_up_speed>(spawn_randomly_power_up_speed);
        _registry->add_system<component::power_up_health>(spawn_randomly_power_up_health);
    }

    void GameRoom::initGame()
    {
        if (_scene.getScenes().empty() == true) {
            throw std::runtime_error("Failed to create load scene");
        }
        std::vector<ClientStruct> clients = _lobby.getClients();
        std::size_t i = 0;
        loadRegistry();
        initComponents();

        for (std::size_t i = 0; i < 3; ++i) {
            sf::Clock clock;
            _clockEnemy.push_back(clock);
        }
        for (const auto &client : clients) {
            Entity player = _registry->spawn_entity();
            _registry->add_component<component::position>(player, {50.0f, (i + 1) * 150.0f});
            _registry->add_component<component::drawable>(player, {"src/r-typesheet24.gif", "Player"});
            _registry->add_component<component::shootable>(player, {true});
            _registry->add_component<component::clock>(player, {0.2f, sf::Clock()});
            _registry->add_component<component::controllable>(player, {true});
            _registry->add_component<component::cooldown>(player, {0.5f, sf::Clock()});
            _registry->add_component<component::velocity>(player, {7.0f, 0, 0});
            _registry->add_component<component::player>(player, {client.id, 100, false, 0.0f});
            i++;
        }
        initSystems();
    }

    void GameRoom::processCommands()
    {
        std::queue<Command> commands;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            std::swap(commands, _commandsQueue);
        }
        while (!commands.empty()) {
            control_system(_registry, commands.front());
            commands.pop();
        }
    }

    bool GameRoom::checkEndGame()
    {
        if (_bossSpawned && win_system(_registry)) {
            Datatype data;
            data.id = 0x51;
            data.length = 1;
            data.payload.emplace_back(1);
            _lobby.sendToEveryone(data);
            return true;
        }
        if (lose_system(_registry)) {
            Datatype data;
            data.id = 0x51;
            data.length = 1;
            data.payload.emplace_back(0);
            _lobby.sendToEveryone(data);
            return true;
        }
        return false;
    }

    void GameRoom::runAnimationSystems()
    {
        collision_player_system(_registry, [this](const Datatype& data) { _lobby.sendToEveryone(data); });
        collision_enemy_system(_registry, [this](const Datatype& data) { _lobby.sendToEveryone(data); });
        shoot_system(_registry, [this](const Datatype& data) { _lobby.sendToEveryone(data); });
        enemy_system(_registry, [this](const Datatype& data) { _lobby.sendToEveryone(data); });
        boss_system(_registry, [this](const Datatype& data) { _lobby.sendToEveryone(data); });
        power_up_speed_system(_registry, [this](const Datatype& data) { _lobby.sendToEveryone(data); });
        power_up_health_system(_registry, [this](const Datatype& data) { _lobby.sendToEveryone(data); });
    }

    void GameRoom::startGame()
    {
        using clock = std::chrono::steady_clock;

        try {
            _gameStarted = true;
            initGame();
            std::size_t nbPlayers = _lobby.getNbClients();
            auto nextTick = clock::now();

            while (*_running && _gameStarted && getNbPlayers() > 0) {
                processCommands();
                if (checkEndGame()) {
                    break;
                }
                spawnEnemy(nbPlayers);
                runAnimationSystems();
                _registry->run_systems();
                network_system(_registry, [this](const Datatype& data) { _lobby.sendToEveryone(data); });

                nextTick += TICK;
                const auto now = clock::now();
                if (nextTick < now) {
                    nextTick = now;
                }
                std::this_thread::sleep_until(nextTick);
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in startGame: " << e.what() << std::endl;
            Datatype data;
            data.id = 0x51;
            data.length = 1;
            data.payload.emplace_back(0);
            _lobby.sendToEveryone(data);
        }
        _gameOver = true;
    }
}
