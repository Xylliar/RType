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

        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << _path << std::endl;
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
            if (line.empty() || line[0] == '#')
                continue;
            arrayScene.push_back(line);
        }
        file.close();
    }

    std::vector<std::string>& Scene::getScenes()
    {
        return arrayScene;
    }

    std::ostream& operator<<(std::ostream& os, Scene& scene)
    {
        const auto& scenes = scene.getScenes();
        os << "Scene:" << std::endl;
        for (const auto& currentScene : scenes) {
            os << currentScene << std::endl;
        }
        return os;
    }
}