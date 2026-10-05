/*
** EPITECH PROJECT, 2025
** RType
** File description:
** System
*/

#include "Component.hpp"
#include "Server.hpp"
#include <random>

constexpr int WINDOW_WIDTH = 1900;
constexpr int WINDOW_HEIGHT = 1000;
constexpr float POWER_UP_SPEED = 1.5f;
constexpr float POWER_UP_DAMAGE = 15;

void spawn_enemy(std::shared_ptr<registry> r, float x, float y) {
    Entity enemy = r->spawn_entity();
    r->add_component<component::position>(enemy, component::position{x, y});
    r->add_component<component::drawable>(enemy, component::drawable{"./src/r-typesheet24.gif", "Enemy"});
    r->add_component<component::shootable>(enemy, component::shootable{true});
    r->add_component<component::velocity>(enemy, component::velocity{5.0f, 0, 5.0f});
    r->add_component<component::enemy>(enemy, component::enemy{false, 20, true, false, 1});
}

void spawn_lateral_enemy(std::shared_ptr<registry> r, float x, float y) {
    Entity enemy = r->spawn_entity();
    r->add_component<component::position>(enemy, component::position{x, y});
    r->add_component<component::drawable>(enemy, component::drawable{"./src/r-typesheet1.gif", "LateralEnemy"});
    r->add_component<component::velocity>(enemy, component::velocity{5.0f, 5.0f, 0});
    r->add_component<component::enemy>(enemy, component::enemy{false, 20, true, false, 2});
}

void spawn_wave_enemy(std::shared_ptr<registry> r, float x, float y) {
    Entity enemy = r->spawn_entity();
    r->add_component<component::position>(enemy, component::position{x, y});
    r->add_component<component::drawable>(enemy, component::drawable{"./src/r-typesheet1.gif", "WaveEnemy"});
    r->add_component<component::velocity>(enemy, component::velocity{7.0f, 7.0f, 5.0f});
    r->add_component<component::enemy>(enemy, component::enemy{false, 10, true, false, 4});
}

void spawn_boss(std::shared_ptr<registry> r, float x, float y, std::size_t nbPlayer) {
    Entity enemy = r->spawn_entity();
    r->add_component<component::position>(enemy, component::position{x, y});
    r->add_component<component::drawable>(enemy, component::drawable{"./src/r-typesheet24.gif", "Boss"});
    r->add_component<component::shootable>(enemy, component::shootable{true});
    r->add_component<component::velocity>(enemy, component::velocity{0, 0, 0});
    r->add_component<component::enemy>(enemy, component::enemy{false, (200 * static_cast<int>(nbPlayer)), true, false, 3});
}

void spawn_boss_two(std::shared_ptr<registry> r, float x, float y, std::size_t nbPlayer) {
    Entity enemy = r->spawn_entity();
    r->add_component<component::position>(enemy, component::position{x, y});
    r->add_component<component::drawable>(enemy, component::drawable{"./src/r-typesheet24.gif", "Boss2"});
    r->add_component<component::shootable>(enemy, component::shootable{true});
    r->add_component<component::velocity>(enemy, component::velocity{1.0f, 0, 1.0f});
    r->add_component<component::enemy>(enemy, component::enemy{false, (200 * static_cast<int>(nbPlayer)), true, false, 5});
}

void spawn_enemy_bullet(std::shared_ptr<registry> r, float x, float y)
{
    Entity bullet = r->spawn_entity();
    r->add_component<component::position>(bullet, component::position{x, y});
    r->add_component<component::velocity>(bullet, component::velocity{25.0f, 25.0f, 0});
    r->add_component<component::drawable>(bullet, component::drawable{"./src/r-typesheet7.gif", "Enemy bullet"});
    r->add_component<component::bullet>(bullet, component::bullet{0, 20});
}

void spawn_boss_one_bullet(std::shared_ptr<registry> r, float x, float y)
{
    Entity bullet = r->spawn_entity();
    r->add_component<component::position>(bullet, component::position{x, y});
    r->add_component<component::velocity>(bullet, component::velocity{25.0f, 25.0f, 0});
    r->add_component<component::drawable>(bullet, component::drawable{"./src/r-typesheet7.gif", "Boss1 bullet"});
    r->add_component<component::bullet>(bullet, component::bullet{0, 20, 30});
}

void spawn_boss_two_bullet(std::shared_ptr<registry> r, float x, float y)
{
    Entity bullet = r->spawn_entity();
    r->add_component<component::position>(bullet, component::position{x, y});
    r->add_component<component::velocity>(bullet, component::velocity{50.0f, 50.0f, 0});
    r->add_component<component::drawable>(bullet, component::drawable{"./src/r-typesheet7.gif", "Boss2 bullet"});
    r->add_component<component::bullet>(bullet, component::bullet{0, 30, 60});
}

void spawn_player_bullet(std::shared_ptr<registry> r, float x, float y, float damage)
{
    Entity bullet = r->spawn_entity();
    r->add_component<component::position>(bullet, component::position{x, y});
    r->add_component<component::velocity>(bullet, component::velocity{25.0f, 25.0f, 0});
    r->add_component<component::drawable>(bullet, component::drawable{"./src/r-typesheet3.gif", "Player bullet"});
    r->add_component<component::bullet>(bullet, component::bullet{0, 20, static_cast<int>(damage)});
    r->add_component<component::clock>(bullet, component::clock{0.1f, sf::Clock()});
}

