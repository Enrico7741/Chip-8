//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#ifndef CHIP8_RENDERMANAGER_HPP
#define CHIP8_RENDERMANAGER_HPP

#include "Layout.hpp"

#include <SDL.h>
#include <iostream>

// Predefined elements which can be drawn
struct TextWidget
{
    int xPos;
    int yPos;
    const std::string &text;
    bool highlighted{false};
};

struct PixelWidget
{
    int xPos;
    int yPos;
    int scale;
    bool set{false};
};

struct ButtonWidget
{
    int xPos;
    int yPos;
    int bitmapIndex;
    bool pressed{false};
};

struct OutlineWidget
{
    int xPos;
    int yPos;
    int width;
    int height;
};

struct SectionBoxWidget
{
    int xPos;
    int yPos;
    int width;
    int height;
};

class RenderManager
{
public:
    RenderManager(SDL_Renderer *renderer);
    ~RenderManager();

    // Overloaded drawing methodes
    void render(const TextWidget &widget) const;
    void render(const PixelWidget &widget) const;
    void render(const ButtonWidget &widget) const;
    void render(const OutlineWidget &widget) const;
    void render(const SectionBoxWidget &widget) const;
    void updateScreen();

private:
    SDL_Renderer *renderer;
    SDL_Texture *font;
    SDL_Texture *buttons;
    
    void setColor(Layout::Color color) const;
    void renderBackgroundColor();
    void loadBitmap(SDL_Texture **texture, const std::string &path);
};

#endif