//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_STACKSECTION_HPP
#define CHIP8_STACKSECTION_HPP

#include "chip8/sections/ISection.hpp"

class StackSection : public ISection
{
public:
    explicit StackSection(std::shared_ptr<RenderManager> renderManager) : ISection(std::move(renderManager)){};

    // StackSection should be unique and local -- no copy/move operators
    StackSection(const StackSection &) = delete;
    StackSection &operator=(const StackSection &) = delete;
    StackSection(StackSection &&) noexcept = delete;
    StackSection &operator=(StackSection &&) noexcept = delete;

    void redraw(const Chip8State &state) override;

private:
    void drawSectionBox() const override;
    void renderStack(const Chip8State &state) const;
};

#endif