void spawn_power_up_damage(std::shared_ptr<registry> r, float x, float y)
{
    Entity power_up = r->spawn_entity();
    r->add_component<component::position>(power_up, component::position{x, y});
    r->add_component<component::drawable>(power_up, component::drawable{"./src/r-typesheet3.gif", "Power up damage"});
    r->add_component<component::velocity>(power_up, component::velocity{5.0f, 5.0f, 0.0f});
    r->add_component<component::power_up_damage>(power_up, component::power_up_damage{"damage", false, true});
}

void spawn_power_up_speed(std::shared_ptr<registry> r, float x, float y)
{
    Entity power_up = r->spawn_entity();
    r->add_component<component::position>(power_up, component::position{x, y});
    r->add_component<component::drawable>(power_up, component::drawable{"./src/r-typesheet3.gif", "Power up speed"});
    r->add_component<component::velocity>(power_up, component::velocity{5.0f, 5.0f, 0.0f});
    r->add_component<component::power_up_speed>(power_up, component::power_up_speed{"speed", false, true});
}

void spawn_power_up_health(std::shared_ptr<registry> r, float x, float y)
{
    Entity power_up = r->spawn_entity();
    r->add_component<component::position>(power_up, component::position{x, y});
    r->add_component<component::drawable>(power_up, component::drawable{"./src/r-typesheet3.gif", "Power up health"});
    r->add_component<component::velocity>(power_up, component::velocity{5.0f, 5.0f, 0.0f});
    r->add_component<component::power_up_health>(power_up, component::power_up_health{"health", false, true});
}

void player_system(std::shared_ptr<registry> r, SparseArray<component::position> &positions,
    SparseArray<component::velocity> &velocities, SparseArray<component::drawable> &drawables, SparseArray<component::controllable> &controllables)
{
    for (std::size_t i = 0; i < velocities.size() && i < velocities.size() && i < drawables.size() && i < controllables.size(); i++) {
        auto &position = positions[i];
        auto &velocity = velocities[i];
        auto &drawable = drawables[i];
        if (drawable.id == "Player") {
            if (i < controllables.size()) {
                if (position.x + velocity.vx > 0 && (position.x + velocity.vx) < WINDOW_WIDTH - 65) {
                    position.x += velocity.vx;
                }
                if (position.y + velocity.vy > 0 && (position.y + velocity.vy) < WINDOW_HEIGHT - 100) {
                    position.y += velocity.vy;
                }
            } else {
                position.x += velocity.vx;
                position.y += velocity.vy;
            }
            velocity.vx = 0;
            velocity.vy = 0;
        }
    }
}

void boss_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone)
{
    auto &bosses = r->get_components<component::enemy>();
    auto &positions = r->get_components<component::position>();
    auto &drawables = r->get_components<component::drawable>();
    auto &velocities = r->get_components<component::velocity>();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 150);
    Datatype data;
    data.id = 0x40;
    data.length = 0;

    for (std::size_t i = 0; i < bosses.size(); i++) {
        if (drawables[i].id != "Boss" && drawables[i].id != "Boss2")
            continue;
        if (bosses[i].display == false)
            continue;
        if (i >= positions.size() || i >= drawables.size() || i >= velocities.size())
            continue;
        auto &velocity = velocities[i];
        auto &position = positions[i];
        if (position.x + velocity.vx > 0 && (position.x + velocity.vx) < WINDOW_WIDTH - 100) {
            position.x += velocity.vx;
        } else {
            velocity.vx = -velocity.vx;
        }
        if (position.y + velocity.vy > 0 && (position.y + velocity.vy) < WINDOW_HEIGHT - 200) {
            position.y += velocity.vy;
        } else {
            velocity.vy = -velocity.vy;
        }
        if (dis(gen) == 1) {
            bosses[i].shouldShoot = true;
        }
    }

    for (std::size_t i = 0; i < bosses.size(); i++) {
        if (i >= positions.size())
            continue;

        if (bosses[i].shouldShoot) {
            if (drawables[i].id == "Boss2") {
                spawn_wave_enemy(r, positions[i].x + 400, positions[i].y + 122);
                spawn_boss_two_bullet(r, positions[i].x + 49, positions[i].y + 122);
                data.payload.emplace_back(0x22);
                data.payload.emplace_back(0x23);
                data.payload.emplace_back((static_cast<uint16_t>(positions[i].x - 50) >> 8) & 0xFF);
                data.payload.emplace_back(static_cast<uint16_t>(positions[i].x - 50) & 0xFF);
                data.payload.emplace_back((static_cast<uint16_t>(positions[i].y) >> 8) & 0xFF);
                data.payload.emplace_back(static_cast<uint16_t>(positions[i].y) & 0xFF);
                data.length += 6;
            } else {
                spawn_boss_one_bullet(r, positions[i].x, positions[i].y + 292);
                spawn_boss_one_bullet(r, positions[i].x + 30, positions[i].y + 292);
                spawn_boss_one_bullet(r, positions[i].x + 60, positions[i].y + 292);
                data.payload.emplace_back(0x22);
                data.payload.emplace_back(0x20);
                data.payload.emplace_back((static_cast<uint16_t>(positions[i].x - 50) >> 8) & 0xFF);
                data.payload.emplace_back(static_cast<uint16_t>(positions[i].x - 50) & 0xFF);
                data.payload.emplace_back((static_cast<uint16_t>(positions[i].y) >> 8) & 0xFF);
                data.payload.emplace_back(static_cast<uint16_t>(positions[i].y) & 0xFF);
                data.length += 6;
            }
            bosses[i].shouldShoot = false;
        }
    }
    if (data.length > 0) {
        sendToEveryone(data);
    }
}

