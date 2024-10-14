
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <memory>
#include "Engine/ECS/Coordinator.h"
#include "Engine/Events/EventManager.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/MorphingSystem.h"
#include "Systems/CameraSystem.h"
#include "Components/PositionComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/FunctionComponent.h"
#include "Components/MorphingComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ProjectionComponent.h"
#include "Components/WaveletComponent.h"
#include "GUI/GUIManager.h"
#include "GUI/Button.h"
#include "Systems/AudioSystem.h"
#include "Systems/WaveletSystem.h"

// 座標系の種類を定義
enum class CoordinateSystemType {
    Cartesian,
    Polar
};

// 現在の座標系を追跡
CoordinateSystemType currentCoordinateSystem = CoordinateSystemType::Cartesian;

// グローバル変数としてCoordinatorとEventManagerを宣言
ECS::Coordinator coordinator;
EventManager eventManager;

// カメラの初期設定
void setupCamera(ECS::Entity entity) {
    // カメラの位置と向きの初期設定
    coordinator.addComponent<CameraComponent>(entity, CameraComponent{
        glm::vec3(0.0f, 0.0f, 2*20.0f),            // カメラの位置（初期位置）
        glm::half_pi<float>(),                    // yaw (90度 in radians)
        0.0f,                                     // pitch
        glm::radians(45.0f)                       // fov
    });

    // 投影情報の初期設定
    coordinator.addComponent<ProjectionComponent>(entity, ProjectionComponent{
        800.0f / 600.0f, // アスペクト比（初期ウィンドウサイズに合わせる）
        0.1f,             // 近クリップ面
        100.0f            // 遠クリップ面
    });
}



