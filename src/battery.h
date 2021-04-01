#pragma once
#include "overlay.h"
#include "overlay_params.h"
#include <logging.h>
#include <vector>
#include <unordered_map>
#include <filesystem>

class BatteryStats{
    public:
        int numBattery();
        void findFiles();
        void update();
        bool files_fetched  = false;
        struct powerStruct{
            FILE *file = nullptr;
            float value;
        };
        std::unordered_map<std::string, powerStruct> powerMap;
        std::vector<std::string> battPath;
        float current_watt = 0;
};

extern BatteryStats Battery_Stats;
