//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_INFOSECTION_HPP
#define CHIP8_INFOSECTION_HPP

#include "chip8/sections/ISection.hpp"

#include <map>
#include <string>

class InfoSection : public ISection
{
public:
    explicit InfoSection(std::shared_ptr<RenderManager> renderManager) : ISection(std::move(renderManager)){};

    // InfoSection should be unique and local -- no copy/move operators
    InfoSection(const InfoSection &) = delete;
    InfoSection &operator=(const InfoSection &) = delete;
    InfoSection(InfoSection &&) noexcept = delete;
    InfoSection &operator=(InfoSection &&) noexcept = delete;

    void redraw(const Chip8State &state) override;

private:
    void drawSectionBox() const override;
    void renderInfo(const Chip8State &state);

    std::map<int, std::string> infoTable{{0, "Emulator"},
                                         {1, "Status:"},
                                         {2, "Speed :"},
                                         {4, "Game"},
                                         {5, "Name:"},
                                         {6, "Size:"},
                                         {8, "Controls"},
                                         {9, "F1 : Start/Stop"},
                                         {10, "F2 : Step"},
                                         {11, "F3 : Breakpoint"},
                                         {12, "F4 : Warp Mode"},
                                         {13, "F5 : Dump Memory"},
                                         {14, "F6 : Reset"},
                                         {15, "+/-: Speed"}};
};

#endif