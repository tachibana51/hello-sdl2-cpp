
#include "Button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

Button::Button(const std::string& text, int x, int y, int w, int h, std::function<void()> onClick)
    : text(text), rect({x, y, w, h}), onClick(onClick) {
    textColor = {255, 255, 255, 255};
    bgColor = {0, 0, 0, 255};
    hoverColor = {50, 50, 50, 255};
    font = TTF_OpenFont("JetBrainsMonoNL-Regular.ttf", 16); // フォントパスを適切に設定してください
}

Button::~Button() {
    if (font) {
        TTF_CloseFont(font);
    }
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Button::createTexture(SDL_Renderer* renderer) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface) {
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
    }
}

void Button::render(SDL_Renderer* renderer) {
    SDL_Color currentColor = isHovered ? hoverColor : bgColor;
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(renderer, &rect);

    if (!texture) {
        createTexture(renderer);
    }

    if (texture) {
        int textW, textH;
        SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
        SDL_Rect textRect = {rect.x + (rect.w - textW) / 2, rect.y + (rect.h - textH) / 2, textW, textH};
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
    }
}

void Button::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = (x >= rect.x) && (x <= rect.x + rect.w) && (y >= rect.y) && (y <= rect.y + rect.h);

        if (inside) {
            isHovered = true;

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (onClick) {
                    onClick();
                }
            }
        } else {
            isHovered = false;
        }
    }
}
