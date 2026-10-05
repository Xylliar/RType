/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** Scene
*/

#include "Scene.hpp"
#include <fstream>
#include <sstream>

namespace rType {
    Scene::Scene(std::string path) : _path(path)
    {
        loadScenes();
    }

    Scene::~Scene()
    {
    }

    void Scene::loadScenes()
    {
        std::ifstream file(_path);
        std::string line;
        std::vector<std::size_t> currentScene;
        bool inScene = false;

        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << _path << std::endl;
            return;
        }
        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            if (line.empty() || line[0] == '#')
                continue;
            if (line == "START") {
                inScene = true;
                currentScene.clear();
            } else if (line == "END") {
                inScene = false;
                arrayScene.push_back(currentScene);
            } else if (inScene) {
                currentScene.push_back(std::atoi(line.c_str()));
            }
        }
        file.close();
    }


    std::vector<std::vector<std::size_t>>& Scene::getScenes()
    {
        return arrayScene;
    }

    std::ostream& operator<<(std::ostream& os, Scene& scene)
    {
        const auto& scenes = scene.getScenes();
        for (const auto& currentScene : scenes) {
            os << "Scene:" << std::endl;
            for (const auto& line : currentScene) {
                os << line << std::endl;
            }
        }
        return os;
    }
}