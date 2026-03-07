#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class $modify(SettingsBTN, EditorPauseLayer)
{
    void btnSettings(CCObject *)
    {
        geode::openSettingsPopup(Mod::get());
    };

    bool init(LevelEditorLayer *lel)
    {
        if (!EditorPauseLayer::init(lel))
            return false;

        bool shortcut = Mod::get()->getSettingValue<bool>("shortcut");

        if (shortcut)
        {
            auto btnSprite = CCSprite::create("btnSprite.png"_spr);
            auto menu = this->getChildByID("guidelines-menu");
            if (menu && btnSprite)
            {
                auto btn = CCMenuItemSpriteExtra::create(
                    btnSprite, this, menu_selector(SettingsBTN::btnSettings));
                btn->setID("settings-button"_spr);
                btn->setZOrder(10);
                menu->addChild(btn);
                menu->updateLayout();
            }
        }

        return true;
    }
};
