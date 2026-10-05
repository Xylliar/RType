/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Arguments
*/

#ifndef CLIARGUMENTS_HPP_
    #define CLIARGUMENTS_HPP_

    #include "Arguments.hpp"

namespace rType {
    class CliArguments : public rType::Arguments {
        public:
            /*
            * @brief Constructor of Arguments class.
            * @param ac number of arguments
            * @param av content of arguments
            */
            CliArguments(int ac, const char *av[]);

            /*
            * @brief Destructor of Arguments class.
            */
            ~CliArguments() = default;

            /*
            * @brief Get the server port.
            * @return Port of the server.
            */
            std::size_t getPort() const { return _port; };

            /*
            * @brief Get the server ip.
            * @return Ip of the server.
            */
            std::string getIp() const { return _ip; };

            std::string getUsername() const { return _username; };

        private:
            /*
            * @brief Retrieve the arguments passed to the program.
            */
            void retrieveArguments() override;

            std::optional<std::size_t> _op_port = std::nullopt;
            std::size_t _port;
            std::string _ip = "127.0.0.1";
            std::string _username = "AimBot";
    };
};

#endif /* !CLIARGUMENTS_HPP_ */
