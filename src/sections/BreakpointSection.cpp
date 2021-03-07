//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/sections/BreakpointSection.hpp"

#include <string>
#include <sstream>

using namespace Layout;

void BreakpointSection::redraw(const Chip8State &state)
{
    drawSectionBox();
    renderBreakpoints(state);
}

void BreakpointSection::drawSectionBox() const
{
    renderManager->render(SectionBoxWidget{BreakpointBox::xPos, BreakpointBox::yPos,
                                          BreakpointBox::width, BreakpointBox::height});
}

void BreakpointSection::renderBreakpoints(const Chip8State &state) const
{
    const auto kStartLine = 2;
    const auto kMaxBreakpointsShown = 14;

    auto padding = Box::outlineThickness + Box::padding;
    auto xPos = BreakpointBox::xPos + padding;
    auto yPos = BreakpointBox::yPos + padding;

    renderManager->render(TextWidget{xPos, yPos, "Break:"});
    
    for (int i = 0; i < state.breakpoints.size() && i < kMaxBreakpointsShown; i++)
    {
        std::stringstream stream;
        stream << std::hex << "-" << state.breakpoints.at(i);
        renderManager->render(TextWidget{xPos,
                                         yPos + (i + kStartLine) * Char::lineHeight,
                                         stream.str()});
    }
}