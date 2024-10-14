
#include "GUIManager.h"

GUIManager::GUIManager(SDL_Renderer* renderer, EventManager& eventManager)
    : renderer(renderer), eventManager(eventManager) {}

GUIManager::~GUIManager() {
    for (auto element : elements) {
        delete element;
    }
    elements.clear();
}

void GUIManager::render() {
    for (auto element : elements) {
        element->render(renderer);
    }
}

void GUIManager::handleEvent(const SDL_Event& event) {
    for (auto element : elements) {
        element->handleEvent(event);
    }
}

void GUIManager::addElement(GUIElement* element) {
    elements.push_back(element);
}
