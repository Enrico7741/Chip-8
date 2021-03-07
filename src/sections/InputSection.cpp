//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/Buttons.hpp"
#include "chip8/sections/InputSection.hpp"

using namespace Layout;

void InputSection::redraw(const Chip8State &state)
{
    drawSectionBox();
    renderButtons(state);
}

void InputSection::drawSectionBox() const
{
    renderManager->render(SectionBoxWidget{InputBox::xPos, InputBox::yPos,
                                          InputBox::width, InputBox::height});
}

void InputSection::renderButtons(const Chip8State &state) const
{
    for (int i = 0; i < keyMap.size(); i++)
    {
        renderManager->render(ButtonWidget{InputBox::buttonPos[i].first,
                                           InputBox::buttonPos[i].second,
                                           i,
                                           state.keypad[keyMap[i].second]});
    }
}