/*
** EPITECH PROJECT, 2024
** rtape
** File description:
** Component
*/

#ifndef COMPONENT_HPP_
    #define COMPONENT_HPP_

    #include <SFML/Graphics.hpp>

namespace component {
    struct position {
        float x;
        float y;
    };

    struct drawable {
        std::string id;
    };

    struct player {
        std::size_t id;
        bool shouldShoot = false;
        float shootX = 0.0f;
        float shootY = 0.0f;
        int score = 0;
    };
};
#endif /* !COMPONENTS_HPP_ */