
#pragma once

struct Event {
    enum Type {
        None,
        GeneratePointCloud,
        SwitchCoordinateSystem,
        // その他のイベントタイプ
    } type;

    // イベントデータ（必要に応じて追加）
};