void shoot_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone)
{
    auto &players = r->get_components<component::player>();
    auto &positions = r->get_components<component::position>();
    auto &cooldowns = r->get_components<component::cooldown>();
    static sf::Clock deltaTimeClock;
    const float deltaTime = deltaTimeClock.restart().asSeconds();
    const auto &power_up_damage = r->get_components<component::power_up_damage>();
    Datatype data;
    data.id = 0x40;
    data.length = 0;

    for (std::size_t i = 0; i < players.size() && i < positions.size(); ++i) {
        auto &player = players[i];

        if (!player.shouldShoot) {
            player.regenerationTime = std::min(player.regenerationTime + deltaTime, player.maxRegeneration);
            continue;
        }

        auto &cooldown = cooldowns[i];
        if (cooldown.clock.getElapsedTime().asSeconds() < cooldown.duration) {
            continue;
        }

        const float regenerationPercent = player.regenerationTime / player.maxRegeneration;
        float damage = player.baselineDamage + (player.maxDamage - player.baselineDamage) * regenerationPercent;
        data.length += 6;
        data.payload.emplace_back(0x42);
        data.payload.emplace_back(0x15);
        if (player.hasPowerUpDamage == true) {
            spawn_player_bullet(r, positions[i].x, positions[i].y, (damage + POWER_UP_DAMAGE));
            data.payload.emplace_back((static_cast<uint16_t>(positions[i].x + 90) >> 8) & 0xFF);
            data.payload.emplace_back(static_cast<uint16_t>(positions[i].x + 90) & 0xFF);
            data.payload.emplace_back((static_cast<uint16_t>(positions[i].y) >> 8) & 0xFF);
            data.payload.emplace_back(static_cast<uint16_t>(positions[i].y) & 0xFF);
        } else {
            spawn_player_bullet(r, positions[i].x, positions[i].y, damage);
            data.payload.emplace_back((static_cast<uint16_t>(positions[i].x + 60) >> 8) & 0xFF);
            data.payload.emplace_back(static_cast<uint16_t>(positions[i].x + 60) & 0xFF);
            data.payload.emplace_back((static_cast<uint16_t>(positions[i].y) >> 8) & 0xFF);
            data.payload.emplace_back(static_cast<uint16_t>(positions[i].y) & 0xFF);
        }
        player.shouldShoot = false;
        player.regenerationTime = 0.0f;
        cooldown.clock.restart();
    }
    if (data.length > 0) {
        sendToEveryone(data);
    }
}

void control_system(std::shared_ptr<registry> r, Command &cmd)
{
    auto &players = r->get_components<component::player>();
    auto &velocities = r->get_components<component::velocity>();

    for (std::size_t i = 0; i < players.size() && i < velocities.size(); i++) {
        if (players[i].id == cmd.playerId) {
            auto &velocity = velocities[i];
            if (cmd.type == 2) {
                r->kill_entity(r->entity_from_index(i));
                return;
            }
            if (cmd.type == 1) {
                players[i].shouldShoot = true;
            } else if (cmd.type == 0 && players[i].hasPowerUpSpeed == true) {
                if (cmd.param == 1) {
                    velocity.vy = -velocity.value * POWER_UP_SPEED;
                } else if (cmd.param == 2) {
                    velocity.vx = velocity.value * POWER_UP_SPEED;
                } else if (cmd.param == 3) {
                    velocity.vy = velocity.value * POWER_UP_SPEED;
                } else if (cmd.param == 4) {
                    velocity.vx = -velocity.value * POWER_UP_SPEED;
                }
            } else if (cmd.type == 0) {
                if (cmd.param == 1) {
                    velocity.vy = -velocity.value;
                } else if (cmd.param == 2) {
                    velocity.vx = velocity.value;
                } else if (cmd.param == 3) {
                    velocity.vy = velocity.value;
                } else if (cmd.param == 4) {
                    velocity.vx = -velocity.value;
                }
            }
        }
    }
}

