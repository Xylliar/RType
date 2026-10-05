/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** Registry
*/

#include "Registry.hpp"
#include <iostream>

extern "C" registry* create_registry() {
    return new registry();
}
