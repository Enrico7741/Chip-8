//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/Chip8.hpp"

#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

// Defines that simplifiy opcode and register handling
#define N (opcode & 0x000F)
#define NN (opcode & 0x00FF)
#define NNN (opcode & 0x0FFF)
#define X ((opcode & 0x0F00) >> 8)
#define Y ((opcode & 0x00F0) >> 4)
#define VX state.V[(opcode & 0x0F00) >> 8]
#define VY state.V[(opcode & 0x00F0) >> 4]

bool Chip8::loadGame(const std::string &gamePath)
{
    using namespace std::filesystem;
    
    if (!exists(gamePath))
    {
        std::cout << "Error: Game file doesn't exist" << std::endl;
        return false;
    }

    if (path(gamePath).extension().compare(".ch8") != 0)
    {
        std::cout << "Error: Only files with the .ch8 extension can be loaded" << std::endl;
        return false;
    }
    
    if (file_size(gamePath) > state.memory.size() - state.kStartAddress)
    {
        std::cout << "Error: Game file is to big" << std::endl;
        return false;
    }

    initialize();

    // Create game and load it into memory
    state.game = std::make_unique<Game>(path(gamePath).filename().string(), gamePath, file_size(gamePath));
    std::fstream gameStream(state.game->path, std::ios::in | std::ios::binary);
    gameStream.read(reinterpret_cast<char*>(state.memory.data()) + state.kStartAddress, state.game->size);

    state.instructionsPerSecond = state.game->getBestSpeed();

    disassembleInstructions();

    return true;
}

void Chip8::initialize()
{
    // Reinitialize everything before a new game gets loaded
    opcode = 0;

    state.I = 0;
    state.delayTimer = 0;
    state.soundTimer = 0;
    state.stackPointer = 0;
    state.V.fill(0);
    state.instructionPointer = state.kStartAddress;

    state.keypad.fill(false);
    state.stack.fill(0);
    state.display.fill(false);
    state.memory.fill(0);

    // Copy fontset to memory location 0x0000
    std::copy(fontset, fontset + sizeof(fontset), state.memory.data());

    state.breakpoints.clear();
    state.disassembly.clear();
    resetTime();

    // Setup the function pointer tables
    setupTables();
}

void Chip8::resetTime()
{
    instructionsExecuted = 0;
    startTime = std::chrono::steady_clock::now();
}

void Chip8::setupTables()
{
    /**
     * Set every function pointer to CPU_NOP to prevent
     * crashes because of invalid opcodes
     */
    for (int i = 0; i < kSizeTable; i++)
    {
        cpu[i] = &Chip8::CPU_NOP;
    }

    for (int i = 0; i < kSizeTable0; i++)
    {
        cpu0[i] = &Chip8::CPU_NOP;
    }

    for (int i = 0; i < kSizeTable8; i++)
    {
        cpu8[i] = &Chip8::CPU_NOP;
    }

    for (int i = 0; i < kSizeTableE; i++)
    {
        cpuE[i] = &Chip8::CPU_NOP;
    }

    for (int i = 0; i < kSizeTableF; i++)
    {
        cpuF[i] = &Chip8::CPU_NOP;
    }
    
    // Fill tables with correct function pointers
    cpu[0x0] = &Chip8::jumpCpu0;
    cpu[0x1] = &Chip8::CPU_1NNN;
    cpu[0x2] = &Chip8::CPU_2NNN;
    cpu[0x3] = &Chip8::CPU_3XNN;
    cpu[0x4] = &Chip8::CPU_4XNN;
    cpu[0x5] = &Chip8::CPU_5XY0;
    cpu[0x6] = &Chip8::CPU_6XNN;
    cpu[0x7] = &Chip8::CPU_7XNN;
    cpu[0x8] = &Chip8::jumpCpu8;
    cpu[0x9] = &Chip8::CPU_9XY0;
    cpu[0xA] = &Chip8::CPU_ANNN;
    cpu[0xB] = &Chip8::CPU_BNNN;
    cpu[0xC] = &Chip8::CPU_CXNN;
    cpu[0xD] = &Chip8::CPU_DXYN;
    cpu[0xE] = &Chip8::jumpCpuE;
    cpu[0xF] = &Chip8::jumpCpuF;

    cpu0[0x0] = &Chip8::CPU_00E0;
    cpu0[0xE] = &Chip8::CPU_00EE;

    cpu8[0x0] = &Chip8::CPU_8XY0;
    cpu8[0x1] = &Chip8::CPU_8XY1;
    cpu8[0x2] = &Chip8::CPU_8XY2;
    cpu8[0x3] = &Chip8::CPU_8XY3;
    cpu8[0x4] = &Chip8::CPU_8XY4;
    cpu8[0x5] = &Chip8::CPU_8XY5;
    cpu8[0x6] = &Chip8::CPU_8XY6;
    cpu8[0x7] = &Chip8::CPU_8XY7;
    cpu8[0xE] = &Chip8::CPU_8XYE;

    cpuE[0x1] = &Chip8::CPU_EXA1;
    cpuE[0xE] = &Chip8::CPU_EX9E;

    cpuF[0x07] = &Chip8::CPU_FX07;
    cpuF[0x0A] = &Chip8::CPU_FX0A;
    cpuF[0x15] = &Chip8::CPU_FX15;
    cpuF[0x18] = &Chip8::CPU_FX18;
    cpuF[0x1E] = &Chip8::CPU_FX1E;
    cpuF[0x29] = &Chip8::CPU_FX29;
    cpuF[0x33] = &Chip8::CPU_FX33;
    cpuF[0x55] = &Chip8::CPU_FX55;
    cpuF[0x65] = &Chip8::CPU_FX65;
}

