#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <chrono>
#include <functional>
#include <map>
#include <string>

#include "Utils.hpp"

using namespace geode::prelude;
using namespace std::chrono;

void HSVtoRGB(float &fR, float &fG, float &fB, float &fH, float &fS,
              float &fV)
{
    float fC = fV * fS; // Chroma
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = fV - fC;

    if (0 <= fHPrime && fHPrime < 1)
    {
        fR = fC;
        fG = fX;
        fB = 0;
    }
    else if (1 <= fHPrime && fHPrime < 2)
    {
        fR = fX;
        fG = fC;
        fB = 0;
    }
    else if (2 <= fHPrime && fHPrime < 3)
    {
        fR = 0;
        fG = fC;
        fB = fX;
    }
    else if (3 <= fHPrime && fHPrime < 4)
    {
        fR = 0;
        fG = fX;
        fB = fC;
    }
    else if (4 <= fHPrime && fHPrime < 5)
    {
        fR = fX;
        fG = 0;
        fB = fC;
    }
    else if (5 <= fHPrime && fHPrime < 6)
    {
        fR = fC;
        fG = 0;
        fB = fX;
    }
    else
    {
        fR = 0;
        fG = 0;
        fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;
}

float g = 0;

cocos2d::_ccColor3B getRainbow(float offset, float saturation, float value)
{
    float R;
    float G;
    float B;

    float hue = fmod(g + offset, 360);
    float sat = saturation / 100.0;
    float vc = value / 100.0;
    HSVtoRGB(R, G, B, hue, sat, vc);

    cocos2d::_ccColor3B out;
    out.r = R * 255;
    out.g = G * 255;
    out.b = B * 255;
    return out;
}

cocos2d::_ccColor4F getRainbow4B(float offset, float saturation, float value)
{
    float R;
    float G;
    float B;

    float hue = fmod(g + offset, 360);
    float sat = saturation / 100.0;
    float vc = value / 100.0;
    HSVtoRGB(R, G, B, hue, sat, vc);

    cocos2d::_ccColor4F out;
    out.r = R * 255;
    out.g = G * 255;
    out.b = B * 255;
    out.a = 255;
    return out;
}

class $modify(PlayerObject){
    void flashPlayer(float p0, float p1, cocos2d::ccColor3B mainColor, cocos2d::ccColor3B secondColor){
        // neutralised kappa
    }};

//-----------Player Rainbow-----------\\

class $modify(PlayLayer){

    void postUpdate(float p0){
        float speed = Mod::get()->getSettingValue<double>("speed");
float saturation = Mod::get()->getSettingValue<double>("saturation");
int offset_color_p1 = Mod::get()->getSettingValue<int64_t>("offset_color_p1");
int offset_color_p2 = Mod::get()->getSettingValue<int64_t>("offset_color_p2");
bool enable = Mod::get()->getSettingValue<bool>("enable");
bool glow = Mod::get()->getSettingValue<bool>("glow");
int preset = Mod::get()->getSettingValue<int64_t>("preset");
int playerPreset = Mod::get()->getSettingValue<int64_t>("playerPreset");
bool sync = Mod::get()->getSettingValue<bool>("sync");
bool wave = Mod::get()->getSettingValue<bool>("wave");
float brightness = Mod::get()->getSettingValue<double>("brightness");
bool superSpeed = Mod::get()->getSettingValue<bool>("superSpeed");

if (superSpeed)
{
    if (g >= 360)
    {
        g = 0;
    }
    else
    {
        g += 10;
    }
}
else
{
    if (g >= 360)
    {
        g = 0;
    }
    else
    {
        g += speed / 10;
    }
}

if (enable == true)
{

    auto rainbowColorMainP1 = getRainbow(offset_color_p1, saturation, brightness);
    auto rainbowColorInvertedP1 = getRainbow(offset_color_p1 + 180, saturation, brightness);

    auto rainbowColorMainP2 = getRainbow(offset_color_p2, saturation, brightness);
    auto rainbowColorInvertedP2 = getRainbow(offset_color_p2 + 180, saturation, brightness);

    auto rainbowColor = getRainbow(0, saturation, brightness);
    auto rainbowColor2 = getRainbow(180, saturation, brightness);

    // if (m_fields->progressBar == nullptr ||
    // 	m_fields->percentLabel == nullptr)
    // {
    // 	for (size_t i = 0; i < this->getChildrenCount(); i++)
    // 	{
    // 		auto obj = this->getChildren()->objectAtIndex(i);
    // 		if (Utils::getNodeName(obj) ==
    // 				"cocos2d::CCLabelBMFont" &&
    // 			m_fields->percentLabel == nullptr)
    // 		{
    // 			auto labelTest =
    // 				static_cast<CCLabelBMFont *>(obj);
    // 			if (strlen(labelTest->getString()) < 6)
    // 			{
    // 				m_fields->percentLabel = labelTest;
    // 			}
    // 		}
    // 		else if (Utils::getNodeName(obj) ==
    // 					 "cocos2d::CCSprite" &&
    // 				 m_fields->progressBar == nullptr)
    // 		{
    // 			m_fields->progressBar =
    // 				static_cast<CCSprite *>(obj);
    // 		}
    // 	}
    // }
    // else
    // {
    // 	if (bar == true)
    // 	{
    // 		m_fields->progressBar->setChildColor(rainbowColor);
    // 	}
    // }

    if (glow == true)
    {
        if (sync == true)
        {
            m_player1->m_glowColor = rainbowColorMainP1;
            m_player2->m_glowColor = rainbowColorMainP2;
            m_player1->updateGlowColor();
            m_player2->updateGlowColor();
        }
        else
        {
            m_player1->m_glowColor = rainbowColorInvertedP1;
            m_player2->m_glowColor = rainbowColorInvertedP2;
            m_player1->updateGlowColor();
            m_player2->updateGlowColor();
        }
    }
    if (wave == true)
    {

        if (playerPreset == 1)
        {
            if (m_player1->m_waveTrail)
            {
                m_player1->m_waveTrail->setColor(rainbowColorMainP1);
            }

            if (m_player2->m_waveTrail)
            {
                m_player2->m_waveTrail->setColor(rainbowColorMainP2);
            }
        }
        else if (playerPreset == 2)
        {
            if (m_player1->m_waveTrail)
            {
                m_player1->m_waveTrail->setColor(rainbowColorMainP1);
            }
        }
        else if (playerPreset == 3)
        {
            if (m_player2->m_waveTrail)
            {
                m_player2->m_waveTrail->setColor(rainbowColorMainP2);
            }
        }
    }

    bool isCube = !m_player1->m_isShip && !m_player1->m_isBall && !m_player1->m_isBird && !m_player1->m_isDart && !m_player1->m_isRobot && !m_player1->m_isSpider && !m_player1->m_isSwing;

    if (isCube || m_player1->m_isShip || m_player1->m_isBall || m_player1->m_isBird || m_player1->m_isDart || m_player1->m_isRobot || m_player1->m_isSpider || m_player1->m_isSwing)
    {

        if (preset == 1)
        {
            if (sync)
            {

                //  "1 = Both player 1 and are rainbow, 2 = Only player 1 is rainbow, 3 = Only player 2 is rainbow",
                if (playerPreset == 1)
                {
                    m_player1->setColor(rainbowColorMainP1);
                    m_player1->setSecondColor(rainbowColorMainP1);

                    m_player2->setColor(rainbowColorMainP2);
                    m_player2->setSecondColor(rainbowColorMainP2);
                }
                else if (playerPreset == 2)
                {
                    m_player1->setColor(rainbowColorMainP1);
                    m_player1->setSecondColor(rainbowColorMainP1);
                }
                else if (playerPreset == 3)
                {
                    m_player2->setColor(rainbowColorMainP2);
                    m_player2->setSecondColor(rainbowColorMainP2);
                }
            }
            else
            {
                if (playerPreset == 1)
                {
                    m_player1->setColor(rainbowColorMainP1);
                    m_player1->setSecondColor(rainbowColorInvertedP1);

                    m_player2->setColor(rainbowColorMainP2);
                    m_player2->setSecondColor(rainbowColorInvertedP2);
                }
                else if (playerPreset == 2)
                {
                    m_player1->setColor(rainbowColorMainP1);
                    m_player1->setSecondColor(rainbowColorInvertedP1);
                }
                else if (playerPreset == 3)
                {
                    m_player2->setColor(rainbowColorMainP2);
                    m_player2->setSecondColor(rainbowColorInvertedP2);
                }
            }
        }
        else if (preset == 2)
        {

            if (playerPreset == 1)
            {
                m_player1->setColor(rainbowColorMainP1);
                m_player2->setColor(rainbowColorMainP2);
            }
            else if (playerPreset == 2)
            {
                m_player1->setColor(rainbowColorMainP1);
            }
            else if (playerPreset == 3)
            {
                m_player2->setColor(rainbowColorMainP2);
            }
        }
        else if (preset == 3)
        {
            if (playerPreset == 1)
            {
                m_player1->setSecondColor(rainbowColorMainP1);
                m_player2->setSecondColor(rainbowColorMainP2);
            }
            else if (playerPreset == 2)
            {
                m_player1->setSecondColor(rainbowColorMainP1);
            }
            else if (playerPreset == 3)
            {
                m_player2->setSecondColor(rainbowColorMainP2);
            }
        }
    }
}

PlayLayer::postUpdate(p0);
}
}
;

//-----------Editor Rainbow-----------\\

#include <Geode/modify/LevelEditorLayer.hpp>
class $modify(LevelEditorLayer){

    void postUpdate(float p0){

        int preset = Mod::get()->getSettingValue<int64_t>("preset");
bool sync = Mod::get()->getSettingValue<bool>("sync");
bool enabled = Mod::get()->getSettingValue<bool>("editorEnable");
float speed = Mod::get()->getSettingValue<double>("speed");
int playerPreset = Mod::get()->getSettingValue<int64_t>("playerPreset");
float saturation = Mod::get()->getSettingValue<double>("saturation");
int offset_color_p1 = Mod::get()->getSettingValue<int64_t>("offset_color_p1");
int offset_color_p2 = Mod::get()->getSettingValue<int64_t>("offset_color_p2");
float brightness = Mod::get()->getSettingValue<double>("brightness");
bool superSpeed = Mod::get()->getSettingValue<bool>("superSpeed");

if (superSpeed)
{
    if (g >= 360)
    {
        g = 0;
    }
    else
    {
        g += 10;
    }
}
else
{
    if (g >= 360)
    {
        g = 0;
    }
    else
    {
        g += speed / 10;
    }
}
if (enabled)
{

    auto rainbowColorMainP1 = getRainbow(offset_color_p1, saturation, brightness);
    auto rainbowColorInvertedP1 = getRainbow(offset_color_p1 + 180, saturation, brightness);

    auto rainbowColorMainP2 = getRainbow(offset_color_p2, saturation, brightness);
    auto rainbowColorInvertedP2 = getRainbow(offset_color_p2 + 180, saturation, brightness);

    auto rainbowColor = getRainbow(0, saturation, brightness);
    auto rainbowColor2 = getRainbow(180, saturation, brightness);

    bool isCube = !m_player1->m_isShip && !m_player1->m_isBall && !m_player1->m_isBird && !m_player1->m_isDart && !m_player1->m_isRobot && !m_player1->m_isSpider && !m_player1->m_isSwing;

    if (isCube || m_player1->m_isShip || m_player1->m_isBall ||
        m_player1->m_isBird || m_player1->m_isDart || m_player1->m_isRobot ||
        m_player1->m_isSpider || m_player1->m_isSwing)
    {

        if (preset == 1)
        {
            if (sync)
            {

                //  "1 = Both player 1 and are rainbow, 2 = Only player 1 is rainbow, 3 = Only player 2 is rainbow",
                if (playerPreset == 1)
                {
                    m_player1->setColor(rainbowColorMainP1);
                    m_player1->setSecondColor(rainbowColorMainP1);

                    m_player2->setColor(rainbowColorMainP2);
                    m_player2->setSecondColor(rainbowColorMainP2);
                }
                else if (playerPreset == 2)
                {
                    m_player1->setColor(rainbowColorMainP1);
                    m_player1->setSecondColor(rainbowColorMainP1);
                }
                else if (playerPreset == 3)
                {
                    m_player2->setColor(rainbowColorMainP2);
                    m_player2->setSecondColor(rainbowColorMainP2);
                }
            }
            else
            {
                if (playerPreset == 1)
                {
                    m_player1->setColor(rainbowColorMainP1);
                    m_player1->setSecondColor(rainbowColorInvertedP1);

                    m_player2->setColor(rainbowColorMainP2);
                    m_player2->setSecondColor(rainbowColorInvertedP2);
                }
                else if (playerPreset == 2)
                {
                    m_player1->setColor(rainbowColorMainP1);
                    m_player1->setSecondColor(rainbowColorInvertedP1);
                }
                else if (playerPreset == 3)
                {
                    m_player2->setColor(rainbowColorMainP2);
                    m_player2->setSecondColor(rainbowColorInvertedP2);
                }
            }
        }
        else if (preset == 2)
        {

            if (playerPreset == 1)
            {
                m_player1->setColor(rainbowColorMainP1);
                m_player2->setColor(rainbowColorMainP2);
            }
            else if (playerPreset == 2)
            {
                m_player1->setColor(rainbowColorMainP1);
            }
            else if (playerPreset == 3)
            {
                m_player2->setColor(rainbowColorMainP2);
            }
        }
        else if (preset == 3)
        {
            if (playerPreset == 1)
            {
                m_player1->setSecondColor(rainbowColorMainP1);
                m_player2->setSecondColor(rainbowColorMainP2);
            }
            else if (playerPreset == 2)
            {
                m_player1->setSecondColor(rainbowColorMainP1);
            }
            else if (playerPreset == 3)
            {
                m_player2->setSecondColor(rainbowColorMainP2);
            }
        }
    }
}
LevelEditorLayer::postUpdate(p0);
}
}
;

//-----------PauseButton Rainbow-----------\\

#include <Geode/modify/PauseLayer.hpp>
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

#include <Geode/modify/EditorPauseLayer.hpp>
//-----------Editor PauseButton Rainbow-----------\\

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