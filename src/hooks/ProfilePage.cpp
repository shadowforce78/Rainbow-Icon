#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include "../settings.hpp"
#include "../color_utils.hpp"

using namespace geode::prelude;

class $modify(MyProfilePage, ProfilePage)
{
    bool init(int accountID, bool ownProfile)
    {
        if (!ProfilePage::init(accountID, ownProfile))
            return false;

        this->schedule(schedule_selector(MyProfilePage::updateProfileRainbow));
        return true;
    }

    void updateProfileRainbow(float dt)
    {
        static int logCounter = 0;
        bool doLog = (logCounter++ % 180) == 0;

        auto settings = getModSettings();
        if (!settings.enable || !settings.profileEnable)
            return;

        updateHue(settings);

        if (settings.pastel)
        {
            settings.saturation = 50;
            settings.brightness = 90;
        }

        auto mainColor = getRainbow(settings.offset_color_p1, settings.saturation, settings.brightness);
        auto invertedColor = getRainbow(settings.offset_color_p1 + 180, settings.saturation, settings.brightness);

        CCNode *menu = nullptr;
        auto children = this->getChildren();

        if (children)
        {
            for (int i = 0; i < children->count(); ++i)
            {
                auto child = static_cast<CCNode *>(children->objectAtIndex(i));
                if (typeinfo_cast<CCLayer *>(child))
                {
                    auto potentialMenu = child->getChildByID("player-menu");
                    if (potentialMenu)
                    {
                        menu = potentialMenu;
                        if (doLog)
                            geode::log::info("ProfilePage: Found 'player-menu' in child layer {}, ID: {}", i, child->getID());
                        break;
                    }
                }
            }
        }

        if (!menu)
        {
            if (doLog)
            {
                geode::log::info("ProfilePage: 'player-menu' not found in any child layer");
                if (children)
                {
                    for (int i = 0; i < children->count(); ++i)
                    {
                        auto child = static_cast<CCNode *>(children->objectAtIndex(i));
                        geode::log::info("ProfilePage Child {}: ID='{}', Type={}", i, child->getID(), typeid(*child).name());
                    }
                }
            }
            return;
        }

        auto menuChildren = menu->getChildren();
        if (!menuChildren)
        {
            if (doLog)
                geode::log::info("ProfilePage: 'player-menu' has no children");
            return;
        }

        if (doLog)
            geode::log::info("ProfilePage: 'player-menu' child count: {}", menuChildren->count());

        for (int i = 0; i < menuChildren->count(); ++i)
        {
            auto node = static_cast<CCNode *>(menuChildren->objectAtIndex(i));

            SimplePlayer *player = nullptr;
            auto nodeChildren = node->getChildren();
            if (nodeChildren)
            {
                for (int j = 0; j < nodeChildren->count(); ++j)
                {
                    auto child = static_cast<CCNode *>(nodeChildren->objectAtIndex(j));
                    player = typeinfo_cast<SimplePlayer *>(child);
                    if (player)
                        break;
                }
            }

            if (player)
            {
                player->updateColors();

                if (settings.glow)
                {
                    player->setGlowOutline(settings.sync ? mainColor : invertedColor);
                }

                auto playerChildren = player->getChildren();
                if (playerChildren)
                {
                    for (int k = 0; k < playerChildren->count(); ++k)
                    {
                        auto mainNode = static_cast<CCNode *>(playerChildren->objectAtIndex(k));
                        auto mainSprite = typeinfo_cast<CCSprite *>(mainNode);

                        if (mainSprite)
                        {
                            mainSprite->setColor(mainColor);

                            auto subChildren = mainSprite->getChildren();
                            if (subChildren)
                            {
                                if (doLog && k == 0)
                                    geode::log::info("ProfilePage Player Child {}: SubChildren Count: {}", k, subChildren->count());

                                // Child [0] -> Glow
                                if (settings.glow && subChildren->count() > 0)
                                {
                                    auto glowNode = static_cast<CCNode *>(subChildren->objectAtIndex(0));
                                    if (auto glowSprite = typeinfo_cast<CCSprite *>(glowNode))
                                    {
                                        glowSprite->setColor(settings.sync ? mainColor : invertedColor);
                                    }
                                    else
                                    {
                                        if (doLog)
                                            geode::log::info("ProfilePage: Child 0 is NOT CCSprite");
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
                                            secondaryColor = settings.sync ? mainColor : invertedColor;
                                        else if (settings.preset == 3)
                                            secondaryColor = mainColor;
                                        else
                                            secondaryColor = GameManager::sharedState()->colorForIdx(GameManager::sharedState()->getPlayerColor2());

                                        if (settings.preset == 0 || settings.preset == 1 || settings.preset == 3)
                                        {
                                            secSprite->setColor(secondaryColor);
                                        }
                                    }
                                    else
                                    {
                                        if (doLog)
                                            geode::log::info("ProfilePage: Child 2 is NOT CCSprite");
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
                    geode::log::info("ProfilePage: SimplePlayer not found in child {}", i);
            }
        }
    }
};
