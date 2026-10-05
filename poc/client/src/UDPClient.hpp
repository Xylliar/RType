/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** UDPClient
*/

#ifndef UDPCLIENT_HPP_
    #define UDPCLIENT_HPP_

    #include <iostream>
    #include <asio.hpp>
    #include <thread>
    #include <map>
    #include <queue>
    #include <memory>

    #include "Network.hpp"

    using asio::ip::udp;

typedef std::function<void(const Datatype &)> command_handler_t;
typedef std::map<uint16_t, command_handler_t> command_map_t;

namespace rType {
    class UDPClient {
        public:
            UDPClient(const std::string &ip, std::size_t port, const std::string &username);
            ~UDPClient();
            void start(command_map_t &commands);
            std::string getIp() const;
            std::size_t getPort() const;
            std::string getUsername() const;
            void clientLoop();
            void receive(const asio::error_code &error, std::size_t bytes);
            void send(const Datatype &data);

            void connect();
            void updatePort(std::size_t port);
            void createRoom();
            void joinRoom(std::size_t id);
            void leaveRoom();
            void startGame();
            void cancelGame();

        private:
            std::string _ip;
            std::size_t _port;
            std::size_t _mainPort;
            std::string _username;
            asio::io_context _ioContext;
            std::thread _ioThread;
            udp::socket _socket;
            udp::endpoint _serverEndpoint;
            std::array<uint8_t, BUFFSIZE> _buffer;
            command_map_t _commands;
            std::queue<std::shared_ptr<std::vector<uint8_t>>> _pendingSends;
            std::mutex _mutex;
            std::mutex _pendingMutex;
    };
}

#endif /* !UDPCLIENT_HPP_ */
