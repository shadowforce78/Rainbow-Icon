#pragma once

#include <Geode/Geode.hpp>
#include "settings.hpp"

using namespace geode::prelude;

// Global hue value shared across hooks
extern float g_hue;

// Convert HSV to RGB
void HSVtoRGB(float &r, float &g, float &b, float h, float s, float v);

// Get a rainbow color with given offset, saturation and value
cocos2d::_ccColor3B getRainbow(float offset, float saturation, float value);

// Update the global hue based on current settings
void updateHue(const RainbowSettings &settings);
