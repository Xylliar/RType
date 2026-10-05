/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Arguments
*/

#ifndef SERVARGUMENTS_HPP_
    #define SERVARGUMENTS_HPP_

    #include "Arguments.hpp"

namespace rType {
    class ServArguments : public rType::Arguments {
        public:
            /*
            * @brief Constructor of Arguments class.
            * @param ac number of arguments
            * @param av content of arguments
            */
            ServArguments(int ac, const char *av[]);

            /*
            * @brief Destructor of Arguments class.
            */
            ~ServArguments() = default;

            /*
            * @brief Get the server port.
            * @return Port of the server.
            */
            std::size_t getPort() const { return _port; };

        private:
            /*
            * @brief Retrieve the arguments passed to the program.
            */
            void retrieveArguments() override;

            std::optional<std::size_t> _op_port = std::nullopt;
            std::size_t _port;
    };
};

#endif /* !SERVARGUMENTS_HPP_ */
