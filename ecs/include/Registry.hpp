/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** Registry
*/

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "SparseArray.hpp"
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <any>
#include <memory>
#include <stdexcept>
#include "Entity.hpp"

class registry {
    public:
        template <class Component>
        SparseArray<Component> &register_component() {
            std::type_index type = std::type_index(typeid(Component));
            if (_components_arrays.find(type) != _components_arrays.end()) {
                throw std::runtime_error("Component already registered");
            }
            _components_arrays[type] = SparseArray<Component>();
            _components_erasers[type] = std::function<void(registry&, Entity const&)>(
                [](registry &reg, Entity const &e) {
                    reg.get_components<Component>().erase(e.getId());
                }
            );

            return std::any_cast<SparseArray<Component> &>(_components_arrays[type]);
        }

        template<class Component>
        SparseArray<Component> &get_components() {
            std::type_index type = std::type_index(typeid(Component));
            if (_components_arrays.find(type) == _components_arrays.end()) {
                throw std::runtime_error("Component not found");
            }
            return std::any_cast<SparseArray<Component> &>(_components_arrays[type]);
        }

        template<class Component>
        SparseArray<Component> const &get_components() const {
            std::type_index type = std::type_index(typeid(Component));
            if (_components_arrays.find(type) == _components_arrays.end()) {
                throw std::runtime_error("Component not found");
            }
            return std::any_cast<SparseArray<Component> const &>(_components_arrays.at(type));
        }

        Entity spawn_entity() {
            return Entity(_next_id++);
        }

        Entity entity_from_index(std::size_t id) {
            return Entity(id);
        }

        void kill_entity(Entity const &e) {
            for (auto &component_eraser : _components_erasers) {
                std::any_cast<std::function<void(registry&, Entity const&)>>(component_eraser.second)(*this, e);
            }
        }

        template<typename Component>
        typename SparseArray<Component>::reference_type add_component(Entity const &to, Component &&c) {
            return get_components<Component>().insert_at(to.getId(), std::forward<Component>(c));
        }

        template<typename Component, typename... Params>
        typename SparseArray<Component>::reference_type emplace_component(Entity const &to, Params&&... p) {
            return get_components<Component>().emplace_at(to.getId(), std::forward<Params>(p)...);
        }

        template<typename Component>
        void remove_component(Entity const &from) {
            get_components<Component>().erase(from.getId());
        }

        template <class... Components, typename Function>
        void add_system(Function &&f) {
            _systems.push_back([fwd = std::forward<Function>(f), this]() {
                fwd(std::shared_ptr<registry>(this, [](registry*){}), this->get_components<Components>()...);
            });
        }

        void run_systems() {
            for (auto &system : _systems) {
                system();
            }
        }

    private:
        std::unordered_map<std::type_index, std::any> _components_arrays;
        std::unordered_map<std::type_index, std::any> _components_erasers;
        std::size_t _next_id = 0;
        std::vector<std::function<void()>> _systems;
};

#ifdef _WIN32
    extern "C" __declspec(dllexport) registry *create_registry();
#else
    extern "C" registry *create_registry();
#endif

#endif // REGISTRY_HPP
