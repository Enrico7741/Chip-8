//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_SECTIONS_ISECTION_HPP
#define CHIP8_SECTIONS_ISECTION_HPP

#include "chip8/Chip8.hpp"
#include "chip8/RenderManager.hpp"

#include <memory>

/**
 * Interface for Sections.
 * A section constitutes an isolated region of the user interface.
 * Sections must be able to draw themselves via choosing the relevant state from
 * the Chip8 state itself and then calling methods on the RenderManager
 */
class ISection
{
public:
    // Virtual destructor
    virtual ~ISection() = default;

    // Use the renderManager to redraw the section with a new state
    virtual void redraw(const Chip8State &state) = 0;

protected:
    /**
     * Constructors and assignment operators
     * Protected to allow usage only in inheriting classes
     */
    explicit ISection(std::shared_ptr<RenderManager> renderManager) : renderManager{std::move(renderManager)}{};
    ISection() = default;
    ISection(const ISection &) = default;
    ISection &operator=(const ISection &) = default;
    ISection(ISection &&) noexcept = default;
    ISection &operator=(ISection &&) noexcept = default;

    // Manager to use for rendering on screen
    std::shared_ptr<RenderManager> renderManager{};

    // Every section has to draw a box around itself
    virtual void drawSectionBox() const = 0;
};

#endif