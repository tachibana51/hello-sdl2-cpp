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

pkg_check_modules(sndfile REQUIRED IMPORTED_TARGET sndfile)
# ヘッダーファイルのディレクトリ
include_directories(src)

# ソースファイルの設定
set(SOURCES
    src/main.cpp
    src/Components/MorphingComponent.hpp
    src/Components/PositionComponent.hpp
    src/Components/VelocityComponent.hpp
    src/Components/FunctionComponent.hpp
    src/Components/CameraComponent.hpp
    src/Components/ProjectionComponent.hpp
    src/Components/AudioComponent.hpp
    src/Components/WaveletComponent.hpp
    src/Components/WaveletVisualizationComponent.hpp
)

# Engine/ECS/*.cpp を追加
set(ENGINE_ECS_SOURCES
    src/Engine/ECS/Coordinator.hpp
    src/Engine/ECS/EntityManager.hpp
    src/Engine/ECS/ComponentManager.hpp
    src/Engine/ECS/SystemManager.hpp
    src/Engine/ECS/System.hpp
)

# Engine/Events/*.cpp を追加
set(ENGINE_EVENTS_SOURCES
    src/Engine/Events/Event.hpp
    src/Engine/Events/EventManager.hpp
)

# Systems/*.cpp を追加
set(SYSTEMS_SOURCES
    src/Systems/MovementSystem.cpp
    src/Systems/RenderSystem.cpp
    src/Systems/MorphingSystem.cpp
    src/Systems/CameraSystem.cpp
    src/Systems/AudioSystem.hpp
    src/Systems/WaveletSystem.hpp
    src/Systems/WaveletVisualizationSystem.hpp
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
target_link_libraries(PointCloudApp ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES} sndfile)
