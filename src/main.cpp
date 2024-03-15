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
cocos2d::_ccColor3B getRainbow(float offset, float saturation)
{
    float R;
    float G;
    float B;

    float hue = fmod(g + offset, 360);
    float sat = saturation / 100.0;
    float vc = 1;
    HSVtoRGB(R, G, B, hue, sat, vc);

    cocos2d::_ccColor3B out;
    out.r = R * 255;
    out.g = G * 255;
    out.b = B * 255;
    return out;
}

class $modify(PlayerObject){void flashPlayer(float p0, float p1,
                                             cocos2d::ccColor3B mainColor,
                                             cocos2d::ccColor3B secondColor){
    // neutralised kappa
}};

//-----------Player Rainbow-----------\\

class $modify(PlayLayer)
{
    CCSprite *progressBar;
    CCLabelBMFont *percentLabel;

    void postUpdate(float p0)
    {
        float speed = Mod::get()->getSettingValue<double>("speed");
        float saturation = Mod::get()->getSettingValue<double>("saturation");
        auto rainbowColor = getRainbow(0, saturation);
        auto rainbowColor2 = getRainbow(180, saturation);
        auto rainbowColor3 = getRainbow(90, saturation);
        bool enable = Mod::get()->getSettingValue<bool>("enable");
        bool glow = Mod::get()->getSettingValue<bool>("glow");
        int preset = Mod::get()->getSettingValue<int64_t>("preset");
        bool sync = Mod::get()->getSettingValue<bool>("sync");
        bool wave = Mod::get()->getSettingValue<bool>("wave");
        bool bar = Mod::get()->getSettingValue<bool>("bar");
        bool trail = Mod::get()->getSettingValue<bool>("trail");

        if (g >= 360)
        {
            g = 0;
        }
        else
        {
            g += speed / 10;
        }

        if (enable == true)
        {
            if (m_fields->progressBar == nullptr ||
                m_fields->percentLabel == nullptr)
            {
                for (size_t i = 0; i < this->getChildrenCount(); i++)
                {
                    auto obj = this->getChildren()->objectAtIndex(i);
                    if (Utils::getNodeName(obj) ==
                            "cocos2d::CCLabelBMFont" &&
                        m_fields->percentLabel == nullptr)
                    {
                        auto labelTest =
                            static_cast<CCLabelBMFont *>(obj);
                        if (strlen(labelTest->getString()) < 6)
                        {
                            m_fields->percentLabel = labelTest;
                        }
                    }
                    else if (Utils::getNodeName(obj) ==
                                 "cocos2d::CCSprite" &&
                             m_fields->progressBar == nullptr)
                    {
                        m_fields->progressBar =
                            static_cast<CCSprite *>(obj);
                    }
                }
            }
            else
            {
                if (bar == true)
                {
                    m_fields->progressBar->setChildColor(rainbowColor);
                }
            }

            if (glow == true)
            {
                if (sync == true)
                {
                    m_player1->m_glowColor = rainbowColor;
                    m_player2->m_glowColor = rainbowColor;
                    m_player1->updateGlowColor();
                    m_player2->updateGlowColor();
                }
                else
                {
                    m_player1->m_glowColor = rainbowColor;
                    m_player2->m_glowColor = rainbowColor2;
                    m_player1->updateGlowColor();
                    m_player2->updateGlowColor();
                }
            }

            if (trail == true)
            {
                // TODO: Add trail color
            }
            if (wave == true)
            {
                if (m_player1->m_waveTrail)
                {
                    m_player1->m_waveTrail->setColor(rainbowColor);
                }

                if (m_player2->m_waveTrail)
                {
                    m_player2->m_waveTrail->setColor(rainbowColor2);
                }
            }
            bool isCube = !m_player1->m_isShip && !m_player1->m_isBall &&
                          !m_player1->m_isBird && !m_player1->m_isDart &&
                          !m_player1->m_isRobot && !m_player1->m_isSpider &&
                          !m_player1->m_isSwing;

            if (isCube || m_player1->m_isShip || m_player1->m_isBall ||
                m_player1->m_isBird || m_player1->m_isDart ||
                m_player1->m_isRobot || m_player1->m_isSpider ||
                m_player1->m_isSwing)
            {
                ccColor3B col1 = rainbowColor;
                ccColor3B col2 = rainbowColor2;

                if (preset == 1)
                {
                    if (sync)
                    {
                        static_cast<cocos2d::CCSprite *>(m_player1)
                            ->setColor(col1);
                        static_cast<cocos2d::CCSprite *>(
                            m_player1->m_iconSpriteSecondary)
                            ->setColor(col1);
                        static_cast<cocos2d::CCSprite *>(m_player2)
                            ->setColor(col2);
                        static_cast<cocos2d::CCSprite *>(
                            m_player2->m_iconSpriteSecondary)
                            ->setColor(col2);
                    }
                    else
                    {
                        static_cast<cocos2d::CCSprite *>(m_player1)
                            ->setColor(col1);
                        static_cast<cocos2d::CCSprite *>(
                            m_player1->m_iconSpriteSecondary)
                            ->setColor(col2);
                        static_cast<cocos2d::CCSprite *>(m_player2)
                            ->setColor(col1);
                        static_cast<cocos2d::CCSprite *>(
                            m_player2->m_iconSpriteSecondary)
                            ->setColor(col2);
                    }
                }
                else if (preset == 2)
                {
                    static_cast<cocos2d::CCSprite *>(m_player1)->setColor(
                        col1);
                    static_cast<cocos2d::CCSprite *>(m_player2)->setColor(
                        col2);
                }
                else if (preset == 3)
                {
                    static_cast<cocos2d::CCSprite *>(
                        m_player1->m_iconSpriteSecondary)
                        ->setColor(col2);
                    static_cast<cocos2d::CCSprite *>(
                        m_player2->m_iconSpriteSecondary)
                        ->setColor(col2);
                }
            }
        }

        PlayLayer::postUpdate(p0);
    }
};

