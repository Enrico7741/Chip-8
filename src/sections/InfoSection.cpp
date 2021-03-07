//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/sections/InfoSection.hpp"

using namespace Layout;

void InfoSection::redraw(const Chip8State &state)
{
    drawSectionBox();
    renderInfo(state);
}

void InfoSection::drawSectionBox() const
{
    renderManager->render(SectionBoxWidget{InfoBox::xPos, InfoBox::yPos,
                                          InfoBox::width, InfoBox::height});
}

void InfoSection::renderInfo(const Chip8State &state)
{
    auto padding = Box::outlineThickness + Box::padding;
    auto xPos = InfoBox::xPos + padding;
    auto yPos = InfoBox::yPos + padding;

    // Update mutable table values
    infoTable[1] = state.isRunning ? "Status: Running" : "Status: Stopped";
    infoTable[2] = "Speed : " + std::to_string(state.instructionsPerSecond);
    infoTable[5] = "Name: " + state.game->name;
    infoTable[6] = "Size: " + std::to_string(state.game->size) + " bytes";

    for (const auto &[offset, text] : infoTable)
    {
        renderManager->render(TextWidget{xPos, yPos + offset * Char::lineHeight, text});
    }
}