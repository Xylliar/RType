/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Main
*/

#include "ServArguments.hpp"
#include "Server.hpp"

int main(int ac, const char *av[])
{
    try {
        rType::ServArguments args(ac, av);
        rType::Server server(args.getPort());

        std::cout << "Aim-box server listening on UDP port " << args.getPort()
            << " (Ctrl+C to stop)" << std::endl;
        server.run();
    } catch (const rType::Error &error) {
        std::cout << error.what() << std::endl;
        return 84;
    } catch (const std::exception &error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 84;
    }
    return 0;
}