void bullet_system(std::shared_ptr<registry> r, SparseArray<component::bullet> &bullets,
    SparseArray<component::position> &positions, SparseArray<component::drawable> &drawables, SparseArray<component::velocity> &velocities)
{
    for (std::size_t i = 0; i < bullets.size() && i < positions.size() && i < drawables.size() && i < velocities.size(); i++) {
        auto &position = positions[i];
        auto &velocity = velocities[i];
        auto &drawable = drawables[i];
        if (drawable.id == "Player bullet") {
            position.x += velocity.vx;
            if (position.x >= WINDOW_WIDTH) {
                Entity entity = r->entity_from_index(i);
                r->kill_entity(entity);
            }
        }
        if (drawable.id == "Enemy bullet" || drawable.id == "Boss1 bullet" || drawable.id == "Boss2 bullet") {
            position.x -= velocity.vx;
            if (position.x <= 0) {
                Entity entity = r->entity_from_index(i);
                r->kill_entity(entity);
            }
        }
    }
}

void enemy_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone)
{
    auto &enemies = r->get_components<component::enemy>();
    auto &positions = r->get_components<component::position>();
    auto &drawables = r->get_components<component::drawable>();
    auto &velocities = r->get_components<component::velocity>();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 150);
    Datatype data;
    data.id = 0x40;
    data.length = 0;

    for (std::size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i].id != 1)
            continue;
        if (enemies[i].display == false)
            continue;
        if (i >= positions.size() || i >= velocities.size() || i >= drawables.size())
            continue;
        if (drawables[i].id != "Enemy")
            continue;

        if (!enemies[i].isUp) {
            positions[i].y += velocities[i].vy;
            if (positions[i].y >= WINDOW_HEIGHT - 100) {
                enemies[i].isUp = true;
            }
        } else {
            positions[i].y -= velocities[i].vy;
            if (positions[i].y <= 50) {
                enemies[i].isUp = false;
            }
        }

        if (dis(gen) == 1) {
            enemies[i].shouldShoot = true;
        }
    }

    for (std::size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i].id != 1)
            continue;
        if (i >= positions.size())
            continue;

        if (enemies[i].shouldShoot) {
            spawn_enemy_bullet(r, positions[i].x, positions[i].y);
            enemies[i].shouldShoot = false;
            data.payload.emplace_back(0x43);
            data.payload.emplace_back(0x15);
            data.payload.emplace_back((static_cast<uint16_t>(positions[i].x - 20) >> 8) & 0xFF);
            data.payload.emplace_back(static_cast<uint16_t>(positions[i].x - 20) & 0xFF);
            data.payload.emplace_back((static_cast<uint16_t>(positions[i].y) >> 8) & 0xFF);
            data.payload.emplace_back(static_cast<uint16_t>(positions[i].y) & 0xFF);
            data.length += 6;
        }
    }
    if (data.length > 0) {
        sendToEveryone(data);
    }
}

void enemy_wave_system(std::shared_ptr<registry> r, SparseArray<component::position> &positions, SparseArray<component::velocity> &velocities,
    SparseArray<component::drawable> &drawables)
{
    for (std::size_t i = 0; i < positions.size() && i < velocities.size() && i < drawables.size(); i++) {
        if (drawables[i].id != "WaveEnemy")
            continue;

        positions[i].x -= velocities[i].vx;
        if (positions[i].y + velocities[i].vy > WINDOW_HEIGHT - 100) {
            velocities[i].vy = -velocities[i].vy;
        } else if (positions[i].y + velocities[i].vy < 50) {
            velocities[i].vy = -velocities[i].vy;
        }
        positions[i].y += velocities[i].vy;
        if (positions[i].x < -60) {
            Entity enemy = r->entity_from_index(i);
            r->kill_entity(enemy);
        }
    }
}

void enemy_lateral_system(std::shared_ptr<registry> r, SparseArray<component::position> &positions, SparseArray<component::velocity> &velocities,
    SparseArray<component::drawable> &drawables)
{
    for (std::size_t i = 0; i < positions.size() && i < velocities.size() && i < drawables.size(); i++) {
        if (drawables[i].id != "LateralEnemy")
            continue;

        positions[i].x -= velocities[i].vx;
        if (positions[i].x < -60) {
            Entity enemy = r->entity_from_index(i);
            r->kill_entity(enemy);
        }
    }
}

void power_up_damage_system(std::shared_ptr<registry> r, SparseArray<component::position> &positions, SparseArray<component::velocity> &velocities,
    SparseArray<component::drawable> &drawables, SparseArray<component::power_up_damage> &power_ups_damage, SparseArray<component::player> &players)
{
    for (std::size_t i = 0; i < power_ups_damage.size() && i < drawables.size() && i < positions.size(); i++) {
        if (drawables[i].id == "Power up damage" && power_ups_damage[i].display == true && power_ups_damage[i].isAttach == false) {
            positions[i].x -= velocities[i].vx;
            if (positions[i].x < -60) {
                Entity power_up = r->entity_from_index(i);
                r->kill_entity(power_up);
            } else {
                for (std::size_t j = 0; j < players.size(); j++) {
                    if (positions[j].x <= positions[i].x &&
                        positions[i].x <= positions[j].x + 67 &&
                        positions[j].y <= positions[i].y &&
                        positions[i].y <= positions[j].y + 29 && power_ups_damage[i].isAttach == false) {
                        power_ups_damage[i].isAttach = true;
                        players[j].hasPowerUpDamage = true;
                    }
                }
            }
        }
    }
}

