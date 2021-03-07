//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/sections/DisplaySection.hpp"

using namespace Layout;

void DisplaySection::redraw(const Chip8State &state)
{
    drawSectionBox();
    renderDisplay(state);
}

void DisplaySection::drawSectionBox() const
{
    renderManager->render(SectionBoxWidget{DisplayBox::xPos, DisplayBox::yPos,
                                          DisplayBox::width, DisplayBox::height});
}

void DisplaySection::renderDisplay(const Chip8State &state) const
{
    for (int y = 0; y < state.kVerticalRes; y++)
    {
        for (int x = 0; x < state.kHorizontalRes; x++)
        {
            renderManager->render(PixelWidget{x * DisplayBox::scale + DisplayBox::displayX,
                                              y * DisplayBox::scale + DisplayBox::displayY,
                                              DisplayBox::scale,
                                              state.display[(y * state.kHorizontalRes) + x]});
        }
    }
}