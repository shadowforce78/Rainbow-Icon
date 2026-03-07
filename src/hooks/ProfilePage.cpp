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
                        break;
                    }
                }
            }
        }

        if (!menu)
            return;

        auto menuChildren = menu->getChildren();
        if (!menuChildren)
            return;

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
    }
};
