//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_DISPLAYSECTION_HPP
#define CHIP8_DISPLAYSECTION_HPP

#include "chip8/sections/ISection.hpp"

class DisplaySection : public ISection
{
public:
    explicit DisplaySection(std::shared_ptr<RenderManager> renderManager) : ISection(std::move(renderManager)){};

    // DisplaySection should be unique and local -- no copy/move operators
    DisplaySection(const DisplaySection &) = delete;
    DisplaySection &operator=(const DisplaySection &) = delete;
    DisplaySection(DisplaySection &&) noexcept = delete;
    DisplaySection &operator=(DisplaySection &&) noexcept = delete;

    void redraw(const Chip8State &state) override;

private:
    void drawSectionBox() const override;
    void renderDisplay(const Chip8State &state) const;
};

#endif