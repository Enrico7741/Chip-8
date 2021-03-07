//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_INPUTSECTION_HPP
#define CHIP8_INPUTSECTION_HPP

#include "chip8/sections/ISection.hpp"

class InputSection : public ISection
{
public:
    explicit InputSection(std::shared_ptr<RenderManager> renderManager) : ISection(std::move(renderManager)){};

    // InputSection should be unique and local -- no copy/move operators
    InputSection(const InputSection &) = delete;
    InputSection &operator=(const InputSection &) = delete;
    InputSection(InputSection &&) noexcept = delete;
    InputSection &operator=(InputSection &&) noexcept = delete;

    void redraw(const Chip8State &state) override;

private:
    void drawSectionBox() const override;
    void renderButtons(const Chip8State &state) const;
};

#endif