void spawn_randomly_power_up_damage(std::shared_ptr<registry> r, SparseArray<component::power_up_damage> &power_ups_damage)
{
    if (power_ups_damage.size() > 1) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2500);
    if (dis(gen) <= 1) {
        float randomY = std::uniform_int_distribution<>(50, 700)(gen);
        spawn_power_up_damage(r, 1600.0f, randomY);
    }
}

void power_up_speed_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone)
{
    auto &power_ups_speed = r->get_components<component::power_up_speed>();
    auto &positions = r->get_components<component::position>();
    auto &velocities = r->get_components<component::velocity>();
    auto &drawables = r->get_components<component::drawable>();
    auto &players = r->get_components<component::player>();
    Datatype data;
    data.id = 0x40;
    data.length = 0;

    for (std::size_t i = 0; i < power_ups_speed.size() && i < drawables.size() && i < positions.size(); i++) {
        if (drawables[i].id == "Power up speed" && power_ups_speed[i].display == true && power_ups_speed[i].isTouch == false) {
            positions[i].x -= velocities[i].vx;
            if (positions[i].x < -60) {
                Entity power_up = r->entity_from_index(i);
                r->kill_entity(power_up);
            } else {
                for (std::size_t j = 0; j < players.size(); j++) {
                    if (positions[j].x <= positions[i].x &&
                        positions[i].x <= positions[j].x + 67 &&
                        positions[j].y <= positions[i].y &&
                        positions[i].y <= positions[j].y + 29 && power_ups_speed[i].isTouch == false) {
                        power_ups_speed[i].isTouch = true;
                        players[j].hasPowerUpSpeed = true;
                        data.payload.emplace_back(0x44);
                        data.payload.emplace_back(0x16);
                        data.payload.emplace_back((static_cast<uint16_t>(positions[j].x) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(positions[j].x));
                        data.payload.emplace_back((static_cast<uint16_t>(positions[j].y) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(positions[j].y));
                        data.length += 6;
                        Entity power_up = r->entity_from_index(i);
                        r->kill_entity(power_up);
                    }
                }
            }
        }
    }
    if (data.length > 0) {
        sendToEveryone(data);
    }
}

void spawn_randomly_power_up_speed(std::shared_ptr<registry> r, SparseArray<component::power_up_speed> &power_ups_speed)
{
    if (power_ups_speed.size() > 2) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2500);
    if (dis(gen) <= 1) {
        float randomY = std::uniform_int_distribution<>(50, 700)(gen);
        spawn_power_up_speed(r, 1600.0f, randomY);
    }
}

void power_up_health_system(std::shared_ptr<registry> r, std::function<void(const Datatype &data)> sendToEveryone)
{
    auto &power_ups_health = r->get_components<component::power_up_health>();
    auto &positions = r->get_components<component::position>();
    auto &velocities = r->get_components<component::velocity>();
    auto &drawables = r->get_components<component::drawable>();
    auto &players = r->get_components<component::player>();
    Datatype data;
    data.id = 0x40;
    data.length = 0;

    for (std::size_t i = 0; i < power_ups_health.size() && i < drawables.size() && i < positions.size(); i++) {
        if (drawables[i].id == "Power up health" && power_ups_health[i].display == true && power_ups_health[i].isTouch == false) {
            positions[i].x -= velocities[i].vx;
            if (positions[i].x < -60) {
                Entity power_up = r->entity_from_index(i);
                r->kill_entity(power_up);
            } else {
                for (std::size_t j = 0; j < players.size(); j++) {
                    if (positions[j].x <= positions[i].x &&
                        positions[i].x <= positions[j].x + 67 &&
                        positions[j].y <= positions[i].y &&
                        positions[i].y <= positions[j].y + 29 && power_ups_health[i].isTouch == false) {
                        power_ups_health[i].isTouch = true;
                        players[j].health = 150;
                        data.payload.emplace_back(0x44);
                        data.payload.emplace_back(0x16);
                        data.payload.emplace_back((static_cast<uint16_t>(positions[j].x) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(positions[j].x));
                        data.payload.emplace_back((static_cast<uint16_t>(positions[j].y) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(positions[j].y));
                        data.length += 6;
                        Entity power_up = r->entity_from_index(i);
                        r->kill_entity(power_up);
                    }
                }
            }
        }
    }
    if (data.length > 0) {
        sendToEveryone(data);
    }
}

void spawn_randomly_power_up_health(std::shared_ptr<registry> r, SparseArray<component::power_up_health> &power_ups_health)
{
    if (power_ups_health.size() > 2) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2500);
    if (dis(gen) <= 1) {
        float randomY = std::uniform_int_distribution<>(50, 700)(gen);
        spawn_power_up_health(r, 1600.0f, randomY);
    }
}

void collision_player_system(std::shared_ptr<registry> r, std::function<void(const Datatype &)> sendToEveryone)
{
    auto &bullets = r->get_components<component::bullet>();
    auto &positions = r->get_components<component::position>();
    auto &drawables = r->get_components<component::drawable>();
    auto &enemies = r->get_components<component::enemy>();
    Datatype data;
    data.id = 0x40;
    data.length = 0;

    for (std::size_t i = 0; i < bullets.size() && i < positions.size() && i < drawables.size(); i++) {
        auto &bullet_position = positions[i];
        auto &bullet_drawable = drawables[i];

        for (std::size_t j = 0; j < enemies.size() && j < positions.size() && j < drawables.size(); j++) {
            auto &enemy = enemies[j];
            auto &enemy_position = positions[j];
            auto &enemy_drawable = drawables[j];

            if (bullet_drawable.id == "Player bullet" && (enemy_drawable.id == "Boss")) {
                if (enemy_position.x <= bullet_position.x &&
                    bullet_position.x <= enemy_position.x + 423 &&
                    enemy_position.y <= bullet_position.y &&
                    bullet_position.y <= enemy_position.y + 523) {
                    enemy.health -= bullets[i].damage;
                    if (enemy.health <= 0) {
                        data.payload.emplace_back(0x41);
                        data.payload.emplace_back(0x20);
                        data.payload.emplace_back((static_cast<uint16_t>(enemy_position.x) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(enemy_position.x));
                        data.payload.emplace_back((static_cast<uint16_t>(enemy_position.y) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(enemy_position.y));
                        data.length += 6;
                        Entity enemy_entity = r->entity_from_index(j);
                        r->kill_entity(enemy_entity);
                    }
                    data.payload.emplace_back(0x41);
                    data.payload.emplace_back(0x18);
                    data.payload.emplace_back((static_cast<uint16_t>(bullet_position.x) >> 8) & 0xFF);
                    data.payload.emplace_back(static_cast<uint16_t>(bullet_position.x));
                    data.payload.emplace_back((static_cast<uint16_t>(bullet_position.y) >> 8) & 0xFF);
                    data.payload.emplace_back(static_cast<uint16_t>(bullet_position.y));
                    data.length += 6;
                    Entity bullet_entity = r->entity_from_index(i);
                    r->kill_entity(bullet_entity);
                    break;
                }
            }
            if (bullet_drawable.id == "Player bullet" && (enemy_drawable.id == "Boss2")) {
                if (enemy_position.x <= bullet_position.x &&
                    bullet_position.x <= enemy_position.x + 594 &&
                    enemy_position.y <= bullet_position.y &&
                    bullet_position.y <= enemy_position.y + 186) {
                    enemy.health -= bullets[i].damage;
                    if (enemy.health <= 0) {
                        data.payload.emplace_back(0x41);
                        data.payload.emplace_back(0x23);
                        data.payload.emplace_back((static_cast<uint16_t>(enemy_position.x) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(enemy_position.x));
                        data.payload.emplace_back((static_cast<uint16_t>(enemy_position.y) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(enemy_position.y));
                        data.length += 6;
                        Entity enemy_entity = r->entity_from_index(j);
                        r->kill_entity(enemy_entity);
                    }
                    data.payload.emplace_back(0x41);
                    data.payload.emplace_back(0x18);
                    data.payload.emplace_back((static_cast<uint16_t>(bullet_position.x) >> 8) & 0xFF);
                    data.payload.emplace_back(static_cast<uint16_t>(bullet_position.x));
                    data.payload.emplace_back((static_cast<uint16_t>(bullet_position.y) >> 8) & 0xFF);
                    data.payload.emplace_back(static_cast<uint16_t>(bullet_position.y));
                    data.length += 6;
                    Entity bullet_entity = r->entity_from_index(i);
                    r->kill_entity(bullet_entity);
                    break;
                }
            }
            if (bullet_drawable.id == "Player bullet" &&
                (enemy_drawable.id == "Enemy" || enemy_drawable.id == "LateralEnemy" || enemy_drawable.id == "WaveEnemy")) {
                if (enemy_position.x <= bullet_position.x &&
                    bullet_position.x <= enemy_position.x + 60 &&
                    enemy_position.y <= bullet_position.y &&
                    bullet_position.y <= enemy_position.y + 60) {
                    enemy.health -= bullets[i].damage;
                    if (enemy.health <= 0) {
                        data.payload.emplace_back(0x41);
                        data.payload.emplace_back(0x17);
                        data.payload.emplace_back((static_cast<uint16_t>(enemy_position.x) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(enemy_position.x));
                        data.payload.emplace_back((static_cast<uint16_t>(enemy_position.y) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(enemy_position.y));
                        data.length += 6;
                        Entity enemy_entity = r->entity_from_index(j);
                        r->kill_entity(enemy_entity);
                    }
                    Entity bullet_entity = r->entity_from_index(i);
                    r->kill_entity(bullet_entity);
                    break;
                }
            }
        }
    }
    if (data.length > 0) {
        sendToEveryone(data);
    }
}

void collision_enemy_system(std::shared_ptr<registry> r, std::function<void(const Datatype &)> sendToEveryone)
{
    auto &bullets = r->get_components<component::bullet>();
    auto &positions = r->get_components<component::position>();
    auto &drawables = r->get_components<component::drawable>();
    auto &players = r->get_components<component::player>();
    Datatype data;
    data.id = 0x40;
    data.length = 0;

    for (std::size_t i = 0; i < bullets.size() && i < positions.size() && i < drawables.size(); i++) {
        auto &bullet_position = positions[i];
        auto &bullet_drawable = drawables[i];

        for (std::size_t j = 0; j < players.size() && i < positions.size() && i < drawables.size(); j++) {

            auto &player = players[j];
            auto &player_position = positions[j];
            auto &player_drawable = drawables[j];
            if ((bullet_drawable.id == "Enemy bullet" || bullet_drawable.id == "Boss1 bullet" || bullet_drawable.id == "Boss2 bullet") && player_drawable.id == "Player") {
                if (player_position.x <= bullet_position.x &&
                    bullet_position.x <= player_position.x + 67 &&
                    player_position.y <= bullet_position.y &&
                    bullet_position.y <= player_position.y + 29) {
                    player.health -= bullets[i].damage;
                    if (player.health <= 0) {
                        data.payload.emplace_back(0x41);
                        data.payload.emplace_back(0x15);
                        data.payload.emplace_back((static_cast<uint16_t>(player_position.x) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(player_position.x));
                        data.payload.emplace_back((static_cast<uint16_t>(player_position.y) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(player_position.y));
                        data.length += 6;
                        Entity player_entity = r->entity_from_index(j);
                        r->kill_entity(player_entity);
                    }
                    data.payload.emplace_back(0x41);
                    data.payload.emplace_back(0x24);
                    data.payload.emplace_back((static_cast<uint16_t>(bullet_position.x) >> 8) & 0xFF);
                    data.payload.emplace_back(static_cast<uint16_t>(bullet_position.x));
                    data.payload.emplace_back((static_cast<uint16_t>(bullet_position.y) >> 8) & 0xFF);
                    data.payload.emplace_back(static_cast<uint16_t>(bullet_position.y));
                    data.length += 6;
                    Entity bullet_entity = r->entity_from_index(i);
                    r->kill_entity(bullet_entity);
                    break;
                }
            }
        }
    }

    auto &enemies_pos = r->get_components<component::position>();
    auto &enemies_draw = r->get_components<component::drawable>();

    for (std::size_t i = 0; i < enemies_draw.size(); i++) {
        if (enemies_draw[i].id != "Enemy" && enemies_draw[i].id != "LateralEnemy" && enemies_draw[i].id != "WaveEnemy" && enemies_draw[i].id != "Boss")
            continue;

        for (std::size_t j = 0; j < players.size(); j++) {
            if (j >= positions.size() || j >= drawables.size())
                continue;

            if (drawables[j].id == "Player") {
                auto &player = players[j];
                if (enemies_pos[i].x <= positions[j].x + 60 &&
                    enemies_pos[i].x + 60 >= positions[j].x &&
                    enemies_pos[i].y <= positions[j].y + 60 &&
                    enemies_pos[i].y + 60 >= positions[j].y) {
                    if (enemies_draw[i].id == "WaveEnemy")
                        player.health -= 20;
                    else
                        player.health -= 40;
                    data.payload.emplace_back(0x41);
                    data.payload.emplace_back(0x17);
                    data.payload.emplace_back((static_cast<uint16_t>(enemies_pos[i].x) >> 8) & 0xFF);
                    data.payload.emplace_back(static_cast<uint16_t>(enemies_pos[i].x));
                    data.payload.emplace_back((static_cast<uint16_t>(enemies_pos[i].y) >> 8) & 0xFF);
                    data.payload.emplace_back(static_cast<uint16_t>(enemies_pos[i].y));
                    data.length += 6;
                    Entity enemy = r->entity_from_index(i);
                    r->kill_entity(enemy);
                    if (player.health <= 0) {
                        data.payload.emplace_back(0x41);
                        data.payload.emplace_back(0x15);
                        data.payload.emplace_back((static_cast<uint16_t>(positions[j].x) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(positions[j].x));
                        data.payload.emplace_back((static_cast<uint16_t>(positions[j].y) >> 8) & 0xFF);
                        data.payload.emplace_back(static_cast<uint16_t>(positions[j].y));
                        data.length += 6;
                        Entity player_entity = r->entity_from_index(j);
                        r->kill_entity(player_entity);
                    }
                }
            }
        }
    }
    if (data.length > 0) {
        sendToEveryone(data);
    }
}

bool win_system(std::shared_ptr<registry> r)
{
    auto &drawables = r->get_components<component::drawable>();

    int nb_ennemies = 0;
    for (size_t i = 0; i < drawables.size(); i++) {
        if (drawables[i].id == "Boss" || drawables[i].id == "Boss2") {
            nb_ennemies++;
        }
    }
    if (nb_ennemies == 0) {
        return true;
    }
    return false;
}

bool lose_system(std::shared_ptr<registry> r)
{
    auto &players = r->get_components<component::player>();
    if (players.size() == 0) {
        return true;
    }
    return false;
}

void logging_system(std::shared_ptr<registry> r)
{
    auto const &positions = r->get_components<component::position>();
    auto const &velocities = r->get_components<component::velocity>();

    for (size_t i = 0; i < positions.size() && i < velocities.size(); i++ ) {
        auto const &pos = positions[i];
        auto const &vel = velocities[i];

        if (pos.x && pos.y && vel.vx && vel.vy) {
            std::cerr << i << ": Position = {" << pos.x << ", " << pos.y
                     << "} , Velocity = {" << vel.vx << ", " << vel.vy << "}" << std::endl;
        }
    }
}

static void add_position(Datatype &msg, const uint8_t id, const component::position &pos) {
    uint16_t x = 0;
    uint16_t y = 0;

    if (pos.x > 0) {
        x = static_cast<uint16_t>(pos.x);
    }
    if (pos.y > 0) {
        y = static_cast<uint16_t>(pos.y);
    }

    msg.length += 5;
    msg.payload.emplace_back(id);
    msg.payload.emplace_back((x >> 8) & 0xFF);
    msg.payload.emplace_back(x & 0xFF);
    msg.payload.emplace_back((y >> 8) & 0xFF);
    msg.payload.emplace_back(y & 0xFF);
}

static void add_position_power_up_damage(Datatype &msg, const uint16_t id, const component::position &pos)
{
    uint16_t x = pos.x + 60;
    uint16_t y = pos.y;

    msg.length += 5;
    msg.payload.emplace_back(id);
    msg.payload.emplace_back((x >> 8) & 0xFF);
    msg.payload.emplace_back(x & 0xFF);
    msg.payload.emplace_back((y >> 8) & 0xFF);
    msg.payload.emplace_back(y & 0xFF);
}

static void add_health(Datatype &msg, int i_health)
{
    uint8_t health = 0;

    if (i_health > 0) {
        health = static_cast<uint8_t>(i_health);
    }
    msg.length += 1;
    msg.payload.emplace_back(health);
}

void network_system(std::shared_ptr<registry> r, std::function<void(const Datatype &)> sendToEveryone){
    const auto &positions = r->get_components<component::position>();
    const auto &bullets = r->get_components<component::bullet>();
    const auto &drawables = r->get_components<component::drawable>();
    const auto &players = r->get_components<component::player>();
    const auto &enemies = r->get_components<component::enemy>();
    const auto &power_ups_damage = r->get_components<component::power_up_damage>();
    const auto &power_ups_speed = r->get_components<component::power_up_speed>();
    const auto &power_ups_health = r->get_components<component::power_up_health>();

    Datatype msg;
    msg.id = 0x14;
    msg.length = 0;

    for (size_t i = 0; i < players.size(); ++i) {
        const auto &pos = positions[i];
        if (pos.x && pos.y) {
            add_position(msg, 0x15, pos);
            add_health(msg, players[i].health);
            msg.length += 1;
            msg.payload.emplace_back(players[i].id);
        }
    }
    const std::size_t playersBytes = msg.payload.size();
    for (size_t i = 0; i < bullets.size(); ++i) {
        const auto &pos = positions[i];
        const auto &bullet = bullets[i];
        const auto &drawable = drawables[i];
        if (bullet.velocity == 0)
            continue;
        if (pos.x && pos.y) {
            if (drawable.id == "Player bullet")
                add_position(msg, 0x18, pos);
            else if (drawable.id == "Enemy bullet")
                add_position(msg, 0x19, pos);
            else if (drawable.id == "Boss1 bullet")
                add_position(msg, 0x22, pos);
            else if (drawable.id == "Boss2 bullet")
                add_position(msg, 0x24, pos);
        }
    }
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].display == false)
            continue;
        const auto &pos = positions[i];
        if (pos.x && pos.y) {
            if (enemies[i].id == 1)
                add_position(msg, 0x16, pos);
            else if (enemies[i].id == 2)
                add_position(msg, 0x17, pos);
            else if (enemies[i].id == 3)
                add_position(msg, 0x20, pos);
            else if (enemies[i].id == 4)
                add_position(msg, 0x21, pos);
            else if (enemies[i].id == 5)
                add_position(msg, 0x23, pos);
        }
    }
    for (size_t i = 0; i < power_ups_damage.size(); ++i) {
        if (power_ups_damage[i].display == false || power_ups_damage[i].isAttach)
            continue;
        const auto &pos = positions[i];
        if (pos.x && pos.y) {
            add_position(msg, 0x25, pos);
        }
    }
    for (size_t j = 0; j < players.size() && j < positions.size(); j++) {
        if (players[j].hasPowerUpDamage && positions[j].x && positions[j].y) {
            add_position_power_up_damage(msg, 0x25, positions[j]);
        }
    }
    for (size_t i = 0; i < power_ups_speed.size(); ++i) {
        if (power_ups_speed[i].display == false)
            continue;
        const auto &pos = positions[i];
        add_position(msg, 0x26, pos);
    }
    for (size_t i = 0; i < power_ups_health.size(); ++i) {
        if (power_ups_health[i].display == false)
            continue;
        const auto &pos = positions[i];
        add_position(msg, 0x27, pos);
    }
    if (msg.payload.size() > MAX_PAYLOAD) {
        const std::size_t kept = playersBytes + ((MAX_PAYLOAD - playersBytes) / 5) * 5;
        msg.payload.resize(kept);
        msg.length = static_cast<uint16_t>(kept);
    }
    sendToEveryone(msg);
}
