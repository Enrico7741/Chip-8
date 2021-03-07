//--------------------------------------------------------------------------------------------------
// Cross-Platform Chip-8 Emulator
// Copyright (C) 2020 Enrico Schörnick
// Licensed under the MIT License
//--------------------------------------------------------------------------------------------------

#include "chip8/RenderManager.hpp"

using namespace Layout;

RenderManager::RenderManager(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    loadBitmap(&font, "data/bitmaps/font.bmp");
    loadBitmap(&buttons, "data/bitmaps/buttons.bmp");
    renderBackgroundColor();
}

RenderManager::~RenderManager()
{
    SDL_DestroyTexture(font);
    SDL_DestroyTexture(buttons);
}

void RenderManager::render(const TextWidget &widget) const
{
    // Place rectangle over first character of bmp to select it
    SDL_Rect charSelector{Char::bmpStart, Char::bmpStart, Char::width, Char::height};
    SDL_Rect charDestination{widget.xPos, widget.yPos, Char::width, Char::height};

    if (widget.highlighted)
    {
        // Initialize highlight line for currently executed instruction
        SDL_Rect background{widget.xPos, widget.yPos, CodeBox::bpLineWidth, Char::height};
        setColor(Colors::breakpoint);
        SDL_RenderFillRect(renderer, &background);
    }

    setColor(Colors::text);
    for (const auto &character : widget.text)
    {
        // Copy character from the font bmp to the frame
        charSelector.x = (character - Char::asciiOffset) * (Char::width + Char::bmpMargin);
        SDL_RenderCopy(renderer, font, &charSelector, &charDestination);
        charDestination.x += (Char::width + Char::margin);
    }
}

void RenderManager::render(const PixelWidget &widget) const
{
    setColor(widget.set ? Colors::displayForeground : Colors::displayBackground);
    SDL_Rect scaledPixel = {widget.xPos, widget.yPos, widget.scale, widget.scale};
    SDL_RenderFillRect(renderer, &scaledPixel);
}

void RenderManager::render(const ButtonWidget &widget) const
{
    // Determine the correct button index 
    auto bmpButtonIndex = widget.bitmapIndex * 2 + widget.pressed;

    // Place rectangle over button of bmp to select it
    SDL_Rect buttonSelector{bmpButtonIndex * (Button::width + Button::bmpMargin),
                            0, Button::width, Button::height};

    // Copy button from the button bmp to the frame
    SDL_Rect buttonDestination{widget.xPos, widget.yPos, Button::width, Button::height};
    setColor(widget.pressed ? Colors::outlineCode : Colors::text);
    SDL_RenderCopy(renderer, buttons, &buttonSelector, &buttonDestination);
}

void RenderManager::render(const OutlineWidget &widget) const
{
    setColor(Colors::outlineCode);
    SDL_Rect outline = {widget.xPos, widget.yPos, widget.width, widget.height};
    SDL_RenderDrawRect(renderer, &outline);
}

void RenderManager::render(const SectionBoxWidget &widget) const
{
    // Drawing the dark lines around section
    setColor(Colors::boxDark);
    SDL_Rect horizontalRect{widget.xPos, widget.yPos, widget.width, Box::outlineThickness};
    SDL_Rect verticalRect{widget.xPos, widget.yPos, Box::outlineThickness, widget.height};
    SDL_RenderFillRect(renderer, &horizontalRect);
    SDL_RenderFillRect(renderer, &verticalRect);

    // Drawing the bright lines around section
    setColor(Colors::boxBright);
    horizontalRect.y += widget.height - Box::outlineThickness;
    verticalRect.x += widget.width - Box::outlineThickness;
    SDL_RenderFillRect(renderer, &horizontalRect);
    SDL_RenderFillRect(renderer, &verticalRect);
}

void RenderManager::updateScreen()
{
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
    renderBackgroundColor();
}

void RenderManager::setColor(Color color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
}

void RenderManager::renderBackgroundColor()
{
    setColor(Colors::background);
    SDL_RenderFillRect(renderer, nullptr);
}

void RenderManager::loadBitmap(SDL_Texture **texture, const std::string &path)
{
    auto surface = SDL_LoadBMP(path.c_str());
    if (surface == nullptr)
    {
        std::cout << "Error: SDL_LoadBMP: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Use a mask to get the elements with a transparent background
        auto mask = SDL_MapRGB(surface->format, 255, 0, 255);
        SDL_SetColorKey(surface, SDL_TRUE, mask);

        // Create a texture we can use to easily copy from
        *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}