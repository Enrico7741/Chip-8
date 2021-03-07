//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_CODESECTION_HPP
#define CHIP8_CODESECTION_HPP

#include "chip8/sections/ISection.hpp"

class CodeSection : public ISection
{
public:
    explicit CodeSection(std::shared_ptr<RenderManager> renderManager) : ISection(std::move(renderManager)){};

    // CodeSection should be unique and local -- no copy/move operators
    CodeSection(const CodeSection &) = delete;
    CodeSection &operator=(const CodeSection &) = delete;
    CodeSection(CodeSection &&) noexcept = delete;
    CodeSection &operator=(CodeSection &&) noexcept = delete;

    void redraw(const Chip8State &state) override;

private:
    void drawSectionBox() const override;
    void renderCode(const Chip8State &state);
    
    // Address of the first and last displayed instruction in section
    int topInstruction{0};
    int bottomInstruction{0};
};

#endif