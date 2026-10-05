/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Entity
*/

#ifndef ENTITY_HPP_
    #define ENTITY_HPP_

    #include <stdio.h>

class Entity {
    public:
        explicit Entity(size_t id = 0) : _id(id) {};
        operator size_t() const { return _id;};
        size_t getId(void) const {return _id;};
    private:
        size_t _id;
};

#endif /* !ENTITY_HPP_ */
