#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Structure to hold mod settings
struct RainbowSettings
{
    double speed;
    double saturation;
    double brightness;
    int64_t offset_color_p1;
    int64_t offset_color_p2;
    bool enable;
    bool glow;
    int64_t preset;
    int64_t playerPreset;
    bool sync;
    bool wave;
    bool superSpeed;
    bool pastel;
    bool editorEnable;
    bool garagePreview;
    bool menuEnable;
    bool profileEnable;
};

// Fetches all settings at once
inline RainbowSettings getModSettings()
{
    RainbowSettings settings;
    auto mod = Mod::get();
    settings.speed = mod->getSettingValue<double>("speed");
    settings.saturation = mod->getSettingValue<double>("saturation");
    settings.brightness = mod->getSettingValue<double>("brightness");
    settings.offset_color_p1 = mod->getSettingValue<int64_t>("offset_color_p1");
    settings.offset_color_p2 = mod->getSettingValue<int64_t>("offset_color_p2");
    settings.enable = mod->getSettingValue<bool>("enable");
    settings.glow = mod->getSettingValue<bool>("glow");
    settings.preset = mod->getSettingValue<int64_t>("preset");
    settings.playerPreset = mod->getSettingValue<int64_t>("playerPreset");
    settings.sync = mod->getSettingValue<bool>("sync");
    settings.wave = mod->getSettingValue<bool>("wave");
    settings.pastel = mod->getSettingValue<bool>("pastel");
    settings.superSpeed = mod->getSettingValue<bool>("superSpeed");
    settings.editorEnable = mod->getSettingValue<bool>("editorEnable");
    settings.garagePreview = mod->getSettingValue<bool>("garagePreview");
    settings.menuEnable = mod->getSettingValue<bool>("menuEnable");
    settings.profileEnable = mod->getSettingValue<bool>("profileEnable");
    return settings;
}
