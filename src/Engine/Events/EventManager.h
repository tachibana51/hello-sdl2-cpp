
#pragma once

#include "Event.h"
#include <queue>
#include <functional>
#include <unordered_map>
#include <vector>

class EventManager {
public:
    using EventHandler = std::function<void(const Event&)>;

    void addListener(Event::Type eventType, EventHandler handler) {
        listeners[eventType].push_back(handler);
    }

    void sendEvent(const Event& event) {
        eventQueue.push(event);
    }

    void processEvents() {
        while (!eventQueue.empty()) {
            Event event = eventQueue.front();
            eventQueue.pop();

            auto& handlers = listeners[event.type];
            for (auto& handler : handlers) {
                handler(event);
            }
        }
    }

private:
    std::unordered_map<Event::Type, std::vector<EventHandler>> listeners;
    std::queue<Event> eventQueue;
};
