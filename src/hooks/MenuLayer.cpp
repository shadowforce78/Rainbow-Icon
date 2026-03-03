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

        if (Loader::get()->isModLoaded("capeling.icon_profile"))
        {
            geode::log::info("Rainbow Icon: 'capeling.icon_profile' is LOADED");
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
            player->setColor(mainColor);

            auto gm = GameManager::sharedState();
            ccColor3B secondaryColor;

            if (settings.preset == 0 || settings.preset == 1) // Both
                secondaryColor = settings.sync ? mainColor : invertedColor;
            else if (settings.preset == 3) // Secondary only
                secondaryColor = mainColor;
            else
                secondaryColor = gm->colorForIdx(gm->getPlayerColor2());

            player->setSecondColor(secondaryColor);

            if (settings.glow)
            {
                player->setGlowOutline(settings.sync ? mainColor : invertedColor);
            }

            player->updateColors();
        }
    }
};
