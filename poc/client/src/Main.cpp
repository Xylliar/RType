/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Main
*/

#include "CliArguments.hpp"
#include "Client.hpp"

int main(int ac, const char *av[])
{
    try {
        rType::CliArguments args(ac, av);
        rType::Client client(args.getIp(), args.getPort(), args.getUsername());

        client.displayLoop();
    } catch (const std::exception &error) {
        std::cout << error.what() << std::endl;
        return 84;
    }
    return 0;
}
