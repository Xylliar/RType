/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** ECSLoader
*/

#ifndef ECSLOADER_HPP_
#define ECSLOADER_HPP_

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../../ecs/include/Registry.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace rType {
    class ECSLoader {
    public:
        explicit ECSLoader(const std::string &path) {
            #ifdef _WIN32
                HMODULE handle = LoadLibrary(path.c_str());
                if (!handle) {
                    throw std::runtime_error("Error loading " + path + ": Unable to load DLL.");
                }
                _handle = static_cast<void *>(handle);
            #else
                auto handle = dlopen(path.c_str(), RTLD_NOW);
                if (!handle) {
                    throw std::runtime_error("Error loading " + path + ": " + dlerror());
                }
                _handle = handle;
            #endif
        }

        ~ECSLoader() {
            if (_handle) {
                #ifdef _WIN32
                    FreeLibrary(static_cast<HMODULE>(_handle));
                #else
                    dlclose(_handle);
                #endif
            }
        }

        template <typename T>
        T* getInstance(const std::string &entry_point) {
            #ifdef _WIN32
                auto create_raw_object = reinterpret_cast<T* (*)()>(
                    GetProcAddress(static_cast<HMODULE>(_handle), entry_point.c_str()));
                if (!create_raw_object) {
                    throw std::runtime_error("Error with entry point \"" + entry_point + "\": Unable to find function.");
                }
            #else
                auto create_raw_object = reinterpret_cast<T* (*)()>(dlsym(_handle, entry_point.c_str()));
                if (!create_raw_object) {
                    throw std::runtime_error("Error with entry point \"" + entry_point + "\": " + dlerror());
                }
            #endif
            return create_raw_object();
        }

    private:
        void *_handle = nullptr;
    };
}

#endif /* !ECSLOADER_HPP_ */
