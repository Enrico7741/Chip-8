//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/sections/CodeSection.hpp"

#include <algorithm>

using namespace Layout;

void CodeSection::redraw(const Chip8State &state)
{
    drawSectionBox();
    renderCode(state);
}

void CodeSection::drawSectionBox() const
{
    renderManager->render(SectionBoxWidget{CodeBox::xPos, CodeBox::yPos, 
                                          CodeBox::width, CodeBox::height});
}

void CodeSection::renderCode(const Chip8State &state)
{
    // Aliases
    const auto &instructionPointer = state.instructionPointer;
    const auto &disassembly = state.disassembly;
    const auto &startAddress = state.kStartAddress;
    const auto &breakpoints = state.breakpoints;

    const size_t maxLines = 25;
    const auto instructionSize = 2;

    // Check whether code window should be moved (and if so, adapt bottom and top addresses)
    if (instructionPointer < topInstruction || instructionPointer > bottomInstruction)
    {
        topInstruction = instructionPointer;
        bottomInstruction = topInstruction + (maxLines - 1) * instructionSize;
    }

    // Index of topInstruction in disassembly vector
    const auto instructionIndex = std::max((topInstruction - startAddress) / instructionSize, 0);

    // Determine if we can print maxLines or less
    const auto numCodeLines = std::min((disassembly.size() - instructionIndex), maxLines);

    for (int i = 0; i < numCodeLines; ++i)
    {
        auto address = topInstruction + i * instructionSize;
        auto yShift = i * Char::lineHeight;

        // Box the currently executed instruction with a red outline
        if (address == instructionPointer)
        {
            renderManager->render(OutlineWidget{CodeBox::outlineX, CodeBox::outlineY + yShift,
                                                CodeBox::outlineWidth, CodeBox::outlineHeight});
        }

        auto padding = Box::outlineThickness + Box::padding;
        auto printRed = std::count(breakpoints.begin(), breakpoints.end(), address) > 0;

        renderManager->render(TextWidget{CodeBox::xPos + padding, CodeBox::yPos + yShift + padding,
                                         disassembly.at(instructionIndex + i), printRed});
    }
}