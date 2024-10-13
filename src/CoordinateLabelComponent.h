// CoordinateLabelComponent.h
#pragma once
#include <string>

struct CoordinateLabelComponent {
    std::string label;

    CoordinateLabelComponent(const std::string& lbl) : label(lbl) {}
};
