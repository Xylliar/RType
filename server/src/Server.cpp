/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#include "Server.hpp"
#include "StringUtils.hpp"
#include <chrono>
#include <csignal>
#include <thread>
#ifdef _WIN32
    #include <cstdio>
    #include <io.h>
#else
    #include <unistd.h>
#endif

namespace {
    std::atomic<bool> stopRequested{false};

    void requestStop(int)
    {
        stopRequested = true;
    }

    bool hasInteractiveConsole()
    {
#ifdef _WIN32
        return _isatty(_fileno(stdin));
#else
        return isatty(STDIN_FILENO) && tcgetpgrp(STDIN_FILENO) == getpgrp();
#endif
    }
}

rType::Server::Server(std::size_t port) : _server(port)
{
    _running = std::make_shared<std::atomic<bool>>(true);
    command_map_t commands = {
        {0x65, std::bind(&Server::addNewClient, this, std::placeholders::_1)},
        {0x66, std::bind(&Server::disconnectClient, this, std::placeholders::_1)},
        {0x67, std::bind(&Server::createRoom, this, std::placeholders::_1)},
        {0x68, std::bind(&Server::listOfRoom, this, std::placeholders::_1)},
        {0x69, std::bind(&Server::joinRoom, this, std::placeholders::_1)},
    };
    _server.start(std::make_unique<command_map_t>(commands));
}

rType::Server::~Server()
{
    *_running = false;
    _server.stop();

    Datatype data;
    data.id = 0x02;
    data.length = 0;

    _server.sendToEveryone(data);
    for (auto &room : _rooms) {
        room->notifyShutdown();
    }
}

void rType::Server::run()
{
    std::signal(SIGINT, requestStop);
    std::signal(SIGTERM, requestStop);

    if (hasInteractiveConsole()) {
        std::thread([] {
            std::string input;
            while (std::getline(std::cin, input)) {
                if (utils::StringUtils::toLower(input) == "exit") {
                    break;
                }
            }
            stopRequested = true;
        }).detach();
    }
    while (!stopRequested) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    *_running = false;
}

void rType::Server::addNewClient(const Datatype &data)
{
    std::string username(data.payload.begin(), data.payload.end());
    if (!utils::StringUtils::isValidUsername(username)) {
        return;
    }

    _server.acceptClient(username);
}

void rType::Server::disconnectClient(const Datatype &)
{
    _server.removeClient();
}

void rType::Server::createRoom(const Datatype &)
{
    _rooms.erase(std::remove_if(_rooms.begin(), _rooms.end(), [](const std::unique_ptr<GameRoom> &room) {
        return room->isReclaimable();
    }), _rooms.end());
    if (_rooms.size() >= MAX_ROOMS) {
        listOfRoom({});
        return;
    }
    std::size_t roomId = 0;
    while (std::any_of(_rooms.begin(), _rooms.end(), [roomId](const std::unique_ptr<GameRoom> &room) {
        return room->getRoomId() == roomId;
    })) {
        roomId++;
    }
    try {
        _rooms.emplace_back(std::make_unique<GameRoom>(roomId, _server.getIoContext(), _server.getMutex(), _running));
        std::size_t port = _rooms.back()->getPort();
        Datatype data;
        data.id = 0x08;
        data.length = 2;
        data.payload.emplace_back((port >> 8) & 0xFF);
        data.payload.emplace_back(port & 0xFF);
        _server.send(data);
        _server.removeClient();
    } catch (const std::exception& e) {
        std::cerr << "Error creating room: " << e.what() << std::endl;
    }
}

void rType::Server::listOfRoom(const Datatype &)
{
    Datatype data;
    data.id = 0x03;
    data.length = 0;
    for (const auto& room : _rooms) {
        if (room->getNbPlayers() < 4 && !room->getGameStatus()) {
            data.payload.emplace_back(room->getRoomId());
            data.length += 1;
        }
    }
    _server.send(data);
}

void rType::Server::joinRoom(const Datatype &data)
{
    if (data.payload.empty()) {
        listOfRoom({});
        return;
    }
    try {
        std::size_t roomId = data.payload[0];
        auto it = std::find_if(_rooms.begin(), _rooms.end(), [roomId](const std::unique_ptr<GameRoom> &room) {
            return room->getRoomId() == roomId;
        });
        if (it == _rooms.end() || (*it)->getGameStatus() || (*it)->getNbPlayers() >= 4) {
            listOfRoom({});
            return;
        }
        std::size_t port = (*it)->getPort();
        Datatype data;
        data.id = 0x08;
        data.length = 2;
        data.payload.emplace_back((port >> 8) & 0xFF);
        data.payload.emplace_back(port & 0xFF);
        _server.send(data);
        _server.removeClient();
    } catch (const std::exception& e) {
        std::cerr << "Error joining room: " << e.what() << std::endl;
    }
}