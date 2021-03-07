//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_CHIP8_HPP
#define CHIP8_CHIP8_HPP

#include "Game.hpp"

#include <array>
#include <vector>
#include <chrono>
#include <string>
#include <memory>
#include <cstdint>

struct Chip8State
{
    const uint16_t kStartAddress{0x200};
    static constexpr uint8_t kVerticalRes{32};
    static constexpr uint8_t kHorizontalRes{64};

    // Registers
    uint16_t I{0};
    uint8_t delayTimer{0};
    uint8_t soundTimer{0};
    uint8_t stackPointer{0};
    std::array<uint8_t, 16> V{};
    uint16_t instructionPointer{kStartAddress};

    // Memory
    std::array<bool, 16> keypad{};
    std::array<uint16_t, 16> stack{};
    std::array<uint8_t, 4096> memory{};
    std::array<bool, kHorizontalRes * kVerticalRes> display{};

    // Current game
    std::unique_ptr<Game> game;

    bool isRunning{false};
    uint16_t instructionsPerSecond{500};
    std::vector<uint16_t> breakpoints;
    std::vector<std::string> disassembly;
};

class Chip8
{
public:
    const Chip8State &getState();
    void start();
    void stop();
    void catchUp();
    int executeMs(int ms);
    void emulateCycle();
    bool loadGame(const std::string &gamePath);
    void setButton(bool pressed, int index);
    void updateTimers();
    void increaseSpeed();
    void decreaseSpeed();
    void toggleBreakpoint();

private:
    Chip8State state{};
    uint16_t opcode{0};
    const uint8_t kMinSpeed{100};
    const uint8_t kSpeedStepSize{100};
    uint64_t instructionsExecuted{0};
    std::chrono::time_point<std::chrono::steady_clock> startTime{};
    const uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    void initialize();
    void resetTime();
    void disassembleInstructions();
    std::string disassemble(uint16_t address);
    
    // Opcode methodes
    void CPU_00E0();
    void CPU_00EE();
    void CPU_1NNN();
    void CPU_2NNN();
    void CPU_3XNN();
    void CPU_4XNN();
    void CPU_5XY0();
    void CPU_6XNN();
    void CPU_7XNN();
    void CPU_8XY0();
    void CPU_8XY1();
    void CPU_8XY2();
    void CPU_8XY3();
    void CPU_8XY4();
    void CPU_8XY5();
    void CPU_8XY6();
    void CPU_8XY7();
    void CPU_8XYE();
    void CPU_9XY0();
    void CPU_ANNN();
    void CPU_BNNN();
    void CPU_CXNN();
    void CPU_DXYN();
    void CPU_EX9E();
    void CPU_EXA1();
    void CPU_FX07();
    void CPU_FX0A();
    void CPU_FX15();
    void CPU_FX18();
    void CPU_FX1E();
    void CPU_FX29();
    void CPU_FX33();
    void CPU_FX55();
    void CPU_FX65();
};

#endif