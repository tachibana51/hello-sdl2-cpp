// onButtonPress.h
#pragma once
#include <vector>
#include <string>
#include "Entity.h"
#include "generatePointCloud.h"
#include "switchCoordinateSystem.h"
#include "PositionComponent.h"

void onButtonPress(std::vector<Entity>& pointCloud, const std::string& buttonId);
