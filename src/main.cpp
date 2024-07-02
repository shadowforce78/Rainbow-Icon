#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <chrono>
#include <functional>
#include <map>
#include <string>

using namespace geode::prelude;
using namespace std::chrono;

void HSVtoRGB(float &r, float &g, float &b, float h, float s, float v)
{
    float c = v * s;
    float hPrime = fmod(h / 60.0, 6);
    float x = c * (1 - fabs(fmod(hPrime, 2) - 1));
    float m = v - c;

    if (0 <= hPrime && hPrime < 1)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (1 <= hPrime && hPrime < 2)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (2 <= hPrime && hPrime < 3)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (3 <= hPrime && hPrime < 4)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (4 <= hPrime && hPrime < 5)
    {
        r = x;
        g = 0;
        b = c;
    }
    else if (5 <= hPrime && hPrime < 6)
    {
        r = c;
        g = 0;
        b = x;
    }
    else
    {
        r = 0;
        g = 0;
        b = 0;
    }

    r += m;
    g += m;
    b += m;
}

float g = 0;

cocos2d::_ccColor3B getRainbow(float offset, float saturation, float value)
{
    float r, g, b;
    HSVtoRGB(r, g, b, fmod(::g + offset, 360), saturation / 100.0, value / 100.0);

    cocos2d::_ccColor3B out;
    out.r = r * 255;
    out.g = g * 255;
    out.b = b * 255;
    return out;
}

cocos2d::_ccColor4F getRainbow4B(float offset, float saturation, float value)
{
    float r, g, b;
    HSVtoRGB(r, g, b, fmod(::g + offset, 360), saturation / 100.0, value / 100.0);


    cocos2d::_ccColor4F out;
    out.r = r * 255;
    out.g = g * 255;
    out.b = b * 255;
    out.a = 255;
    return out;
}

class $modify(PlayerObject){
    void flashPlayer(float p0, float p1, cocos2d::ccColor3B mainColor, cocos2d::ccColor3B secondColor){
        // neutralised kappa
    }};

class $modify(PlayLayer){
    void postUpdate(float p0){
        auto mod = Mod::get();
auto speed = mod->getSettingValue<double>("speed");
auto saturation = mod->getSettingValue<double>("saturation");
auto brightness = mod->getSettingValue<double>("brightness");
auto offset_color_p1 = mod->getSettingValue<int64_t>("offset_color_p1");
auto offset_color_p2 = mod->getSettingValue<int64_t>("offset_color_p2");
auto enable = mod->getSettingValue<bool>("enable");
auto glow = mod->getSettingValue<bool>("glow");
auto preset = mod->getSettingValue<int64_t>("preset");
auto playerPreset = mod->getSettingValue<int64_t>("playerPreset");
auto sync = mod->getSettingValue<bool>("sync");
auto wave = mod->getSettingValue<bool>("wave");
auto superSpeed = mod->getSettingValue<bool>("superSpeed");
auto pastel = mod->getSettingValue<bool>("pastel");

if (::g >= 360)
{
    ::g = 0;
}
else
{
    ::g += superSpeed ? 10 : speed / 10;
}

if (enable)
{

    if (pastel)
    {
        saturation = 50;
        brightness = 90;
    }

    auto mainColorP1 = getRainbow(offset_color_p1, saturation, brightness);
    auto invertedColorP1 = getRainbow(offset_color_p1 + 180, saturation, brightness);
    auto mainColorP2 = getRainbow(offset_color_p2, saturation, brightness);
    auto invertedColorP2 = getRainbow(offset_color_p2 + 180, saturation, brightness);

    if (glow)
    {
        auto glowColor1 = sync ? mainColorP1 : invertedColorP1;
        auto glowColor2 = sync ? mainColorP2 : invertedColorP2;

        m_player1->m_glowColor = glowColor1;
        m_player2->m_glowColor = glowColor2;
        m_player1->updateGlowColor();
        m_player2->updateGlowColor();
    }

    if (wave)
    {
        if (playerPreset == 1 || playerPreset == 2)
        {
            if (m_player1->m_waveTrail)
                m_player1->m_waveTrail->setColor(mainColorP1);
        }
        if (playerPreset == 1 || playerPreset == 3)
        {
            if (m_player2->m_waveTrail)
                m_player2->m_waveTrail->setColor(mainColorP2);
        }
    }

    bool isCube = !m_player1->m_isShip && !m_player1->m_isBall && !m_player1->m_isBird && !m_player1->m_isDart && !m_player1->m_isRobot && !m_player1->m_isSpider && !m_player1->m_isSwing;
    if (isCube || m_player1->m_isShip || m_player1->m_isBall || m_player1->m_isBird || m_player1->m_isDart || m_player1->m_isRobot || m_player1->m_isSpider || m_player1->m_isSwing)
    {
        if (preset == 1)
        {
            if (sync)
            {
                auto color1 = playerPreset != 3 ? mainColorP1 : mainColorP2;
                auto color2 = playerPreset != 2 ? mainColorP2 : mainColorP1;

                if (playerPreset == 1 || playerPreset == 2)
                {
                    m_player1->setColor(color1);
                    m_player1->setSecondColor(color1);
                }
                if (playerPreset == 1 || playerPreset == 3)
                {
                    m_player2->setColor(color2);
                    m_player2->setSecondColor(color2);
                }
            }
            else
            {
                auto color1 = playerPreset != 3 ? mainColorP1 : mainColorP2;
                auto color2 = playerPreset != 2 ? mainColorP2 : mainColorP1;

                if (playerPreset == 1 || playerPreset == 2)
                {
                    m_player1->setColor(color1);
                    m_player1->setSecondColor(invertedColorP1);
                }
                if (playerPreset == 1 || playerPreset == 3)
                {
                    m_player2->setColor(color2);
                    m_player2->setSecondColor(invertedColorP2);
                }
            }
        }
        else if (preset == 2)
        {
            if (playerPreset == 1 || playerPreset == 2)
            {
                m_player1->setColor(mainColorP1);
            }
            if (playerPreset == 1 || playerPreset == 3)
            {
                m_player2->setColor(mainColorP2);
            }
        }
        else if (preset == 3)
        {
            if (playerPreset == 1 || playerPreset == 2)
            {
                m_player1->setSecondColor(mainColorP1);
            }
            if (playerPreset == 1 || playerPreset == 3)
            {
                m_player2->setSecondColor(mainColorP2);
            }
        }
    }
}

PlayLayer::postUpdate(p0);
}
}
;

