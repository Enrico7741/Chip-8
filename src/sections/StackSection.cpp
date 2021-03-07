//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/Utils.hpp"
#include "chip8/Layout.hpp"
#include "chip8/sections/StackSection.hpp"

#include <string>
#include <sstream>

using namespace Layout;

void StackSection::redraw(const Chip8State &state)
{
    drawSectionBox();
    renderStack(state);
}

void StackSection::drawSectionBox() const
{
    renderManager->render(SectionBoxWidget{StackBox::xPos, StackBox::yPos,
                                          StackBox::width, StackBox::height});
}

void StackSection::renderStack(const Chip8State &state) const
{
    auto padding = Box::outlineThickness + Box::padding;
    auto xPos = StackBox::xPos + padding;
    auto yPos = StackBox::yPos + StackBox::height - padding - Char::height;
    
    for (int i = 0; i < state.stack.size(); i++)
    {
        std::stringstream stream;
        stream << "Stack " << std::hex << i << " = #" << getValueString(state.stack[i]);

        // Render additional outline to indicate the current stack level
        if (state.stackPointer == i)
        {
            renderManager->render(OutlineWidget{StackBox::outlineX,
                                                StackBox::outlineY - i * Char::lineHeight,
                                                StackBox::outlineWidth,
                                                StackBox::outlineHeight});
        }

        renderManager->render(TextWidget{xPos, yPos - i * Char::lineHeight, stream.str()});
    }
}