#pragma once

#include <vector>
#include <cstdint>

struct AudioComponent {
    std::vector<float> samples; // 音声データのサンプル
    int sampleRate;             // サンプリングレート
    int channels;               // チャンネル数
};
