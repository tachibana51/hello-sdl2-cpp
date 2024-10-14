
#pragma once
#include "GUIElement.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>

class Button : public GUIElement {
public:
    Button(const std::string& text, int x, int y, int w, int h, std::function<void()> onClick);
    ~Button();

    void render(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event) override;

private:
    std::string text;
    SDL_Rect rect;
    std::function<void()> onClick;
    bool isHovered = false;
    TTF_Font* font = nullptr;
    SDL_Color textColor;
    SDL_Color bgColor;
    SDL_Color hoverColor;
    SDL_Texture* texture = nullptr;

    void createTexture(SDL_Renderer* renderer);
};
