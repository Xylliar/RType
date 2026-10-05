/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** StringUtils
*/

#ifndef STRINGUTILS_HPP_
    #define STRINGUTILS_HPP_

    #include <algorithm>
    #include <cctype>
    #include <string>

namespace utils {
    class StringUtils {
        public:
            ~StringUtils() = default;

            static std::string toLower(const std::string &str)
            {
                std::string lowerStr = str;
                std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return std::tolower(c); });
                std::string::iterator _ = lowerStr.erase(std::remove(lowerStr.begin(), lowerStr.end(), ' '), lowerStr.end());
                return lowerStr;
            }

            static bool isValidUsername(const std::string &name)
            {
                return !name.empty() && name.size() <= 12 && std::none_of(name.begin(), name.end(), [](unsigned char c) {
                    return c < 0x20 || c == 0x7F;
                });
            }

        private:
            StringUtils();
    };
}

#endif /* !STRINGUTILS_HPP_ */