class $modify(LevelEditorLayer){
    void postUpdate(float p0){
        auto mod = Mod::get();
auto speed = mod->getSettingValue<double>("speed");
auto saturation = mod->getSettingValue<double>("saturation");
auto brightness = mod->getSettingValue<double>("brightness");
auto offset_color_p1 = mod->getSettingValue<int64_t>("offset_color_p1");
auto offset_color_p2 = mod->getSettingValue<int64_t>("offset_color_p2");
auto enable = mod->getSettingValue<bool>("editorEnable");
auto preset = mod->getSettingValue<int64_t>("preset");
auto playerPreset = mod->getSettingValue<int64_t>("playerPreset");
auto sync = mod->getSettingValue<bool>("sync");
auto superSpeed = mod->getSettingValue<bool>("superSpeed");
if (::g >= 360)
{
    ::g = 0;
}
else
{
    ::g += superSpeed ? 10 : speed / 10;
}

if (enable)
{

    auto mainColorP1 = getRainbow(offset_color_p1, saturation, brightness);
    auto invertedColorP1 = getRainbow(offset_color_p1 + 180, saturation, brightness);
    auto mainColorP2 = getRainbow(offset_color_p2, saturation, brightness);
    auto invertedColorP2 = getRainbow(offset_color_p2 + 180, saturation, brightness);

    bool isCube = !m_player1->m_isShip && !m_player1->m_isBall && !m_player1->m_isBird && !m_player1->m_isDart && !m_player1->m_isRobot && !m_player1->m_isSpider && !m_player1->m_isSwing;
    if (isCube || m_player1->m_isShip || m_player1->m_isBall || m_player1->m_isBird || m_player1->m_isDart || m_player1->m_isRobot || m_player1->m_isSpider || m_player1->m_isSwing)
    {
        if (preset == 1)
        {
            if (sync)
            {
                auto color1 = playerPreset != 3 ? mainColorP1 : mainColorP2;
                auto color2 = playerPreset != 2 ? mainColorP2 : mainColorP1;

                if (playerPreset == 1 || playerPreset == 2)
                {
                    m_player1->setColor(color1);
                    m_player1->setSecondColor(color1);
                }
                if (playerPreset == 1 || playerPreset == 3)
                {
                    m_player2->setColor(color2);
                    m_player2->setSecondColor(color2);
                }
            }
            else
            {
                auto color1 = playerPreset != 3 ? mainColorP1 : mainColorP2;
                auto color2 = playerPreset != 2 ? mainColorP2 : mainColorP1;

                if (playerPreset == 1 || playerPreset == 2)
                {
                    m_player1->setColor(color1);
                    m_player1->setSecondColor(invertedColorP1);
                }
                if (playerPreset == 1 || playerPreset == 3)
                {
                    m_player2->setColor(color2);
                    m_player2->setSecondColor(invertedColorP2);
                }
            }
        }
        else if (preset == 2)
        {
            if (playerPreset == 1 || playerPreset == 2)
            {
                m_player1->setColor(mainColorP1);
            }
            if (playerPreset == 1 || playerPreset == 3)
            {
                m_player2->setColor(mainColorP2);
            }
        }
        else if (preset == 3)
        {
            if (playerPreset == 1 || playerPreset == 2)
            {
                m_player1->setSecondColor(mainColorP1);
            }
            if (playerPreset == 1 || playerPreset == 3)
            {
                m_player2->setSecondColor(mainColorP2);
            }
        }
    }
}

LevelEditorLayer::postUpdate(p0);
}
}
;

class $modify(SettingsBTN, EditorPauseLayer)
{
    void btnSettings(CCObject *)
    {
        geode::openSettingsPopup(Mod::get());
    };
    bool init(LevelEditorLayer * lel)
    {
        if (!EditorPauseLayer::init(lel))
            return false;
        bool shortcut = Mod::get()->getSettingValue<bool>("shortcut");

        auto btnSprite = CCSprite::create("btnSprite.png"_spr);
        auto menu = this->getChildByID("guidelines-menu");
        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite, this, menu_selector(SettingsBTN::btnSettings));
        btn->setID("settings-button"_spr);
        btn->setZOrder(10);

        if (shortcut == true)
        {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }
};

class $modify(OpenSettings, PauseLayer)
{
    void btnSettings(CCObject *)
    {
        geode::openSettingsPopup(Mod::get());
    };

    void customSetup()
    {
        bool shortcut = Mod::get()->getSettingValue<bool>("shortcut");
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto bottomRightPos =
            ccp((winSize.width / 2) - 41, (winSize.height / 2) - 181);

        PauseLayer::customSetup();
        auto btnSprite = CCSprite::create("btnSprite.png"_spr);
        auto menu = this->getChildByID("right-button-menu");
        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite, this, menu_selector(OpenSettings::btnSettings));
        btn->setID("settings-button"_spr);
        btn->setZOrder(10);

        if (shortcut == true)
        {
            menu->addChild(btn);
            menu->updateLayout();
        }
    };
};