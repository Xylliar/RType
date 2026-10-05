/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Lobby
*/

#ifndef LOBBY_HPP_
    #define LOBBY_HPP_

    #include <iostream>
    #include <asio.hpp>
    #include <algorithm>
    #include <map>
    #include <memory>
    #include <mutex>
    #include <queue>
    #include "Network.hpp"

    using asio::ip::udp;

namespace rType {
    struct ClientStruct {
        std::size_t id;
        std::string username;
        udp::endpoint endpoint;
    };

    class Lobby {
        public:
            Lobby(asio::io_context &ioContext, std::shared_ptr<std::mutex> mutex, std::size_t id);
            ~Lobby();
            std::size_t getPort() const;
            std::vector<ClientStruct> &getClients();
            std::size_t getNbClients() const;
            int getCurrentClientId() const;
            void start(std::unique_ptr<command_map_t> commands);
            void receive(const asio::error_code &error, std::size_t bytes);
            void send(const Datatype &data);
            void send(const udp::endpoint &endpoint, const Datatype &data);
            void sendToEveryone(const Datatype &data);
            void serverLoop();
            void acceptClient(const std::string &username);
            void removeClient();

        private:
            std::size_t _id = 0;
            std::size_t _clientId = 0;
            std::size_t _nbClients = 0;
            std::size_t _port = 0;
            udp::socket _socket;
            std::shared_ptr<std::mutex> _mutex;
            udp::endpoint _endpoint;
            std::array<uint8_t, BUFFSIZE> _buffer;
            std::unique_ptr<command_map_t> _commands;
            std::vector<ClientStruct> _clients;
            std::queue<std::shared_ptr<std::vector<uint8_t>>> _pendingSends;
            std::mutex _pendingMutex;
    };
}

#endif /* !LOBBY_HPP_ */