// ランダム速度を割り当てる関数の実装
void assignRandomVelocities() {
    for (ECS::Entity entity = 0; entity < ECS::MAX_ENTITIES; ++entity) {
        if (coordinator.hasComponent<VelocityComponent>(entity)) {
            auto& velocity = coordinator.getComponent<VelocityComponent>(entity).velocity;
            velocity = glm::vec3(
                (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 1.0f, // -0.5 to 0.5
                (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 1.0f,
                (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 1.0f
            );
        }
    }
    SDL_Log("Assigned random velocities to all entities.");
}
void switchCoordinateSystemMorph() {
    // ターゲット座標系を決定
    CoordinateSystemType targetCoordinateSystem;
    if (currentCoordinateSystem == CoordinateSystemType::Cartesian) {
        targetCoordinateSystem = CoordinateSystemType::Polar;
    } else {
        targetCoordinateSystem = CoordinateSystemType::Cartesian;
    }

    // モーフの総時間（秒）
    float morphDuration = 2.0f;

    // すべてのエンティティを対象にモーフィングを設定
    for (ECS::Entity entity = 0; entity < ECS::MAX_ENTITIES; ++entity) {
        if (coordinator.hasComponent<PositionComponent>(entity)) {
            glm::vec3 currentPos = coordinator.getComponent<PositionComponent>(entity).position;
            glm::vec3 targetPos;

            if (targetCoordinateSystem == CoordinateSystemType::Polar) {
                // 直交座標系から極座標系への変換
                float r = glm::length(glm::vec2(currentPos.x, currentPos.y));
                float theta = atan2(currentPos.y, currentPos.x);
                float phi = currentPos.z; // 3Dの場合は適宜設定

                // 視覚的に極座標系を表現するために、rをx軸、thetaをy軸、phiをz軸として設定
                targetPos = glm::vec3(r, theta, phi);
            } else {
                // 極座標系から直交座標系への変換
                // x = r * cos(theta), y = r * sin(theta), z = phi
                float r = currentPos.x;
                float theta = currentPos.y;
                float phi = currentPos.z;

                targetPos = glm::vec3(r * cos(theta), r * sin(theta), phi);
            }

            // MorphingComponent を追加または更新
            if (coordinator.hasComponent<MorphingComponent>(entity)) {
                // 既存の MorphingComponent を更新
                auto& morph = coordinator.getComponent<MorphingComponent>(entity);
                morph.startPosition = currentPos;
                morph.targetPosition = targetPos;
                morph.duration = morphDuration;
                morph.elapsed = 0.0f;
            } else {
                // 新たに MorphingComponent を追加
                coordinator.addComponent<MorphingComponent>(entity, MorphingComponent{
                    currentPos,
                    targetPos,
                    morphDuration,
                    0.0f
                });
            }
        }
    }

    // 現在の座標系を更新
    currentCoordinateSystem = targetCoordinateSystem;

    SDL_Log("Coordinate system switched to %s.",
            targetCoordinateSystem == CoordinateSystemType::Cartesian ? "Cartesian" : "Polar");
}

// 点群の生成処理
void generatePointCloud(int numPoints) {
    for (int i = 0; i < numPoints; ++i) {
        ECS::Entity entity = coordinator.createEntity();
        glm::vec3 position(
            static_cast<float>(rand()) / RAND_MAX * 10.0f - 5.0f, // -5 to 5
            static_cast<float>(rand()) / RAND_MAX * 10.0f - 5.0f, // -5 to 5
            static_cast<float>(rand()) / RAND_MAX * 10.0f - 5.0f  // -5 to 5
        );
        glm::vec3 velocity(
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.1f,
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.1f,
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.1f
        );

        coordinator.addComponent(entity, PositionComponent{position});
        coordinator.addComponent(entity, VelocityComponent{velocity});
    }

    SDL_Log("Generated %d point entities.", numPoints);
}

// サンプルポイントの生成関数
void generateSamplePoint(ECS::Entity entity, glm::vec3 position) {
    coordinator.addComponent<PositionComponent>(entity, PositionComponent{position});
}

int main(int argc, char* argv[]) {
    // SDLとTTFの初期化
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        SDL_Log("Unable to initialize SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    const int INITIAL_WINDOW_WIDTH = 800;
    const int INITIAL_WINDOW_HEIGHT = 600;

    int windowWidth = INITIAL_WINDOW_WIDTH;
    int windowHeight = INITIAL_WINDOW_HEIGHT;

    SDL_Window* window = SDL_CreateWindow("3D Point Cloud Visualization",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          windowWidth, windowHeight,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // レンダラーの作成（垂直同期を有効にする）
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // ECSの初期化
    coordinator.init();

    // コンポーネントの登録
    coordinator.registerComponent<PositionComponent>();
    coordinator.registerComponent<VelocityComponent>();
    coordinator.registerComponent<FunctionComponent>();
    coordinator.registerComponent<MorphingComponent>();    // MorphingComponent の登録
    coordinator.registerComponent<CameraComponent>();      // CameraComponent の登録
    coordinator.registerComponent<ProjectionComponent>();  // ProjectionComponent の登録
    coordinator.registerComponent<AudioComponent>();
    coordinator.registerComponent<WaveletComponent>();

    // システムの登録
    auto movementSystem = coordinator.registerSystem<MovementSystem>();
    {
        ECS::Signature signature;
        signature.set(coordinator.getComponentType<PositionComponent>(), true);
        signature.set(coordinator.getComponentType<VelocityComponent>(), true); // VelocityComponentが必要な場合
        movementSystem->setCoordinator(&coordinator);
        coordinator.setSystemSignature<MovementSystem>(signature);
    }

    auto renderSystem = coordinator.registerSystem<RenderSystem>();
    {
        ECS::Signature signature;
        signature.set(coordinator.getComponentType<PositionComponent>(), true);
        renderSystem->setCoordinator(&coordinator);
        renderSystem->setRenderer(renderer);
        renderSystem->setWindowSize(windowWidth, windowHeight);
        coordinator.setSystemSignature<RenderSystem>(signature);
                // フォントのロード
        std::string fontPath = "JetBrainsMonoNL-Regular.ttf"; // フォントファイルのパスを指定
        int fontSize = 12; // フォントサイズを指定
        if (!renderSystem->loadFont(fontPath, fontSize)) {
            SDL_Log("Failed to load font. Exiting.");
            // 必要に応じてエラーハンドリング（例: アプリケーションを終了）
            // Clean up and exit
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return -1;
        }
    }

    auto morphingSystem = coordinator.registerSystem<MorphingSystem>(); // MorphingSystem の登録
    {
        ECS::Signature signature;
        signature.set(coordinator.getComponentType<MorphingComponent>(), true);
        morphingSystem->setCoordinator(&coordinator);
        coordinator.setSystemSignature<MorphingSystem>(signature);
    }

    auto cameraSystem = coordinator.registerSystem<CameraSystem>(); // CameraSystem の登録
    {
        ECS::Signature signature;
        signature.set(coordinator.getComponentType<CameraComponent>(), true);
        signature.set(coordinator.getComponentType<ProjectionComponent>(), true);
        cameraSystem->setCoordinator(&coordinator);
        coordinator.setSystemSignature<CameraSystem>(signature);
    }
    // AudioSystem の登録
    auto audioSystem = coordinator.registerSystem<AudioSystem>();
    {
        ECS::Signature signature;
        signature.set(coordinator.getComponentType<AudioComponent>(), true);
        audioSystem->setCoordinator(&coordinator);
        coordinator.setSystemSignature<AudioSystem>(signature);
    }
    // WaveletSystem の登録
    auto waveletSystem = coordinator.registerSystem<WaveletSystem>();
    {
        ECS::Signature signature;
        signature.set(coordinator.getComponentType<WaveletComponent>(), true);
        waveletSystem->setCoordinator(&coordinator);
        coordinator.setSystemSignature<WaveletSystem>(signature);
    }


    // カメラの初期設定
    ECS::Entity cameraEntity = coordinator.createEntity();
    setupCamera(cameraEntity);
    // RenderSystem に CameraSystem の参照を設定
    renderSystem->setCameraSystem(cameraSystem.get());

    // 初期点群の生成（テスト用）
    generatePointCloud(1000);
    SDL_Log("Initial point cloud generated.");

    // サンプルポイントの生成
    ECS::Entity sampleEntity = coordinator.createEntity();
    generateSamplePoint(sampleEntity, glm::vec3(0.0f, 0.0f, 0.0f)); // 原点に配置
    SDL_Log("Sample point at origin generated.");

    // イベントリスナーの設定
    eventManager.addListener(Event::GeneratePointCloud, [](const Event&) {
        generatePointCloud(1000);
    });

    eventManager.addListener(Event::SwitchCoordinateSystem, [](const Event&) {
        switchCoordinateSystemMorph(); // 座標系の切り替えをモーフィングで実行
    });

    // GUIマネージャの初期化
    GUIManager guiManager(renderer, eventManager);

    // GUI要素の追加
    guiManager.addElement(new Button("Generate", 10, 10, 100, 30, []() {
        eventManager.sendEvent(Event{Event::GeneratePointCloud});
    }));

    guiManager.addElement(new Button("Switch Coordinate", 120, 10, 150, 30, []() {
        eventManager.sendEvent(Event{Event::SwitchCoordinateSystem});
    }));

    guiManager.addElement(new Button("Assign Velocities", 280, 10, 150, 30, []() {
        assignRandomVelocities();
    }));

    guiManager.addElement(new Button("Load Audio", 440, 10, 120, 30, [audioSystem]() {
        // ファイル選択ダイアログを開く（簡易的に固定パスを使用）
        std::string audioPath = "assets/audio/in.wav"; // 実際にはファイル選択ダイアログを実装することを推奨
        ECS::Entity audioEntity = coordinator.createEntity();
        if (audioSystem->loadAudioFile(audioPath, audioEntity)) {
            SDL_Log("Audio file loaded and assigned to entity %d.", audioEntity);
        }
    }));
    // ボタンの追加
    guiManager.addElement(new Button("Wavelet Transform", 440, 50, 150, 30, [waveletSystem]() {
        // 音声エンティティを取得（ここでは単一エンティティと仮定）
        // 複数エンティティに対応する場合はループを使用
        for (ECS::Entity entity = 0; entity < ECS::MAX_ENTITIES; ++entity) {
            if (coordinator.hasComponent<AudioComponent>(entity)) {
                waveletSystem->performWaveletTransform(entity);
            }
        }
    }));

    guiManager.addElement(new Button("Inverse Wavelet", 600, 50, 150, 30, [waveletSystem]() {
        // ウェーブレット変換されたエンティティを取得
        for (ECS::Entity entity = 0; entity < ECS::MAX_ENTITIES; ++entity) {
            if (coordinator.hasComponent<WaveletComponent>(entity)) {
                waveletSystem->performInverseWaveletTransform(entity);
            }
        }
    }));


    // メインループ
    bool running = true;
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // ウィンドウサイズ変更の処理
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                    renderSystem->setWindowSize(windowWidth, windowHeight);

                    // ProjectionComponent のアスペクト比を更新
                    for (ECS::Entity entity = 0; entity < ECS::MAX_ENTITIES; ++entity) {
                        if (coordinator.hasComponent<ProjectionComponent>(entity)) {
                            auto& proj = coordinator.getComponent<ProjectionComponent>(entity);
                            proj.aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
                        }
                    }
                }
            }

            // GUIイベントの処理
            guiManager.handleEvent(event);

            // CameraSystem のマウスイベントの処理
            cameraSystem->handleMouseEvent(event);
        }

        // システムの更新
        movementSystem->update(deltaTime);
        morphingSystem->update(deltaTime);

        // イベントの処理
        eventManager.processEvents();

        // 描画処理の開始
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 背景色を黒に設定
        SDL_RenderClear(renderer);

        // RenderSystem の描画を行う
        renderSystem->update(deltaTime); // 描画を行う

        // GUIの描画
        guiManager.render();
        cameraSystem->handleMouseEvent(event);
        // レンダリングの表示
        SDL_RenderPresent(renderer);
    }

    // クリーンアップ
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
