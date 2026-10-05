/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Lobby
*/

#include "Lobby.hpp"

rType::Lobby::Lobby(asio::io_context &ioContext, std::shared_ptr<std::mutex> mutex, std::size_t id)
    : _id(id), _socket(ioContext, udp::endpoint(udp::v4(), 0)), _mutex(mutex)
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
        {
            std::lock_guard<std::mutex> lock(_clientsMutex);
            for (auto &client : _clients) {
                if (client.endpoint == _endpoint) {
                    client.lastSeen = std::chrono::steady_clock::now();
                }
            }
        }
        auto cmd = _commands->find(data.id);

        if (cmd != _commands->end()) {
           cmd->second(data);
        }
    } catch (const std::exception &e) {
        // std::cerr << "Error in receive: " << e.what() << std::endl;
    }
}

void rType::Lobby::send(const Datatype &data)
{
    send(_endpoint, data);
}

void rType::Lobby::send(const udp::endpoint &endpoint, const Datatype &data)
{
    auto serializedData = std::make_shared<std::vector<uint8_t>>(data.serialize());
    std::lock_guard<std::mutex> lock(*_mutex);
    _socket.async_send_to(
        asio::buffer(*serializedData),
        endpoint,
        [serializedData](const asio::error_code &error, std::size_t) {
            if (error && error != asio::error::operation_aborted) {
                std::cerr << "Error: " << error.message() << std::endl;
            }
        }
    );
}

void rType::Lobby::sendToEveryone(const Datatype &data)
{
    std::vector<udp::endpoint> endpoints;
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        for (const auto &client : _clients) {
            endpoints.push_back(client.endpoint);
        }
    }
    for (const auto &endpoint : endpoints) {
        send(endpoint, data);
    }
}

void rType::Lobby::serverLoop()
{
    _socket.async_receive_from(asio::buffer(_buffer), _endpoint, [this](const asio::error_code &error, std::size_t bytes) {
        if (error == asio::error::operation_aborted) {
            return;
        }
        receive(error, bytes);
        _buffer.fill(0);
        serverLoop();
    });
}

void rType::Lobby::acceptClient(const std::string &username)
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientStruct &client) {
        return client.endpoint == _endpoint;
    });
    if (it == _clients.end()) {
        if (_clients.size() >= 4) {
            return;
        }
        _clients.emplace_back(ClientStruct{_clientId++, username, _endpoint, std::chrono::steady_clock::now()});
    }

    Datatype data;
    data.id = 0x01;
    data.length = 0;
    send(data);
}

std::optional<rType::ClientStruct> rType::Lobby::removeClient()
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientStruct &client) {
        return client.endpoint == _endpoint;
    });

    if (it == _clients.end()) {
        return std::nullopt;
    }
    ClientStruct removed = *it;
    _clients.erase(it);
    return removed;
}

std::vector<rType::ClientStruct> rType::Lobby::removeInactiveClients(std::chrono::steady_clock::duration timeout)
{
    const auto deadline = std::chrono::steady_clock::now() - timeout;
    std::vector<ClientStruct> removed;

    std::lock_guard<std::mutex> lock(_clientsMutex);
    auto it = std::remove_if(_clients.begin(), _clients.end(), [&](const ClientStruct &client) {
        if (client.lastSeen < deadline) {
            removed.push_back(client);
            return true;
        }
        return false;
    });
    _clients.erase(it, _clients.end());
    return removed;
}

std::vector<rType::ClientStruct> rType::Lobby::getClients() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    return _clients;
}

std::size_t rType::Lobby::getNbClients() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    return _clients.size();
}

int rType::Lobby::getCurrentClientId() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientStruct &client) {
        return client.endpoint == _endpoint;
    });
    if (it != _clients.end()) {
        return static_cast<int>(it->id);
    }
    return -1;
}
