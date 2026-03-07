#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "../settings.hpp"
#include "../color_utils.hpp"

using namespace geode::prelude;

class $modify(MyGarageLayer, GJGarageLayer)
{
    void onSettings(CCObject *)
    {
        geode::openSettingsPopup(Mod::get());
    }

    bool init()
    {
        if (!GJGarageLayer::init())
            return false;

        geode::log::info("MyGarageLayer::init called - Hook is working!");

        if (Mod::get()->getSettingValue<bool>("shortcut"))
        {
            auto menu = this->getChildByID("shards-menu");
            if (menu)
            {
                auto btnSprite = CCSprite::create("btnSprite.png"_spr);
                if (btnSprite)
                {
                    auto btn = CCMenuItemSpriteExtra::create(
                        btnSprite, this, menu_selector(MyGarageLayer::onSettings));
                    btn->setID("rainbow-settings-button"_spr);
                    menu->addChild(btn);
                    menu->updateLayout();
                }
            }
        }

        this->schedule(schedule_selector(MyGarageLayer::rainbowUpdate));
        return true;
    }

    void rainbowUpdate(float dt)
    {
        static int logCounter = 0;
        bool doLog = (logCounter++ % 180) == 0;

        auto settings = getModSettings();

        if (doLog)
        {
            geode::log::info("MyGarageLayer::rainbowUpdate - Enable: {}, GaragePreview: {}", settings.enable, settings.garagePreview);
        }

        if (!settings.enable || !settings.garagePreview)
            return;

        updateHue(settings);

        if (settings.pastel)
        {
            settings.saturation = 50;
            settings.brightness = 90;
        }

        auto mainColorP1 = getRainbow(settings.offset_color_p1, settings.saturation, settings.brightness);
        auto invertedColorP1 = getRainbow(settings.offset_color_p1 + 180, settings.saturation, settings.brightness);

        SimplePlayer *player = m_playerObject;
        if (!player)
        {
            player = typeinfo_cast<SimplePlayer *>(this->getChildByID("player-icon"));
        }

        if (player)
        {
            if (doLog)
            {
                geode::log::info("GaragePlayer Found: {}", player);
            }

            auto gm = GameManager::sharedState();

            if (settings.preset == 1) // Both colors
            {
                player->setColor(mainColorP1);
                player->setSecondColor(settings.sync ? mainColorP1 : invertedColorP1);
            }
            else if (settings.preset == 2) // Primary color only
            {
                player->setColor(mainColorP1);
                player->setSecondColor(gm->colorForIdx(gm->getPlayerColor2()));
            }
            else if (settings.preset == 3) // Secondary color only
            {
                player->setColor(gm->colorForIdx(gm->getPlayerColor()));
                player->setSecondColor(mainColorP1);
            }

            if (settings.glow)
            {
                player->setGlowOutline(settings.sync ? mainColorP1 : invertedColorP1);
            }

            player->updateColors();
        }
        else
        {
            if (doLog)
                geode::log::info("GaragePlayer NOT FOUND");
        }
    }
};