void Chip8::disassembleInstructions()
{
    // Walk through memory and disassemble every instruction
    for (int i = state.kStartAddress; i < state.kStartAddress + state.game->size; i += sizeof(opcode))
    {
        state.disassembly.push_back(disassemble(i));
    }
}

std::string Chip8::disassemble(uint16_t address)
{
    uint16_t opcode = state.memory[address] << 8 | state.memory[address + 1];

    std::stringstream str;
    str << std::hex << address << " - ";

    // Create the string representation of the opcode
    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x000F) {
        case 0x0000: str << "CLS"; break;
        case 0x000E: str << "RET"; break;
        } break;
    case 0x1000: str << "JP    #" << NNN; break;
    case 0x2000: str << "CALL  #" << NNN; break;
    case 0x3000: str << "SE    V" << X << "   #" << NN; break;
    case 0x4000: str << "SNE   V" << X << "   #" << NN; break;
    case 0x5000: str << "SE    V" << X << "   V" << Y; break;
    case 0x6000: str << "LD    V" << X << "   #" << NN; break;
    case 0x7000: str << "ADD   V" << X << "   #" << NN; break;
    case 0x8000:
        switch (opcode & 0x000F) {
        case 0x0000: str << "LD    V" << X << "   V" << Y; break;
        case 0x0001: str << "OR    V" << X << "   V" << Y; break;
        case 0x0002: str << "AND   V" << X << "   V" << Y; break;
        case 0x0003: str << "XOR   V" << X << "   V" << Y; break;
        case 0x0004: str << "ADD   V" << X << "   V" << Y; break;
        case 0x0005: str << "SUB   V" << X << "   V" << Y; break;
        case 0x0006: str << "SHR   V" << X; break;
        case 0x0007: str << "SUBN  V" << X << "   V" << Y; break;
        case 0x000E: str << "SHL   V" << X; break;
        } break;
    case 0x9000: str << "SNE   V" << X << "   V" << Y; break;
    case 0xA000: str << "LD    I    #" << NNN; break;
    case 0xB000: str << "JP    V0   #" << NNN; break;
    case 0xC000: str << "RND   V" << X << "   #" << NN; break;
    case 0xD000: str << "DRW   V" << X << "   V" << Y << "   #" << N; break;
    case 0xE000:
        switch (opcode & 0x00FF) {
        case 0x009E: str << "SKP   V" << X; break;
        case 0x00A1: str << "SKNP  V" << X; break;
        } break;
    case 0xF000:
        switch (opcode & 0x00FF) {
        case 0x0007: str << "LD    V" << X << "   DT"; break;
        case 0x000A: str << "LD    V" << X << "   K"; break;
        case 0x0015: str << "LD    DT   V" << X; break;
        case 0x0018: str << "LD    ST   V" << X; break;
        case 0x001E: str << "ADD   I    V" << X; break;
        case 0x0029: str << "LD    F    V" << X; break;
        case 0x0033: str << "BCD   V" << X; break;
        case 0x0055: str << "LD    [I]   V" << X; break;
        case 0x0065: str << "LD    V" << X <<"   [I]"; break;
        } break;
    }

    return str.str();
}

const Chip8State& Chip8::getState()
{
    return state;
}

void Chip8::start()
{
    state.isRunning = true;
    resetTime();
}

void Chip8::stop()
{
    state.isRunning = false;
}

void Chip8::catchUp()
{
    using namespace std::chrono;

    // Theoretical time in microseconds for one instruction to hit our target speed
    auto instructionTime = 1000000 / state.instructionsPerSecond;

    // How many microseconds did pass since start of emulation?
    auto elapsed = duration_cast<microseconds>(steady_clock::now() - startTime).count();

    // Execute as many instructions as needed to be up to date
    auto instructionsShould = elapsed / instructionTime;
    while (instructionsShould > instructionsExecuted && state.isRunning)
    {
        emulateCycle();
        instructionsExecuted++;
    }
}

