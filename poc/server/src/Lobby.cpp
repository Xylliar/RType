/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Lobby
*/

#include "Lobby.hpp"

rType::Lobby::Lobby(asio::io_context &ioContext, std::shared_ptr<std::mutex> mutex, std::size_t id)
    : _id(id), _nbClients(0), _socket(ioContext, udp::endpoint(udp::v4(), 0)), _mutex(mutex)
{
    _port = _socket.local_endpoint().port();
}

rType::Lobby::~Lobby()
{
    _socket.close();
}

void rType::Lobby::start(std::unique_ptr<command_map_t> commands)
{
    _commands = std::move(commands);
    serverLoop();
}

std::size_t rType::Lobby::getPort() const
{
    return _port;
}

void rType::Lobby::receive(const asio::error_code &error, std::size_t bytes)
{
    if (error) {
        return;
    }
    try {
        Datatype data = Datatype::deserialize(_buffer, bytes);
        auto cmd = _commands->find(data.id);

        if (cmd != _commands->end()) {
           cmd->second(data);
        } else {
            std::cerr << "Unknown command id: " << data.id << std::endl;
        }
    } catch (const std::exception &e) {
        // std::cerr << "Error in receive: " << e.what() << std::endl;
    }
}

void rType::Lobby::send(const Datatype &data)
{
    std::shared_ptr<std::vector<uint8_t>> serializedData = std::make_shared<std::vector<uint8_t>>(data.serialize());
    {
        std::lock_guard<std::mutex> lock(_pendingMutex);
        _pendingSends.push(serializedData);
    }
    std::lock_guard<std::mutex> lock(*_mutex);
    _socket.async_send_to(
        asio::buffer(*serializedData),
        _endpoint,
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

void rType::Lobby::send(const udp::endpoint &endpoint, const Datatype &data)
{
    std::shared_ptr<std::vector<uint8_t>> serializedData = std::make_shared<std::vector<uint8_t>>(data.serialize());
    {
        std::lock_guard<std::mutex> lock(_pendingMutex);
        _pendingSends.push(serializedData);
    }
    std::lock_guard<std::mutex> lock(*_mutex);
    _socket.async_send_to(
        asio::buffer(*serializedData),
        endpoint,
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

void rType::Lobby::sendToEveryone(const Datatype &data)
{
    for (auto &client : _clients) {
        send(client.endpoint, data);
    }
}

void rType::Lobby::serverLoop()
{
    _socket.async_receive_from(asio::buffer(_buffer), _endpoint, [this](const asio::error_code &error, std::size_t bytes) {
        receive(error, bytes);
        _buffer.fill(0);
        serverLoop();
    });
}

void rType::Lobby::acceptClient(const std::string &username)
{
    if (_nbClients >= 4) {
        return;
    }
    Datatype data;
    data.id = 0x01;
    data.length = 0;

    send(data);
    _clients.emplace_back(ClientStruct{_clientId++, username, _endpoint});
    _nbClients++;
}

void rType::Lobby::removeClient()
{
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientStruct &client) {
        return client.endpoint == _endpoint;
    });

    if (it != _clients.end()) {
        it = _clients.erase(it);
        _nbClients--;
    }
}

std::vector<rType::ClientStruct> &rType::Lobby::getClients()
{
    return _clients;
}

std::size_t rType::Lobby::getNbClients() const
{
    return _nbClients;
}

int rType::Lobby::getCurrentClientId() const
{
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientStruct &client) {
        return client.endpoint == _endpoint;
    });
    if (it != _clients.end()) {
        return it->id;
    }
    return -1;
}
