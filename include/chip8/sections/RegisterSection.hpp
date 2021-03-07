//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_REGISTERSECTION_HPP
#define CHIP8_REGISTERSECTION_HPP

#include "chip8/sections/ISection.hpp"

class RegisterSection : public ISection
{
public:
    explicit RegisterSection(std::shared_ptr<RenderManager> renderManager) : ISection(std::move(renderManager)){};

    // RegisterSection should be unique and local -- no copy/move operators
    RegisterSection(const RegisterSection &) = delete;
    RegisterSection &operator=(const RegisterSection &) = delete;
    RegisterSection(RegisterSection &&) noexcept = delete;
    RegisterSection &operator=(RegisterSection &&) noexcept = delete;

    void redraw(const Chip8State &state) override;

private:
    void drawSectionBox() const override;
    void renderRegisters(const Chip8State &state) const;
};

#endif