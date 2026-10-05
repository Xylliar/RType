/*
** EPITECH PROJECT, 2025
** rtape
** File description:
** Scene
*/

#ifndef SCENE_HPP_
    #define SCENE_HPP_
    #include <iostream>
    #include <vector>
    #include <string>

namespace rType {
    class Scene {
        public:
            Scene(std::string path);
            ~Scene();
            std::vector<std::vector<std::size_t>>& getScenes();
        private:
            std::string _path;
            std::vector<std::vector<std::size_t>> arrayScene;
            void loadScenes(void);
            void ParseFile(void);
    };
    std::ostream& operator<<(std::ostream& os, const Scene& scene);
};
#endif /* !SCENE_HPP_ */
