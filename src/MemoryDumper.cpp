//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/MemoryDumper.hpp"

#include <fstream>
#include <iostream>

void MemoryDumper::dumpMemory(const std::array<uint8_t, 4096> &memory)
{
    std::fstream dumpFile("memoryDump.bin", std::ios::out | std::ios::binary);
    if (dumpFile.is_open())
    {
        dumpFile.write((char *)memory.data(), memory.size());
        dumpFile.close();
        std::cout << "Info: Current memory dumped to memoryDump.bin" << std::endl;
    }
    else
    {
        std::cout << "Error: Couldn't open memoryDump.bin" << std::endl;
    }
}