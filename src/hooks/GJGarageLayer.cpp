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

            player->updateColors();

            if (settings.glow)
            {
                player->setGlowOutline(settings.sync ? mainColorP1 : invertedColorP1);
            }

            auto children = player->getChildren();
            if (children)
            {
                for (int i = 0; i < children->count(); ++i)
                {
                    auto mainNode = static_cast<CCNode *>(children->objectAtIndex(i));
                    auto mainSprite = typeinfo_cast<CCSprite *>(mainNode);

                    if (mainSprite)
                    {
                        mainSprite->setColor(mainColorP1);

                        if (mainSprite->getChildrenCount() > 0)
                        {
                            auto subChildren = mainSprite->getChildren();
                            if (doLog && i == 0)
                                geode::log::info("SubChild Count for Child 0: {}", subChildren->count());

                            // Child [0] -> Glow
                            if (settings.glow && subChildren->count() > 0)
                            {
                                auto glowNode = static_cast<CCNode *>(subChildren->objectAtIndex(0));
                                if (auto glowSprite = typeinfo_cast<CCSprite *>(glowNode))
                                {
                                    glowSprite->setColor(settings.sync ? mainColorP1 : invertedColorP1);
                                }
                            }

                            // Child [2] -> Secondary
                            if (subChildren->count() > 2)
                            {
                                auto secNode = static_cast<CCNode *>(subChildren->objectAtIndex(2));
                                if (auto secSprite = typeinfo_cast<CCSprite *>(secNode))
                                {
                                    ccColor3B secondaryColor;
                                    if (settings.preset == 0 || settings.preset == 1)
                                        secondaryColor = settings.sync ? mainColorP1 : invertedColorP1;
                                    else if (settings.preset == 3)
                                        secondaryColor = mainColorP1;
                                    else
                                        secondaryColor = GameManager::sharedState()->colorForIdx(GameManager::sharedState()->getPlayerColor2());

                                    if (settings.preset == 0 || settings.preset == 1 || settings.preset == 3)
                                    {
                                        secSprite->setColor(secondaryColor);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (doLog)
                geode::log::info("GaragePlayer NOT FOUND");
        }
    }
};
