/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** Main
*/

#ifndef DISPLAYCONFIG_HPP_
    #define DISPLAYCONFIG_HPP_

namespace rType {
    constexpr int WINDOW_WIDTH = 1900;
    constexpr int WINDOW_HEIGHT = 1000;
    constexpr int FONT_SIZE_TITLE = 100;
    constexpr int FONT_SIZE_LARGE = 70;
    constexpr int FONT_SIZE_MEDIUM = 50;
    constexpr int FONT_SIZE_SMALL = 30;
    constexpr int FONT_SIZE_XSMALL = 20;
    constexpr float LIST_TEXT_SPACING = 90.0f;
    constexpr float HEADER_Y = 50.f;
    constexpr float TITLE_Y = HEADER_Y;
    constexpr float PADDING_X = 20.f;
    constexpr float PADDING_Y = 20.f;
    constexpr float FOOTER_Y = WINDOW_HEIGHT - 80.f;
    constexpr float CONTENT_CENTER_Y = WINDOW_HEIGHT / 2.f;
    constexpr float BUTTON_SPACING = 140.f;
    constexpr int MAX_LEVELS = 8;
}

#endif // DISPLAYCONFIG_HPP_
