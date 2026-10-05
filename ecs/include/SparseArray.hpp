/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** SparseArray
*/

#ifndef SPARSEARRAY_HPP
#define SPARSEARRAY_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>
#include <any>
#include <typeindex>
#include "Entity.hpp"

template <typename Component>
class SparseArray {
public:
    using value_type = Component;
    using reference_type = value_type&;
    using const_reference_type = value_type const &;
    using container_t = std::vector<value_type>;
    using size_type = typename container_t::size_type;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

public:
    SparseArray() = default;

    SparseArray(SparseArray const & cp) {
        _data = cp._data;
    }

    SparseArray(SparseArray && mv) noexcept {
        _data = std::move(mv._data);
    }

    SparseArray &operator=(SparseArray const &cp) {
        _data = cp._data;
        return *this;
    }

    SparseArray &operator=(SparseArray &&mv) noexcept {
        _data = std::move(mv._data);
        return *this;
    }

    reference_type operator[](size_t idx) {
        return _data[idx];
    }

    const_reference_type operator[](size_t idx) const {
        return _data[idx];
    }

    iterator begin() {
        return _data.begin();
    }

    const_iterator begin() const {
        return _data.begin();
    }

    const_iterator cbegin() const {
        return _data.cbegin();
    }

    iterator end() {
        return _data.end();
    }

    const_iterator end() const {
        return _data.end();
    }

    const_iterator cend() const {
        return _data.cend();
    }

    size_type size() const {
        return _data.size();
    }

    reference_type insert_at(size_type pos, Component &&component) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = std::move(component);
        return _data[pos];
    }

    reference_type insert_at(size_type pos, Component const &component) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = component;
        return _data[pos];
    }

    template <class... Params>
    reference_type emplace_at(size_type pos, Params&&... params) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = Component(std::forward<Params>(params)...);
        return _data[pos];
    }

    void erase(size_type pos) {
        if (pos < _data.size()) {
            _data.erase(_data.begin() + pos);
        }
    }

    size_type get_index(value_type const &component) const {
        for (size_type i = 0; i < _data.size(); ++i) {
            if (std::addressof(_data[i]) == std::addressof(component))
                return i;
        }
        return static_cast<size_type>(-1);
    }

private:
    container_t _data;
};

#endif // SPARSEARRAY_HPP
