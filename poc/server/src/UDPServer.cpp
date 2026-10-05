/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** UDPServer
*/

#include "UDPServer.hpp"

rType::UDPServer::UDPServer(std::size_t port)
    : _ioContext(), _socket(_ioContext, udp::endpoint(udp::v4(), port))
{
    _mutex = std::make_shared<std::mutex>();
}

rType::UDPServer::~UDPServer()
{
    _socket.close();
    _ioContext.stop();
    if (_ioThread.joinable())
        _ioThread.join();
}

void rType::UDPServer::start(std::unique_ptr<command_map_t> commands)
{
    _commands = std::move(commands);
    _ioThread = std::thread([this] {
        serverLoop();
        _ioContext.run();
    });
}

asio::io_context &rType::UDPServer::getIoContext()
{
    return _ioContext;
}

std::size_t rType::UDPServer::getNbClients() const
{
    return _nbClients;
}

void rType::UDPServer::receive(const asio::error_code &error, std::size_t bytes)
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

void rType::UDPServer::send(const Datatype &data)
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

void rType::UDPServer::send(const udp::endpoint &endpoint, const Datatype &data)
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

void rType::UDPServer::sendToEveryone(const Datatype &data)
{
    for (auto &client : _clients) {
        send(client.endpoint, data);
    }
}

void rType::UDPServer::serverLoop()
{
    _socket.async_receive_from(asio::buffer(_buffer), _endpoint, [this](const asio::error_code &error, std::size_t bytes) {
        receive(error, bytes);
        _buffer.fill(0);
        serverLoop();
    });
}


void rType::UDPServer::acceptClient(const std::string &username)
{
    Datatype data;
    data.id = 0x01;
    data.length = 0;

    send(data);
    std::cout << "New client connected: " << username << " on " << _endpoint.address().to_string() << ":" << _endpoint.port() << std::endl;
    _clients.emplace_back(ClientData{username, _endpoint});
    _nbClients++;
}

void rType::UDPServer::removeClient()
{
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientData &client) {
        return client.endpoint == _endpoint;
    });

    if (it != _clients.end()) {
        it = _clients.erase(it);
        _nbClients--;
    }
}

std::vector<rType::ClientData> &rType::UDPServer::getClients()
{
    return _clients;
}

rType::ClientData &rType::UDPServer::getCurrentClient()
{
    auto it = std::find_if(_clients.begin(), _clients.end(), [this](const ClientData &client) {
        return client.endpoint == _endpoint;
    });

    if (it == _clients.end()) {
        std::cerr << "Client note found" << std::endl;
        return _clients[0];
    }
    return *it;
}

std::shared_ptr<std::mutex> &rType::UDPServer::getMutex()
{
    return _mutex;
}
