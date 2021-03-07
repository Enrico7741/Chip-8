//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_LAYOUT_HPP
#define CHIP8_LAYOUT_HPP

#include <vector>
#include <utility>
#include <cstdint>

/**
 * Namespace for global constants concerning the layout.
 * Constants defined here shall be used throughout all layout relevant code.
 */
namespace Layout
{
    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    namespace Colors
    {
        const Color text{0, 0, 0};
        const Color boxDark{5, 10, 15};
        const Color boxBright{60, 80, 90};
        const Color background{25, 35, 45};
        const Color breakpoint{150, 0, 0};
        const Color outlineCode{255, 0, 0};
        const Color displayBackground{40, 40, 40};
        const Color displayForeground{170, 255, 170};
    }

    namespace Box
    {
        const int margin{11};          // Space between each box
        const int padding{5};          // Padding between boundary and content
        const int outlineThickness{4}; // Thickness of box outline
    }

    namespace Char
    {
        const int width{10};
        const int height{14};
        const int margin{4};              // Space between chars
        const int bmpStart{0};            // Start of first char in bitmap
        const int bmpMargin{2};           // Space between two chars in bitmap
        const int asciiOffset{33};        // Ascii value of first char in bitmap
        const int lineHeight{height + 5}; // Y-distance between start of two lines
    }

    namespace Button
    {
        const int width{32};
        const int height{36};
        const int bmpMargin{1}; // Space between two buttons in bitmap
    }

    namespace MainWindow
    {
        const int width{1351};
        const int firstRowHeight{488};  // Heigth of sections in first row of the window
        const int secondRowHeight{317}; // Heigth of sections in second row of the window
        const int height{firstRowHeight + secondRowHeight + 3 * Box::margin};
    }

    namespace DisplayBox
    {
        const int xPos{Box::margin};
        const int yPos{Box::margin};
        const int width{968};
        const int height{MainWindow::firstRowHeight};
        const int scale{15};
        const int displayX{xPos + Box::outlineThickness}; // X-position of display in box
        const int displayY{yPos + Box::outlineThickness}; // Y-position of display in box
    }

    namespace InfoBox
    {
        const int xPos{Box::margin};
        const int yPos{2 * Box::margin + DisplayBox::height};
        const int width{350};
        const int height{MainWindow::secondRowHeight};
    }

    namespace InputBox
    {
        const int xPos{2 * Box::margin + InfoBox::width};
        const int yPos{2 * Box::margin + DisplayBox::height};
        const int width{236};
        const int height{MainWindow::secondRowHeight};

        const int stepSizeX = 52;                      // Distance between button columns in x
        const int firstCol = xPos + Box::padding + 20; // Start of first button column in x
        const int secondCol = firstCol + stepSizeX;
        const int thirdCol = secondCol + stepSizeX;
        const int fourthCol = thirdCol + stepSizeX;

        const int stepSizeY = 69;                      // Distance between button rows in y
        const int firstRow = yPos + Box::padding + 33; // Start of first button row in y
        const int secondRow = firstRow + stepSizeY;
        const int thirdRow = secondRow + stepSizeY;
        const int fourthRow = thirdRow + stepSizeY;

        const std::vector<std::pair<int, int>> buttonPos{
            {Layout::InputBox::firstCol, Layout::InputBox::firstRow},    // 1
            {Layout::InputBox::secondCol, Layout::InputBox::firstRow},   // 2
            {Layout::InputBox::thirdCol, Layout::InputBox::firstRow},    // 3
            {Layout::InputBox::fourthCol, Layout::InputBox::firstRow},   // 4

            {Layout::InputBox::firstCol, Layout::InputBox::secondRow},   // Q
            {Layout::InputBox::secondCol, Layout::InputBox::secondRow},  // W
            {Layout::InputBox::thirdCol, Layout::InputBox::secondRow},   // E
            {Layout::InputBox::fourthCol, Layout::InputBox::secondRow},  // R

            {Layout::InputBox::firstCol, Layout::InputBox::thirdRow},    // A
            {Layout::InputBox::secondCol, Layout::InputBox::thirdRow},   // S
            {Layout::InputBox::thirdCol, Layout::InputBox::thirdRow},    // D
            {Layout::InputBox::fourthCol, Layout::InputBox::thirdRow},   // F

            {Layout::InputBox::firstCol, Layout::InputBox::fourthRow},   // Z
            {Layout::InputBox::secondCol, Layout::InputBox::fourthRow},  // X
            {Layout::InputBox::thirdCol, Layout::InputBox::fourthRow},   // C
            {Layout::InputBox::fourthCol, Layout::InputBox::fourthRow}}; // V
    }

    namespace BreakpointBox
    {
        const int xPos{3 * Box::margin + InfoBox::width + InputBox::width};
        const int yPos{2 * Box::margin + DisplayBox::height};
        const int width{125};
        const int height{MainWindow::secondRowHeight};
    }

    namespace StackBox
    {
        const int xPos{4 * Box::margin + InfoBox::width + InputBox::width + BreakpointBox::width};
        const int yPos{2 * Box::margin + DisplayBox::height};
        const int width{224};
        const int height{MainWindow::secondRowHeight};

        // Outline for current stack level. Complicated...
        const int outlineX{xPos + Box::outlineThickness + 3};
        const int outlineY{yPos + height - Box::outlineThickness - Box::padding - Char::height - 2};
        const int outlineWidth{width - 2 * (Box::outlineThickness + 3)};
        const int outlineHeight{Char::height + 4};
    }

    namespace RegisterBox
    {
        const int xPos{2 * Box::margin + DisplayBox::width};
        const int yPos{2 * Box::margin + DisplayBox::height};
        const int width{350};
        const int height{MainWindow::secondRowHeight};
        const int colWidth{197}; // Distance in x to second column
    }

    namespace CodeBox
    {
        const int xPos{2 * Box::margin + DisplayBox::width};
        const int yPos{Box::margin};
        const int width{350};
        const int height{MainWindow::firstRowHeight};

        // Outline for current instruction. Also complicated...
        const int outlineX{xPos + Box::outlineThickness + 3};
        const int outlineY{yPos + Box::outlineThickness + 3};
        const int outlineWidth{width - 2 * (Box::outlineThickness + 3)};
        const int outlineHeight{Char::height + 4};

        // Breakpoint background width
        const int bpLineWidth{width - 2 * (Box::outlineThickness + Box::padding)};
    }
}

#endif