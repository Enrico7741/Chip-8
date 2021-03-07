//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/Utils.hpp"
#include "chip8/sections/RegisterSection.hpp"

#include <string>
#include <sstream>

using namespace Layout;

void RegisterSection::redraw(const Chip8State &state)
{
    drawSectionBox();
    renderRegisters(state);
}

void RegisterSection::drawSectionBox() const
{
    renderManager->render(SectionBoxWidget{RegisterBox::xPos, RegisterBox::yPos,
                                          RegisterBox::width, RegisterBox::height});
}

void RegisterSection::renderRegisters(const Chip8State &state) const
{
    auto padding = Box::outlineThickness + Box::padding;
    auto xPos = RegisterBox::xPos + padding;
    auto yPos = RegisterBox::yPos + padding;

    // Render V registers
    for (int i = 0; i < state.V.size(); i++)
    {
        std::stringstream stream;
        stream << "V" << std::hex << i << " = #" << getValueString(state.V[i]);
        renderManager->render(TextWidget{xPos, yPos + i * Char::lineHeight, stream.str()});
    }

    // Render remaining registers
    xPos += RegisterBox::colWidth;
    renderManager->render(TextWidget{xPos, yPos,
                                     " I = #" + getValueString(state.I)});
    renderManager->render(TextWidget{xPos, yPos + 2 * Char::lineHeight,
                                     "IP = #" + getValueString(state.instructionPointer)});
    renderManager->render(TextWidget{xPos, yPos + 3 * Char::lineHeight,
                                     "SP = #" + getValueString(state.stackPointer)});
    renderManager->render(TextWidget{xPos, yPos + 5 * Char::lineHeight,
                                     "DT = #" + getValueString(state.delayTimer)});
    renderManager->render(TextWidget{xPos, yPos + 6 * Char::lineHeight,
                                     "ST = #" + getValueString(state.soundTimer)});
}
