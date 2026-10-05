/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** UDPServer
*/

#ifndef UDPSERVER_HPP_
    #define UDPSERVER_HPP_

    #include <iostream>
    #include <asio.hpp>
    #include <atomic>
    #include <thread>
    #include <mutex>
    #include <algorithm>
    #include <map>
    #include <memory>
    #include <queue>
    #include "Network.hpp"

    using asio::ip::udp;

namespace rType {
    struct ClientData {
        std::string username;
        udp::endpoint endpoint;
    };

    class UDPServer {
        public:
            UDPServer(std::size_t port);
            ~UDPServer();
            void start(std::unique_ptr<command_map_t> commands);
            void stop();
            asio::io_context &getIoContext();
            std::shared_ptr<std::mutex> &getMutex();
            std::vector<ClientData> &getClients();
            ClientData &getCurrentClient();
            std::size_t getNbClients() const;
            void receive(const asio::error_code &error, std::size_t bytes);
            void send(const Datatype &data);
            void send(const udp::endpoint &endpoint, const Datatype &data);
            void sendToEveryone(const Datatype &data);
            void serverLoop();
            void acceptClient(const std::string &username);
            void removeClient();

        private:
            asio::io_context _ioContext;
            udp::socket _socket;
            std::thread _ioThread;
            std::shared_ptr<std::mutex> _mutex;
            udp::endpoint _endpoint;
            std::array<uint8_t, BUFFSIZE> _buffer;
            std::unique_ptr<command_map_t> _commands;
            std::size_t _nbClients = 0;
            std::vector<ClientData> _clients;
            std::queue<std::shared_ptr<std::vector<uint8_t>>> _pendingSends;
            std::mutex _pendingMutex;
    };
}

#endif /* !UDPSERVER_HPP_ */
