# CMakeLists.txt
INCLUDE(FindPkgConfig)
cmake_minimum_required(VERSION 3.10)
project(PointCloudApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SDL2の設定
find_package(SDL2 REQUIRED)
# SDL2_ttf library
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
include_directories(${SDL2_INCLUDE_DIRS})

# GLMの設定（必要に応じてGLMのパスを設定）
find_path(GLM_INCLUDE_DIR glm/glm.hpp)
if (GLM_INCLUDE_DIR)
    include_directories(${GLM_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Could not find GLM library.")
endif()

# ソースファイルの設定
set(SOURCES
    src/main.cpp
    src/generatePointCloud.cpp
    src/updatePositions.cpp
    src/drawPointCloud.cpp
    src/switchCoordinateSystem.cpp
    src/morphTransition.cpp
    src/handleCameraMovement.cpp
    src/renderScene.cpp
    src/onButtonPress.cpp
    src/onMouseMovement.cpp
    src/DebugRenderUtils.h
    src/DebugUtils.h
)

# ヘッダーファイルのディレクトリ
include_directories(src)

# 実行ファイルの生成
add_executable(PointCloudApp ${SOURCES})

# ライブラリのリンク
target_link_libraries(PointCloudApp ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES})
