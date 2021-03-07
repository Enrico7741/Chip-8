//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_BUTTONS_HPP
#define CHIP8_BUTTONS_HPP

#include <SDL.h>
#include <array>
#include <utility>

/**
 * Chip8 had its own keypad which we need to emulate with a modern keyboard.
 * The mapping is done as follows:
 * 
 * Keyboard   -->  Chip8 Keypad
 * 1 2 3 4         1 2 3 C
 * Q W E R    -->  4 5 6 D
 * A S D F         7 8 9 E
 * Z X C V         A 0 B F
 * 
 * Where the Chip8 keys are interpreted as hex values (which are used as
 * indices for the key values in Chip8State::keypad)
 */
const std::array<std::pair<SDL_Keycode, int>, 16> keyMap{{{SDLK_1, 0x1},
                                                          {SDLK_2, 0x2},
                                                          {SDLK_3, 0x3},
                                                          {SDLK_4, 0xC},
                                                          {SDLK_q, 0x4},
                                                          {SDLK_w, 0x5},
                                                          {SDLK_e, 0x6},
                                                          {SDLK_r, 0xD},
                                                          {SDLK_a, 0x7},
                                                          {SDLK_s, 0x8},
                                                          {SDLK_d, 0x9},
                                                          {SDLK_f, 0xE},
                                                          {SDLK_z, 0xA},
                                                          {SDLK_x, 0x0},
                                                          {SDLK_c, 0xB},
                                                          {SDLK_v, 0xF}}};

#endif