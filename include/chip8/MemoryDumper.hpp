//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_MEMORYDUMPER_HPP
#define CHIP8_MEMORYDUMPER_HPP

#include <array>
#include <cstdint>

class MemoryDumper
{
public:
    MemoryDumper(){};
    void dumpMemory(const std::array<uint8_t, 4096> &memory);
};

#endif