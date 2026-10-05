/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** System
*/

#ifndef SYSTEM_HPP_
    #define SYSTEM_HPP_

    #include "../../ecs/include/Registry.hpp"
    #include "Component.hpp"
    #include "Network.hpp"

struct Command {
    std::size_t playerId;
    std::size_t type;
    std::size_t param;
};

void spawn_enemy(std::shared_ptr<registry> r, float x, float y);

void spawn_lateral_enemy(std::shared_ptr<registry> r, float x, float y);

void spawn_wave_enemy(std::shared_ptr<registry> r, float x, float y);
void spawn_power_up_damage(std::shared_ptr<registry> r, float x, float y);
void spawn_power_up_speed(std::shared_ptr<registry> r, float x, float y);

void enemy_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone);

void enemy_lateral_system(std::shared_ptr<registry> r, SparseArray<component::position> &positions, SparseArray<component::velocity> &velocities,
    SparseArray<component::drawable> &drawables);

void enemy_wave_system(std::shared_ptr<registry> r, SparseArray<component::position> &positions, SparseArray<component::velocity> &velocities,
    SparseArray<component::drawable> &drawables);

void shoot_system(std::shared_ptr<registry> r, std::function<void(const Datatype &)> sendToEveryone);

void player_system(std::shared_ptr<registry> r, SparseArray<component::position> &positions,
    SparseArray<component::velocity> &velocities, SparseArray<component::drawable> &drawables, SparseArray<component::controllable> &controllables);

void spawn_randomly_power_up_damage(std::shared_ptr<registry> r, SparseArray<component::power_up_damage> &power_ups_damage);

void power_up_damage_system(std::shared_ptr<registry> r, SparseArray<component::position> &positions, SparseArray<component::velocity> &velocities,
    SparseArray<component::drawable> &drawables, SparseArray<component::power_up_damage> &power_ups_damage, SparseArray<component::player> &players);

void spawn_randomly_power_up_speed(std::shared_ptr<registry> r, SparseArray<component::power_up_speed> &power_ups_speed);

void power_up_speed_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone);

void spawn_randomly_power_up_health(std::shared_ptr<registry> r, SparseArray<component::power_up_health> &power_ups_health);

void power_up_health_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone);

void control_system(std::shared_ptr<registry> r, Command &cmd);

void bullet_system(std::shared_ptr<registry> r, SparseArray<component::bullet> &bullets,
    SparseArray<component::position> &positions, SparseArray<component::drawable> &drawables, SparseArray<component::velocity> &velocities);

void collision_player_system(std::shared_ptr<registry> r, std::function<void(const Datatype &)> sendToEveryone);

void collision_enemy_system(std::shared_ptr<registry> r, std::function<void(const Datatype &)> sendToEveryone);

bool win_system(std::shared_ptr<registry> r);

bool lose_system(std::shared_ptr<registry> r);

void spawn_boss(std::shared_ptr<registry> r, float x, float y, std::size_t nbPlayers);

void spawn_boss_two(std::shared_ptr<registry> r, float x, float y, std::size_t nbPlayers);

void boss_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone);

void network_system(std::shared_ptr<registry> r, std::function<void(const Datatype &)> send);

#endif /* !SYSTEM_HPP_ */
