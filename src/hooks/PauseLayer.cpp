#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class $modify(OpenSettings, PauseLayer)
{
    void btnSettings(CCObject *)
    {
        geode::openSettingsPopup(Mod::get());
    };

    void customSetup()
    {
        PauseLayer::customSetup();

        bool shortcut = Mod::get()->getSettingValue<bool>("shortcut");

        if (shortcut)
        {
            auto btnSprite = CCSprite::create("btnSprite.png"_spr);
            auto menu = this->getChildByID("right-button-menu");
            if (menu && btnSprite)
            {
                auto btn = CCMenuItemSpriteExtra::create(
                    btnSprite, this, menu_selector(OpenSettings::btnSettings));
                btn->setID("settings-button"_spr);
                btn->setZOrder(10);
                menu->addChild(btn);
                menu->updateLayout();
            }
        }
    };
};
