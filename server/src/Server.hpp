/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Server
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

    #include "UDPServer.hpp"
    #include "GameRoom.hpp"
    #include <vector>
    #include <memory>

typedef std::function<void(const Datatype &)> command_handler_t;
typedef std::map<uint16_t, command_handler_t> command_map_t;

namespace rType {
    class Server {
        public:
            Server(std::size_t port);
            ~Server();

            void run();

            void addNewClient(const Datatype &);
            void disconnectClient(const Datatype &);
            void createRoom(const Datatype &);
            void listOfRoom(const Datatype &);
            void joinRoom(const Datatype &);

        private:
            static constexpr std::size_t MAX_ROOMS = 32;

            UDPServer _server;
            std::vector<std::unique_ptr<GameRoom>> _rooms{};
            std::shared_ptr<std::atomic<bool>> _running;
    };
};

#endif /* !SERVER_HPP_ */