/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Error
*/

#ifndef ERROR_HPP_
    #define ERROR_HPP_

    #include <iostream>
    #include <stdexcept>

namespace rType {
    class Error : public std::exception {
        public:

            /**
             * @brief Construct a new Error object.
             * @param message The error message.
             */
            Error(std::string &message) : _message(message) {}

            /**
             * @brief Get the error message.
             * @return const char* The error message.
             */
            virtual const char *what() const throw()
            {
                return _message.c_str();
            }
        private:
            std::string _message;
    };

    class ParameterError : public Error {
        public:
            /**
             * @brief Construct a new ParameterError object.
             * @param message The error message.
             */
            ParameterError(std::string message) : Error(message) {}
    };

    class ConnectionError : public Error {
        public:
            /**
             * @brief Construct a new ConnectionError object.
             * @param message The error message.
             */
            ConnectionError(std::string message) : Error(message) {}
    };

    class TextureError : public Error {
        public:
            /**
             * @brief Construct a new TextureError object.
             * @param message The error message.
             */
            TextureError(std::string message) : Error(message) {}
    };
}

#endif /* !ERROR_HPP_ */
