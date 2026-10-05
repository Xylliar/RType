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
    #include <chrono>
    #include <map>
    #include <memory>
    #include <mutex>
    #include <optional>
    #include <queue>
    #include "Network.hpp"

    using asio::ip::udp;

namespace rType {
    struct ClientStruct {
        std::size_t id;
        std::string username;
        udp::endpoint endpoint;
        std::chrono::steady_clock::time_point lastSeen;
    };

    class Lobby {
        public:
            Lobby(asio::io_context &ioContext, std::shared_ptr<std::mutex> mutex, std::size_t id);
            ~Lobby();
            std::size_t getPort() const;
            std::vector<ClientStruct> getClients() const;
            std::size_t getNbClients() const;
            int getCurrentClientId() const;
            void start(std::unique_ptr<command_map_t> commands);
            void receive(const asio::error_code &error, std::size_t bytes);
            void send(const Datatype &data);
            void send(const udp::endpoint &endpoint, const Datatype &data);
            void sendToEveryone(const Datatype &data);
            void serverLoop();
            void acceptClient(const std::string &username);
            std::optional<ClientStruct> removeClient();
            std::vector<ClientStruct> removeInactiveClients(std::chrono::steady_clock::duration timeout);

        private:
            std::size_t _id = 0;
            std::size_t _clientId = 1;
            std::size_t _port = 0;
            udp::socket _socket;
            std::shared_ptr<std::mutex> _mutex;
            udp::endpoint _endpoint;
            std::array<uint8_t, BUFFSIZE> _buffer;
            std::unique_ptr<command_map_t> _commands;
            std::vector<ClientStruct> _clients;
            mutable std::mutex _clientsMutex;
    };
}

#endif /* !LOBBY_HPP_ */
