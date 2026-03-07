#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include "../settings.hpp"
#include "../color_utils.hpp"

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer)
{
    bool init()
    {
        if (!MenuLayer::init())
            return false;

        if (Loader::get()->isModLoaded("dasshu.icon_profile"))
        {
            this->schedule(schedule_selector(MyMenuLayer::updateProfileRainbow));
        }

        return true;
    }

    void updateProfileRainbow(float dt)
    {
        auto settings = getModSettings();
        if (!settings.enable || !settings.menuEnable)
            return;

        updateHue(settings);

        if (settings.pastel)
        {
            settings.saturation = 50;
            settings.brightness = 90;
        }

        auto mainColor = getRainbow(settings.offset_color_p1, settings.saturation, settings.brightness);
        auto invertedColor = getRainbow(settings.offset_color_p1 + 180, settings.saturation, settings.brightness);

        auto menu = this->getChildByID("profile-menu");
        if (!menu)
            return;

        auto btn = static_cast<CCMenuItemSpriteExtra *>(menu->getChildByID("profile-button"));
        if (!btn)
            return;

        CCNode *iconNode = btn->getChildByID("profile-icon");
        if (!iconNode)
        {
            auto normalImg = btn->getNormalImage();
            if (normalImg && std::string(normalImg->getID()) == "profile-icon")
            {
                iconNode = normalImg;
            }
        }

        if (!iconNode)
            return;

        SimplePlayer *player = nullptr;
        auto children = iconNode->getChildren();
        if (children)
        {
            for (int i = 0; i < children->count(); ++i)
            {
                auto child = static_cast<CCNode *>(children->objectAtIndex(i));
                player = typeinfo_cast<SimplePlayer *>(child);
                if (player)
                    break;
            }
        }

        if (player)
        {
            auto gm = GameManager::sharedState();

            if (settings.preset == 1) // Both colors
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

            if (settings.glow)
            {
                player->setGlowOutline(settings.sync ? mainColor : invertedColor);
            }

            player->updateColors();
        }
    }
};
