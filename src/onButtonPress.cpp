// onButtonPress.h
#include "onButtonPress.h"
#include <vector>
#include <string>
#include "Entity.h"
#include "generatePointCloud.h"
#include "switchCoordinateSystem.h"
#include "PositionComponent.h"

void onButtonPress(std::vector<Entity>& pointCloud, const std::string& buttonId) {
    if (buttonId == "generate") {
        pointCloud = generatePointCloud(1000);
    } else if (buttonId == "switchCoordinate") {
        switchCoordinateSystem(pointCloud, CoordinateSystem::Polar);
    }
    // その他のボタン処理
}
