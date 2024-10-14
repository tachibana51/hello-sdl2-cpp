# CMakeLists.txt
INCLUDE(FindPkgConfig)
cmake_minimum_required(VERSION 3.10)
project(PointCloudApp)

# C++20を使用
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SDL2の設定
find_package(SDL2 REQUIRED)
# SDL2_ttf library
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
include_directories(${SDL2_INCLUDE_DIRS})

# SDL2_ttfの設定

# GLMの設定
find_path(GLM_INCLUDE_DIR glm/glm.hpp)
if (GLM_INCLUDE_DIR)
    include_directories(${GLM_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Could not find GLM library.")
endif()

# ヘッダーファイルのディレクトリ
include_directories(src)

# ソースファイルの設定
set(SOURCES
    src/main.cpp
    src/Components/MorphingComponent.h
    src/Components/PositionComponent.h
    src/Components/VelocityComponent.h
    src/Components/FunctionComponent.h
    src/Components/CameraComponent.h
    src/Components/ProjectionComponent.h
)

# Engine/ECS/*.cpp を追加
set(ENGINE_ECS_SOURCES
    src/Engine/ECS/Coordinator.h
    src/Engine/ECS/EntityManager.h
    src/Engine/ECS/ComponentManager.h
    src/Engine/ECS/SystemManager.h
    src/Engine/ECS/System.h
)

# Engine/Events/*.cpp を追加
set(ENGINE_EVENTS_SOURCES
    src/Engine/Events/Event.h
    src/Engine/Events/EventManager.h
)

# Systems/*.cpp を追加
set(SYSTEMS_SOURCES
    src/Systems/MovementSystem.cpp
    src/Systems/RenderSystem.cpp
    src/Systems/MorphingSystem.cpp
    src/Systems/CameraSystem.cpp
    # 他のシステムがあればここに追加
)

# GUI/*.cpp を追加
set(GUI_SOURCES
    src/GUI/Button.cpp
    src/GUI/GUIManager.cpp
    # 他のGUI要素があればここに追加
)

# ソースファイルを結合
set(SOURCES
    ${SOURCES}
    ${ENGINE_ECS_SOURCES}
    ${ENGINE_EVENTS_SOURCES}
    ${SYSTEMS_SOURCES}
    ${GUI_SOURCES}
)

# 実行ファイルの生成
add_executable(PointCloudApp ${SOURCES})

# ライブラリのリンク
target_link_libraries(PointCloudApp ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES})
