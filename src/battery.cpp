#include <stdio.h>
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
    int batt_num = 0;
    FILE *file;
    string syspath = battPath[batt_num].c_str();
    if (numBattery() == 2) {

    }
    string current_power = syspath + "/current_now";
    string current_voltage = syspath +"/voltage_now";
    string charge_now = syspath + "/charge_now";
    string charge_full = syspath + "/charge_full";
    string percent = syspath + "/capacity";
    file = fopen(current_power.c_str(), "r" );
    powerMap["current_now"] = {file, 0};
    file = fopen(current_voltage.c_str(), "r");
    powerMap["voltage_now"] = {file, 0};
    file = fopen(charge_now.c_str(), "r");
    powerMap["charge_now"] = {file, 0};
    file = fopen(charge_full.c_str(), "r");
    powerMap["charge_full"] = {file, 0};

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
    current_percent = powerMap["charge_now"].value / powerMap["charge_full"].value * 100;
}

BatteryStats Battery_Stats;
