// src/Systems/AudioSystem.hpp
#pragma once

#include "../Engine/ECS/System.hpp"
#include "../Engine/ECS/Coordinator.hpp"
#include "../Components/AudioComponent.hpp"
#include <sndfile.h>
#include <string>
#include <SDL2/SDL.h>

class AudioSystem : public ECS::System {
public:
    AudioSystem() : coordinator(nullptr) {}
    ~AudioSystem() {}

    void setCoordinator(ECS::Coordinator* coord) {
        coordinator = coord;
    }

    // 音声ファイルを読み込む関数
    bool loadAudioFile(const std::string& filePath, ECS::Entity entity) {
        if (!coordinator) {
            SDL_Log("AudioSystem: Coordinator is null.");
            return false;
        }

        SF_INFO sfInfo;
        SNDFILE* sndFile = sf_open(filePath.c_str(), SFM_READ, &sfInfo);
        if (!sndFile) {
            SDL_Log("Failed to open audio file %s: %s", filePath.c_str(), sf_strerror(NULL));
            return false;
        }

        // サンプル数の取得
        sf_count_t numFrames = sfInfo.frames;
        int channels = sfInfo.channels;

        // バッファの確保
        std::vector<float> samples(numFrames * channels);
        sf_count_t numRead = sf_readf_float(sndFile, samples.data(), numFrames);
        if (numRead != numFrames) {
            SDL_Log("Failed to read all samples from %s.", filePath.c_str());
            sf_close(sndFile);
            return false;
        }

        sf_close(sndFile);

        // AudioComponent をエンティティに追加
        coordinator->addComponent<AudioComponent>(entity, AudioComponent{
            std::move(samples),
            sfInfo.samplerate,
            sfInfo.channels
        });

        SDL_Log("Loaded audio file %s: %ld frames, %d channels, %d Hz.",
                filePath.c_str(), numRead, channels, sfInfo.samplerate);

        return true;
    }

    void update(float deltaTime) {
        // 現在の実装では特に更新処理は不要
    }

private:
    ECS::Coordinator* coordinator;
};