//-----------Editor Rainbow-----------\\


#include <Geode/modify/LevelEditorLayer.hpp>
class $modify(LevelEditorLayer){

    void postUpdate(float p0){

        auto rainbowColor = getRainbow(0, 100);
auto rainbowColor2 = getRainbow(180, 100);
auto rainbowColor3 = getRainbow(90, 100);
int preset = Mod::get()->getSettingValue<int64_t>("preset");
bool sync = Mod::get()->getSettingValue<bool>("sync");
bool enabled = Mod::get()->getSettingValue<bool>("editorEnable");
float speed = Mod::get()->getSettingValue<double>("speed");

if (g >= 360)
{
    g = 0;
}
else
{
    g += speed / 10;
}
if (enabled)
{

    bool isCube = !m_player1->m_isShip && !m_player1->m_isBall && !m_player1->m_isBird && !m_player1->m_isDart && !m_player1->m_isRobot && !m_player1->m_isSpider && !m_player1->m_isSwing;

    if (isCube || m_player1->m_isShip || m_player1->m_isBall ||
        m_player1->m_isBird || m_player1->m_isDart || m_player1->m_isRobot ||
        m_player1->m_isSpider || m_player1->m_isSwing)
    {
        ccColor3B col1 = rainbowColor;
        ccColor3B col2 = rainbowColor2;

        if (preset == 1)
        {
            if (sync)
            {
                static_cast<cocos2d::CCSprite *>(m_player1)->setColor(col1);
                static_cast<cocos2d::CCSprite *>(
                    m_player1->m_iconSpriteSecondary)
                    ->setColor(col1);
                static_cast<cocos2d::CCSprite *>(m_player2)->setColor(col2);
                static_cast<cocos2d::CCSprite *>(
                    m_player2->m_iconSpriteSecondary)
                    ->setColor(col2);
            }
            else
            {
                static_cast<cocos2d::CCSprite *>(m_player1)->setColor(col1);
                static_cast<cocos2d::CCSprite *>(
                    m_player1->m_iconSpriteSecondary)
                    ->setColor(col2);
                static_cast<cocos2d::CCSprite *>(m_player2)->setColor(col1);
                static_cast<cocos2d::CCSprite *>(
                    m_player2->m_iconSpriteSecondary)
                    ->setColor(col2);
            }
        }
        else if (preset == 2)
        {
            static_cast<cocos2d::CCSprite *>(m_player1)->setColor(col1);
            static_cast<cocos2d::CCSprite *>(m_player2)->setColor(col2);
        }
        else if (preset == 3)
        {
            static_cast<cocos2d::CCSprite *>(m_player1->m_iconSpriteSecondary)
                ->setColor(col2);
            static_cast<cocos2d::CCSprite *>(m_player2->m_iconSpriteSecondary)
                ->setColor(col2);
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
    void btnSettings(CCObject *) { geode::openSettingsPopup(Mod::get()); };

    void customSetup()
    {
        bool shortcut = Mod::get()->getSettingValue<bool>("shortcut");
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto bottomRightPos =
            ccp((winSize.width / 2) - 41, (winSize.height / 2) - 181);

        PauseLayer::customSetup();
        auto btnSprite = CCSprite::create("btnSprite.png"_spr);
        btnSprite->setScale(0.350f);
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

//-----------Particle Rainbow-----------\\


#include <Geode/modify/CCParticleSystem.hpp>
class $modify(MyParticleSystem, CCParticleSystem){
    auto getPParticles(){
        return m_pParticles;
}

void forceColor(const ccColor4F &col)
{
    this->CCParticleSystem::setStartColor(col);
    this->CCParticleSystem::setEndColor(col);
}
}
;

class $modify(HookedPlayerObject, PlayerObject)
{

    bool enable = Mod::get()->getSettingValue<bool>("particles");

    bool init(int p0, int p1, GJBaseGameLayer *p2, cocos2d::CCLayer *p3, bool p4)
    {
        if (!PlayerObject::init(p0, p1, p2, p3, p4))
            return false;

        this->schedule(schedule_selector(HookedPlayerObject::updateGlowColorD));

        return true;
    }

    void updateGlowColorD(float dt)
    {
        constexpr float interval = 0.2f;

        ccColor3B rainbowColor = getRainbow(0, 100);
        auto col = ccc4FFromccc3B(rainbowColor);
        if (enable)
        {
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_unk6dc))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_trailingParticles))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_shipClickParticles))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_unk6e8))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_ufoClickParticles))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_robotBurstParticles))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_unk6f4))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_swingBurstParticles1))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_swingBurstParticles2))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_unk704))->forceColor(col);
            static_cast<MyParticleSystem *>(static_cast<CCParticleSystem *>(m_unk708))->forceColor(col);
        }
    }

    void updateGlowColor()
    {
        PlayerObject::updateGlowColor();
        this->updateGlowColorD(0.f);
    }
};