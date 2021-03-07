//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_BREAKPOINTSECTION_HPP
#define CHIP8_BREAKPOINTSECTION_HPP

#include "chip8/sections/ISection.hpp"

class BreakpointSection : public ISection
{
public:
    explicit BreakpointSection(std::shared_ptr<RenderManager> renderManager) : ISection(std::move(renderManager)){};

    // BreakpointSection should be unique and local -- no copy/move operators
    BreakpointSection(const BreakpointSection &) = delete;
    BreakpointSection &operator=(const BreakpointSection &) = delete;
    BreakpointSection(BreakpointSection &&) noexcept = delete;
    BreakpointSection &operator=(BreakpointSection &&) noexcept = delete;

    void redraw(const Chip8State &state) override;

private:
    void drawSectionBox() const override;
    void renderBreakpoints(const Chip8State &state) const;
};

#endif