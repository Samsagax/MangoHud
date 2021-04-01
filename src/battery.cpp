#include "battery.h"
#include <filesystem.h>

namespace fs = ghc::filesystem;

int BatteryStats::numBattery(){
    fs::path path("/sys/class/power_supply/");
    int batteryCount=0;

    for (auto& p : fs::directory_iterator(path)) {
        string fileName = p.path().filename();
        //std::cout << p << '\n';
        if (fileName.find("BAT") != std::string::npos) {
            batteryCount+=1;
            battPath.push_back(p.path());
        }
    }
    return batteryCount;
}


void BatteryStats::findFiles(){
    FILE *file;
    file = fopen("/sys/class/power_supply/BAT1/current_now", "r");
    powerMap["current_now"] = {file, 0};
    file = fopen("/sys/class/power_supply/BAT1/voltage_now", "r");
    powerMap["voltage_now"] = {file, 0};
    // file = fopen("/sys/class/power_supply/BAT1/charge_now", "r");
    // powerMap["charge_now"] = {file, 0};
    // file = fopen("/sys/class/power_supply/BAT1/charge_full", "r");
    // powerMap["charge_full"] = {file, 0};

    files_fetched = true;
}

void BatteryStats::update(){
    if (!files_fetched && numBattery() > 0) {
        //we found battiers lets get their values
        findFiles();
    }

    for(auto &pair : powerMap){
        if(pair.second.file) {
            rewind(pair.second.file);
            fflush(pair.second.file);
            fscanf(pair.second.file, "%f", &pair.second.value);
            pair.second.value /= 1000000;
        }
    }
    current_watt = powerMap["current_now"].value * powerMap["voltage_now"].value;
}

BatteryStats Battery_Stats;
