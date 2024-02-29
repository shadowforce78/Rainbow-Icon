#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <chrono>

using namespace geode::prelude;
using namespace std::chrono;

void HSVtoRGB(float &fR, float &fG, float &fB, float &fH, float &fS, float &fV)
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

class $modify(PlayLayer){
    void postUpdate(float p0){
        PlayLayer::postUpdate(p0);

float speed = Mod::get()->getSettingValue<double>("speed");
float saturation = Mod::get()->getSettingValue<double>("saturation");

if (g >= 360)
{
    g = 0;
}
else
{
    g += speed / 10;
}

auto rainbowColor = getRainbow(0, saturation);
auto rainbowColor2 = getRainbow(180, saturation);
bool enable = Mod::get()->getSettingValue<bool>("enable");
bool glow = Mod::get()->getSettingValue<bool>("glow");
int preset = Mod::get()->getSettingValue<int64_t>("preset");
bool sync = Mod::get()->getSettingValue<bool>("sync");

if (enable == true)
{

    if (glow == true)
    {
        m_player1->m_glowColor = rainbowColor;
        m_player2->m_glowColor = rainbowColor2;
        m_player1->updateGlowColor();
        m_player2->updateGlowColor();
    }

    if (preset == 0)
    {
        printf("preset 0, Mod Disabled\n");
    }

    if (preset == 1)
    {
        if (sync == true)
        {
            m_player1->setColor(rainbowColor);
            m_player1->setSecondColor(rainbowColor);
            m_player2->setColor(rainbowColor2);
            m_player2->setSecondColor(rainbowColor2);
        }
        else
        {
            m_player1->setColor(rainbowColor);
            m_player1->setSecondColor(rainbowColor2);
            m_player2->setColor(rainbowColor2);
            m_player2->setSecondColor(rainbowColor);
        }

        if (m_player1->m_waveTrail)
        {
            m_player1->m_waveTrail->setColor(rainbowColor);
        }

        if (m_player2->m_waveTrail)
        {
            m_player2->m_waveTrail->setColor(rainbowColor2);
        }
    }

    if (preset == 2)
    {
        m_player1->setColor(rainbowColor);
        m_player2->setColor(rainbowColor2);
        if (m_player1->m_waveTrail)
        {
            m_player1->m_waveTrail->setColor(rainbowColor);
        }
        if (m_player2->m_waveTrail)
        {
            m_player2->m_waveTrail->setColor(rainbowColor2);
        }
    }

    if (preset == 3)
    {
        m_player1->setSecondColor(rainbowColor);
        m_player2->setSecondColor(rainbowColor2);
        if (m_player1->m_waveTrail)
        {
            m_player1->m_waveTrail->setColor(rainbowColor);
        }
        if (m_player2->m_waveTrail)
        {
            m_player2->m_waveTrail->setColor(rainbowColor2);
        }
    }
}
}
}
;
