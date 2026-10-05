/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Client
*/

#include "UDPClient.hpp"
#include "StringUtils.hpp"

rType::UDPClient::UDPClient(const std::string &ip, std::size_t port, const std::string &username)
    : _ip(ip), _port(port), _mainPort(port), _username(username), _socket(_ioContext), _heartbeatTimer(_ioContext),
    _serverEndpoint(asio::ip::make_address(ip), port)
{
}

rType::UDPClient::~UDPClient()
{
    stop();
}

void rType::UDPClient::start(command_map_t &commands)
{
    _commands = commands;
    _socket.open(udp::v4());
    connect();
    scheduleHeartbeat();
    _ioThread = std::thread([this] {
        clientLoop();
        _ioContext.run();
    });
}

void rType::UDPClient::stop()
{
    if (_stopped.exchange(true)) {
        return;
    }
    _ioContext.stop();
    if (_ioThread.joinable())
        _ioThread.join();

    if (_socket.is_open()) {
        Datatype data;
        data.id = 0x66;
        data.length = 0;
        const std::vector<uint8_t> bytes = data.serialize();
        asio::error_code ignored;
        _socket.send_to(asio::buffer(bytes), _serverEndpoint, 0, ignored);
    }
}

void rType::UDPClient::scheduleHeartbeat()
{
    _heartbeatTimer.expires_after(std::chrono::seconds(1));
    _heartbeatTimer.async_wait([this](const asio::error_code &error) {
        if (error) {
            return;
        }
        Datatype data;
        data.id = 0x6D;
        data.length = 0;
        send(data);
        scheduleHeartbeat();
    });
}

std::string rType::UDPClient::getIp() const
{
    return _ip;
}

std::size_t rType::UDPClient::getPort() const
{
    return _port;
}

std::string rType::UDPClient::getUsername() const
{
    return _username;
}

void rType::UDPClient::receive(const asio::error_code &error, std::size_t bytes)
{
    if (error) {
        std::cerr << "Error: " << error.message() << std::endl;
        return;
    }
    try {
        Datatype data = Datatype::deserialize(_buffer, bytes);
        auto cmd = _commands.find(data.id);

        if (cmd != _commands.end()) {
           cmd->second(data);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// void rType::UDPClient::send(const Datatype &data)
// {
//     _socket.async_send_to(asio::buffer(data.serialize()), _serverEndpoint, [this](const asio::error_code &error, std::size_t) {
//         if (error) {
//             std::cerr << "Error: " << error.message() << std::endl;
//         }
//     });
// }

void rType::UDPClient::send(const Datatype &data)
{
    std::shared_ptr<std::vector<uint8_t>> serializedData = std::make_shared<std::vector<uint8_t>>(data.serialize());
    {
        std::lock_guard<std::mutex> lock(_pendingMutex);
        _pendingSends.push(serializedData);
    }
    std::lock_guard<std::mutex> lock(_mutex);
    _socket.async_send_to(
        asio::buffer(*serializedData),
        _serverEndpoint,
        [this, serializedData](const asio::error_code &error, std::size_t) {
            if (error) {
                std::cerr << "Error: " << error.message() << std::endl;
            }
            std::lock_guard<std::mutex> lock(_pendingMutex);
            if (!_pendingSends.empty()) {
                _pendingSends.pop();
            }
        }
    );
}

void rType::UDPClient::clientLoop()
{
    _socket.async_receive_from(asio::buffer(_buffer), _senderEndpoint, [this](const asio::error_code &error, std::size_t bytes) {
        if (error == asio::error::operation_aborted) {
            return;
        }
        receive(error, bytes);
        _buffer.fill(0);
        clientLoop();
    });
}

void rType::UDPClient::connect()
{
    Datatype data;
    data.id = 0x65;
    data.length = _username.size();
    for (const auto &c : _username) {
        data.payload.emplace_back(static_cast<uint8_t>(c));
    }

    send(data);
}

void rType::UDPClient::updatePort(std::size_t port)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (port == 0) {
        _port = _mainPort;
        _serverEndpoint.port(_mainPort);
    } else {
        _port = port;
        _serverEndpoint.port(port);
    }
}

void rType::UDPClient::startGame(uint8_t selectedLvl)
{
    Datatype data;
    data.id = 0x6B;
    data.length = 1;
    data.payload.emplace_back(selectedLvl);
    send(data);
}

void rType::UDPClient::createRoom()
{
    Datatype data;
    data.id = 0x67;
    data.length = 0;

    send(data);
}

void rType::UDPClient::joinRoom(std::size_t id)
{
    Datatype data;
    data.id = 0x69;
    data.length = 1;
    data.payload.emplace_back(static_cast<uint8_t>(id));

    send(data);
}


void rType::UDPClient::leaveRoom()
{
    Datatype data;
    data.id = 0x6A;
    data.length = 0;

    send(data);
    std::lock_guard<std::mutex> lock(_mutex);
    _serverEndpoint.port(_mainPort);
}

void rType::UDPClient::cancelGame()
{
    Datatype data;
    data.id = 0x6C;
    data.length = 0;

    send(data);
}
