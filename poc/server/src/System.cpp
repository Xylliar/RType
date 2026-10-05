/*
** EPITECH PROJECT, 2024
** rtape
** File description:
** system
*/

#include "Component.hpp"
#include "Server.hpp"
#include <random>

void init_spawn_targets(std::shared_ptr<registry> r) {
    for (int i = 0; i < 4; i++) {
        spawn_target(r);
    }
}

void spawn_target(std::shared_ptr<registry> r) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, 1072);
    std::uniform_int_distribution<> disY(0, 672);

    bool positionValid = false;
    float x, y;

    while (!positionValid) {
        x = static_cast<float>(disX(gen));
        y = static_cast<float>(disY(gen));
        positionValid = true;

        auto &positions = r->get_components<component::position>();
        for (const auto &pos : positions) {
            if (x < pos.x + 128 && x + 128 > pos.x && y < pos.y + 128 && y + 128 > pos.y) {
                positionValid = false;
                break;
            }
        }
    }

    Entity target = r->spawn_entity();
    r->add_component<component::position>(target, {x, y});
    r->add_component<component::drawable>(target, {"Target"});
}

void target_system(std::shared_ptr<registry> r) {
    auto &positions = r->get_components<component::position>();

    if (positions.size() < 4) {
        spawn_target(r);
    }
}

void hit_system(std::shared_ptr<registry> r) {
    auto &players = r->get_components<component::player>();
    auto &positions = r->get_components<component::position>();
    auto &drawables = r->get_components<component::drawable>();

    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].shouldShoot) {
            for (size_t j = 0; j < positions.size(); j++) {
                if (drawables[j].id == "Target" &&
                    players[i].shootX >= positions[j].x && players[i].shootX <= positions[j].x + 128 &&
                    players[i].shootY >= positions[j].y && players[i].shootY <= positions[j].y + 128) {
                    Entity target = r->entity_from_index(j);
                    r->kill_entity(target);
                    spawn_target(r);
                    players[i].score++;
                    break;
                }
            }
            players[i].shouldShoot = false;
        }
    }
}

void control_system(std::shared_ptr<registry> r, Command &cmd)
{
    auto &players = r->get_components<component::player>();

    for (std::size_t i = 0; i < players.size(); i++) {
        if (players[i].id == cmd.playerId) {
            players[i].shouldShoot = true;
            players[i].shootX = cmd.posX;
            players[i].shootY = cmd.posY;
        }
    }
}

Datatype end_system(std::shared_ptr<registry> r)
{
    auto &players = r->get_components<component::player>();
    Datatype data;
    data.id = 0x51;
    data.length = 0;

    for (size_t i = 0; i < players.size(); i++) {
        uint16_t id = players[i].id;
        uint16_t score = players[i].score;

        data.length += 4;
        data.payload.emplace_back((id >> 8) & 0xFF);
        data.payload.emplace_back(id & 0xFF);
        data.payload.emplace_back((score >> 8) & 0xFF);
        data.payload.emplace_back(score & 0xFF);
    }
    return data;
}

static void add_position(Datatype &msg, uint8_t id, const component::position &pos) {
    uint16_t x = pos.x;
    uint16_t y = pos.y;

    msg.length += 5;
    msg.payload.emplace_back(id);
    msg.payload.emplace_back((x >> 8) & 0xFF);
    msg.payload.emplace_back(x & 0xFF);
    msg.payload.emplace_back((y >> 8) & 0xFF);
    msg.payload.emplace_back(y & 0xFF);
}

Datatype network_system(std::shared_ptr<registry> r) {
    const auto &positions = r->get_components<component::position>();
    const auto &players = r->get_components<component::player>();
    Datatype msg;
    msg.id = 0x14;
    msg.length = 0;

    for (const auto &pos : positions) {
        if (pos.x && pos.y) {
            add_position(msg, 0x15, pos);
        }
    }
    for (const auto &player : players) {
        msg.length += 5;
        msg.payload.emplace_back(0x16);
        msg.payload.emplace_back((player.id >> 8) & 0xFF);
        msg.payload.emplace_back(player.id & 0xFF);
        msg.payload.emplace_back((player.score >> 8) & 0xFF);
        msg.payload.emplace_back(player.score & 0xFF);
    }
    return msg;
}