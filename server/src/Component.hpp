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

    struct velocity {
        float value = 0;
        float vx = 0;
        float vy = 0;
    };

    struct drawable {
        std::string path;
        std::string id;
    };

    struct clock {
        float refresh;
        sf::Clock clock;
    };

    struct controllable {
        bool isControllable = false;
    };

    struct acceleration {
        float value = 0;
    };

    struct deceleration {
        float value = 0;
    };

    struct bullet {
        float x = 0;
        float velocity = 0;
        int damage = 10;
    };

    struct shootable {
        bool isShootable = false;
    };

    struct cooldown {
        float duration;
        sf::Clock clock;
    };

    struct enemy {
        bool isUp = false;
        int health = 100;
        bool display = false;
        bool shouldShoot = false;
        std::size_t id;
    };

    struct player {
        std::size_t id;
        int health = 100;
        bool shouldShoot = false;
        float regenerationTime = 0.0f;
        static constexpr float maxRegeneration = 2.0f;
        static constexpr float baselineDamage = 5.0f;
        static constexpr float maxDamage = 20.0f;
        bool hasPowerUpDamage = false;
        bool hasPowerUpSpeed = false;
    };

    struct power_up_damage {
        std::string type;
        bool isAttach = false;
        bool display = false;
    };

    struct power_up_speed {
        std::string type;
        bool isTouch = false;
        bool display = false;
    };

    struct power_up_health {
        std::string type;
        bool isTouch = false;
        bool display = false;
    };
};
#endif /* !COMPONENTS_HPP_ */