int Chip8::executeMs(int ms)
{
    using namespace std::chrono;

    startTime = steady_clock::now();
    auto instructionsExecuted = 0;

    // Execute as many instructions as possible in the given time
    while (ms > duration_cast<milliseconds>(steady_clock::now() - startTime).count())
    {
        emulateCycle();
        instructionsExecuted++;
    }

    return instructionsExecuted;
}

void Chip8::emulateCycle()
{
    opcode = state.memory[state.instructionPointer] << 8 | state.memory[state.instructionPointer + 1];

    // Immediately increase instruction pointer. Simplifies the emulation methodes.
    state.instructionPointer += sizeof(opcode);

    // Determine the right methode to call for the given opcode
    // Dereference our chip-8 object and get the function to call
    ((*this).*(cpu[(opcode & 0xF000) >> 12]))();

    // If the next instruction is a breakpoint we need to stop
    if (std::find(state.breakpoints.begin(), state.breakpoints.end(), 
                  state.instructionPointer) != state.breakpoints.end())
    {
        stop();
    }
}

void Chip8::jumpCpu0()
{
    ((*this).*(cpu0[opcode & 0x000F]))();
}

void Chip8::jumpCpu8()
{
    ((*this).*(cpu8[opcode & 0x000F]))();
}

void Chip8::jumpCpuE()
{
    ((*this).*(cpuE[opcode & 0x000F]))();
}

void Chip8::jumpCpuF()
{
    ((*this).*(cpuF[opcode & 0x00FF]))();
}

void Chip8::setButton(bool pressed, int index)
{
    state.keypad[index] = pressed;
}

void Chip8::updateTimers()
{
    if (state.isRunning)
    {
        if (state.delayTimer > 0)
        {
            state.delayTimer--;
        }

        if (state.soundTimer > 0)
        {
            state.soundTimer--;
        }
    }
}

void Chip8::increaseSpeed()
{
    state.instructionsPerSecond += kSpeedStepSize;
    resetTime();
}

void Chip8::decreaseSpeed()
{
    if (state.instructionsPerSecond > kMinSpeed)
    {
        state.instructionsPerSecond -= kSpeedStepSize;
        resetTime();
    }
}

void Chip8::toggleBreakpoint()
{
    stop();

    // Check if the next instruction is already in our breakpointlist
    if (std::find(state.breakpoints.begin(), state.breakpoints.end(), 
                  state.instructionPointer) == state.breakpoints.end())
    {
        state.breakpoints.push_back(state.instructionPointer);
    }
    else
    {
        // Remove our element (element gets shiftet to the end and after that erased)
        state.breakpoints.erase(std::remove(state.breakpoints.begin(), 
                                            state.breakpoints.end(), 
                                            state.instructionPointer), 
                                            state.breakpoints.end());
    }
}

void Chip8::CPU_NOP(){}

void Chip8::CPU_00E0()
{
    state.display.fill(false);
}

void Chip8::CPU_00EE()
{
    state.stackPointer--;
    state.instructionPointer = state.stack[state.stackPointer];
}

void Chip8::CPU_1NNN()
{
    state.instructionPointer = NNN;
}

void Chip8::CPU_2NNN()
{
    state.stack[state.stackPointer] = state.instructionPointer;
    state.stackPointer++;
    state.instructionPointer = NNN;
}

void Chip8::CPU_3XNN()
{
    state.instructionPointer += (VX == NN) ? sizeof(opcode) : 0;
}

void Chip8::CPU_4XNN()
{
    state.instructionPointer += (VX != NN) ? sizeof(opcode) : 0;
}

void Chip8::CPU_5XY0()
{
    state.instructionPointer += (VX == VY) ? sizeof(opcode) : 0;
}

void Chip8::CPU_6XNN()
{
    VX = NN;
}

void Chip8::CPU_7XNN()
{
    VX += NN;
}

void Chip8::CPU_8XY0()
{
    VX = VY;
}

void Chip8::CPU_8XY1()
{
    VX |= VY;
}

void Chip8::CPU_8XY2()
{
    VX &= VY;
}

void Chip8::CPU_8XY3()
{
    VX ^= VY;
}

void Chip8::CPU_8XY4()
{
    // First check if an overflow will occur
    state.V[0xF] = (VY > (0xFF - VX)) ? 1 : 0;
    VX += VY;
}

void Chip8::CPU_8XY5()
{
    // First check if an underflow will occur
    state.V[0xF] = (VY > VX) ? 0 : 1;
    VX -= VY;
}

