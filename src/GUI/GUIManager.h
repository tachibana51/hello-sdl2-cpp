
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "GUIElement.h"
#include "../Engine/Events/EventManager.h"

class GUIManager {
public:
    GUIManager(SDL_Renderer* renderer, EventManager& eventManager);
    ~GUIManager();

    void render();
    void handleEvent(const SDL_Event& event);
    void addElement(GUIElement* element);

private:
    SDL_Renderer* renderer;
    std::vector<GUIElement*> elements;
    EventManager& eventManager;
};
