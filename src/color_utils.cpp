#include "color_utils.hpp"
#include <cmath>

float g_hue = 0;

void HSVtoRGB(float &r, float &g, float &b, float h, float s, float v)
{
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;

    if (h < 60)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300)
    {
        r = x;
        g = 0;
        b = c;
    }
    else
    {
        r = c;
        g = 0;
        b = x;
    }

    r += m;
    g += m;
    b += m;
}

cocos2d::_ccColor3B getRainbow(float offset, float saturation, float value)
{
    float r, g, b;
    HSVtoRGB(r, g, b, fmod(g_hue + offset, 360), saturation / 100.0, value / 100.0);

    cocos2d::_ccColor3B out;
    out.r = static_cast<unsigned char>(r * 255);
    out.g = static_cast<unsigned char>(g * 255);
    out.b = static_cast<unsigned char>(b * 255);
    return out;
}

void updateHue(const RainbowSettings &settings)
{
    if (g_hue >= 360)
    {
        g_hue = 0;
    }
    else
    {
        g_hue += settings.superSpeed ? 10.0f : static_cast<float>(settings.speed) / 10.0f;
    }
}