void Chip8::CPU_8XY6()
{
    // Set VF to the lsb of VX
    state.V[0xF] = VX & 0x1;
    VX >>= 1;

    /**
     * Alternative implementation (used by some games)
     * state.V[0xF] = VY & 0x1;
     * VX = VY >> 1;
     */
}

void Chip8::CPU_8XY7()
{
    // First check if an underflow will occur
    state.V[0xF] = (VX > VY) ? 0 : 1;
    VX = VY - VX;
}

void Chip8::CPU_8XYE()
{
    // Set VF to the msb of VX
    state.V[0xF] = VX >> 7;
    VX <<= 1;

    /**
     * Alternative implementation (used by some games)
     * state.V[0xF] = VY >> 7;
     * VX = VY << 1;
     */
}

void Chip8::CPU_9XY0()
{
    state.instructionPointer += (VX != VY) ? sizeof(opcode) : 0;
}

void Chip8::CPU_ANNN()
{
    state.I = NNN;
}

void Chip8::CPU_BNNN()
{
    state.instructionPointer = NNN + state.V[0];
}

void Chip8::CPU_CXNN()
{
    // Set VX to a random number masked with NN
    VX = NN & (rand() % 0xFF);
}

void Chip8::CPU_DXYN()
{
    // Destination of sprite on display
    uint8_t xDest = VX; // Wrap around feature % state.kHorizontalRes;
    uint8_t yDest = VY; // Wrap around feature % state.kVerticalRes;

    // Width and hight of the sprite
    uint8_t spriteHeight = N;
    const uint8_t KSpriteWidth = 8;

    // Clean collision flag
    state.V[0xF] = 0;

    for (int y = 0; y < spriteHeight; y++)
    {
        // Get bits of the current sprite row
        uint8_t spriteRowBits = state.memory[state.I + y];

        for (int x = 0; x < KSpriteWidth; x++)
        {
            // Walk through the sprite row bits and check if one is set
            if ((spriteRowBits & (0x80 >> x)) != 0)
            {
                // Determine the position in the display array
                auto pixel = (((yDest + y) * state.kHorizontalRes) + xDest + x);

                if (state.display[pixel])
                {
                    // If ANY pixel gets changed from 1 to 0 we have a collision
                    state.V[0xF] |= 1;
                }

                // XOR display pixel with sprite bit
                state.display[pixel] ^= true;
            }
        }
    }
}

void Chip8::CPU_EX9E()
{
    state.instructionPointer += state.keypad[VX] ? sizeof(opcode) : 0;
}

void Chip8::CPU_EXA1()
{
    state.instructionPointer += !(state.keypad[VX]) ? sizeof(opcode) : 0;
}

void Chip8::CPU_FX07()
{
    VX = state.delayTimer;
}

void Chip8::CPU_FX0A()
{
    /**
     * Normally this is a blocking operation, but because we only work with
     * one thread the application would get unresponsive if we block here.
     * As a workaround the instruction pointer gets modified so we only 
     * move to the next opcode after a key has been pressed.
     */
    auto keyPressed = false;
    for (int i = 0; i < state.keypad.size(); i++)
    {
        if (state.keypad[i])
        {
            VX = i;
            keyPressed = true;
        }
    }

    if (!keyPressed)
    {
        state.instructionPointer -= sizeof(opcode);
    }
}

void Chip8::CPU_FX15()
{
    state.delayTimer = VX;
}

void Chip8::CPU_FX18()
{
    state.soundTimer = VX;
}

void Chip8::CPU_FX1E()
{
    // First check if I will be bigger than 0xFFF afterwards
    state.V[0xF] = (state.I + VX > 0xFFF) ? 1 : 0;
    state.I += VX;
}

void Chip8::CPU_FX29()
{
    // Set I to the address of the fontset sprite representing the VX value
    state.I = VX * 0x5;
}

void Chip8::CPU_FX33()
{
    // Store binary coded decimal of VX value in I, I+1, I+2
    state.memory[state.I] = VX / 100; 				// First digit of VX value
    state.memory[state.I + 1] = (VX / 10) % 10;		// Mid digit
    state.memory[state.I + 2] = (VX % 100) % 10;	// Last digit
}

void Chip8::CPU_FX55()
{
    // Store V0 to VX in memory starting at I
    memcpy(state.memory.data() + state.I, state.V.data(), X + 1);

    /**
     * Alternative implementation (used by some games)
     * state.I += X + 1;
     */
}

void Chip8::CPU_FX65()
{
    // Load V0 to VX with values in memory starting at I
    memcpy(state.V.data(), state.memory.data() + state.I, X + 1);

    /**
     * Alternative implementation (used by some games)
     * state.I += X + 1;
     */
}
