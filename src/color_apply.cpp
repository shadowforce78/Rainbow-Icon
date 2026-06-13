#include "color_apply.hpp"

void applyRainbowColors(PlayerObject *player, bool isPlayer1, RainbowSettings &settings, const cocos2d::_ccColor3B &mainColor, const cocos2d::_ccColor3B &invertedColor)
{
    if (!player)
        return;

    int64_t playerPreset = settings.playerPreset;
    bool applyToThisPlayer = (playerPreset == 1) || (isPlayer1 && playerPreset == 2) || (!isPlayer1 && playerPreset == 3);

    if (!applyToThisPlayer)
        return;

    auto gm = GameManager::sharedState();

    // Apply Player Colors based on preset
    if (settings.preset == 0) // Glow only - restore normal colors so flashPlayer doesn't leave the icon white
    {
        player->setColor(gm->colorForIdx(gm->getPlayerColor()));
        player->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
    }
    else if (settings.preset == 1) // Both colors
    {
        player->setColor(mainColor);
        player->setSecondColor(settings.sync ? mainColor : invertedColor);
    }
    else if (settings.preset == 2) // Primary color only
    {
        player->setColor(mainColor);
        player->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
    }
    else if (settings.preset == 3) // Secondary color only
    {
        player->setColor(gm->colorForIdx(gm->getPlayerColor()));
        player->setSecondColor(mainColor);
    }
    else if (settings.preset == 4) // Glow and Trail only - restore normal icon colors, glow/wave trail still get rainbow
    {
        player->setColor(gm->colorForIdx(gm->getPlayerColor()));
        player->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
    }

    // Apply Wave Trail Color
    if (player->m_waveTrail)
    {
        if (settings.wave)
        {
            player->m_waveTrail->setColor(mainColor);
        }
        else
        {
            ccColor3B normalColor = gm->colorForIdx(isPlayer1 ? gm->getPlayerColor() : gm->getPlayerColor2());
            player->m_waveTrail->setColor(normalColor);
        }
    }

    // Apply Glow Color
    if (settings.glow)
    {
        player->m_glowColor = settings.sync ? mainColor : invertedColor;
        player->updateGlowColor();
    }
}

void applyRainbowColorsSimple(SimplePlayer *player, bool isPlayer1, RainbowSettings &settings, const cocos2d::_ccColor3B &mainColor, const cocos2d::_ccColor3B &invertedColor)
{
    if (!player)
        return;

    int64_t playerPreset = settings.playerPreset;
    bool applyToThisPlayer = (playerPreset == 1) || (isPlayer1 && playerPreset == 2) || (!isPlayer1 && playerPreset == 3);

    if (!applyToThisPlayer)
        return;

    auto gm = GameManager::sharedState();

    if (settings.preset == 0) // Glow only - restore normal colors so flashPlayer doesn't leave the icon white
    {
        player->setColor(gm->colorForIdx(gm->getPlayerColor()));
        player->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
    }
    else if (settings.preset == 1) // Both colors
    {
        player->setColor(mainColor);
        player->setSecondColor(settings.sync ? mainColor : invertedColor);
    }
    else if (settings.preset == 2) // Primary color only
    {
        player->setColor(mainColor);
        player->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
    }
    else if (settings.preset == 3) // Secondary color only
    {
        player->setColor(gm->colorForIdx(gm->getPlayerColor()));
        player->setSecondColor(mainColor);
    }
    else if (settings.preset == 4) // Glow and Trail only - restore normal icon colors, glow/wave trail still get rainbow
    {
        player->setColor(gm->colorForIdx(gm->getPlayerColor()));
        player->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
    }

    if (settings.glow)
    {
        player->setGlowOutline(settings.sync ? mainColor : invertedColor);
    }

    player->updateColors();
}
