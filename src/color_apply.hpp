#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include "settings.hpp"

using namespace geode::prelude;

// Apply rainbow colors to a PlayerObject (in-game)
void applyRainbowColors(PlayerObject *player, bool isPlayer1, RainbowSettings &settings, const cocos2d::_ccColor3B &mainColor, const cocos2d::_ccColor3B &invertedColor);

// Apply rainbow colors to a SimplePlayer (garage, menus)
void applyRainbowColorsSimple(SimplePlayer *player, bool isPlayer1, RainbowSettings &settings, const cocos2d::_ccColor3B &mainColor, const cocos2d::_ccColor3B &invertedColor);
