#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>
#include <chrono>
#include <functional>
#include <map>
#include <string>

using namespace geode::prelude;
using namespace std::chrono;

// Structure to hold mod settings
struct RainbowSettings {
    double speed;
    double saturation;
    double brightness;
    int64_t offset_color_p1;
    int64_t offset_color_p2;
    bool enable;
    bool glow;
    int64_t preset;
    int64_t playerPreset;
    bool sync;
    bool wave;
    bool superSpeed;
    bool pastel;
    bool editorEnable; // Added for editor specific enable
};

// Fetches all settings at once
RainbowSettings getModSettings() {
    auto mod = Mod::get();
    return {
        mod->getSettingValue<double>("speed"),
        mod->getSettingValue<double>("saturation"),
        mod->getSettingValue<double>("brightness"),
        mod->getSettingValue<int64_t>("offset_color_p1"),
        mod->getSettingValue<int64_t>("offset_color_p2"),
        mod->getSettingValue<bool>("enable"),
        mod->getSettingValue<bool>("glow"),
        mod->getSettingValue<int64_t>("preset"),
        mod->getSettingValue<int64_t>("playerPreset"),
        mod->getSettingValue<bool>("sync"),
        mod->getSettingValue<bool>("wave"),
        mod->getSettingValue<bool>("superSpeed"),
        mod->getSettingValue<bool>("pastel"),
        mod->getSettingValue<bool>("editorEnable") // Fetch editor setting
    };
}


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

float g_hue = 0; // Renamed global variable for clarity

cocos2d::_ccColor3B getRainbow(float offset, float saturation, float value)
{
    float r, g, b;
    // Use g_hue instead of ::g
    HSVtoRGB(r, g, b, fmod(g_hue + offset, 360), saturation / 100.0, value / 100.0);

    cocos2d::_ccColor3B out;
    out.r = static_cast<unsigned char>(r * 255); // Use static_cast for clarity
    out.g = static_cast<unsigned char>(g * 255);
    out.b = static_cast<unsigned char>(b * 255);
    return out;
}

// Removed getRainbow4B as it wasn't used

// Helper function to apply colors to a player
void applyRainbowColors(PlayerObject* player, bool isPlayer1, RainbowSettings& settings, const cocos2d::_ccColor3B& mainColor, const cocos2d::_ccColor3B& invertedColor) {
    if (!player) return;

    int64_t playerPreset = settings.playerPreset;
    bool applyToThisPlayer = (playerPreset == 1) || (isPlayer1 && playerPreset == 2) || (!isPlayer1 && playerPreset == 3);

    if (!applyToThisPlayer) return;

    // Apply Wave Trail Color
    if (settings.wave && player->m_waveTrail) {
        player->m_waveTrail->setColor(mainColor);
    }

    // Apply Player Colors based on preset
    if (settings.preset == 1) // Both colors
    {
        player->setColor(mainColor);
        player->setSecondColor(settings.sync ? mainColor : invertedColor);
    }
    else if (settings.preset == 2) // Primary color only
    {
        player->setColor(mainColor);
    }
    else if (settings.preset == 3) // Secondary color only
    {
        player->setSecondColor(mainColor);
    }

    // Apply Glow Color
    if (settings.glow) {
        player->m_glowColor = settings.sync ? mainColor : invertedColor;
        player->updateGlowColor();
    }
}

// Helper function to update hue
void updateHue(const RainbowSettings& settings) {
     if (g_hue >= 360)
    {
        g_hue = 0;
    }
    else
    {
        // Use settings directly
        g_hue += settings.superSpeed ? 10.0f : static_cast<float>(settings.speed) / 10.0f;
    }
}


class $modify(PlayerObject)
{
    // Removed flashPlayer method (already done)
};

class $modify(MyPlayLayer, PlayLayer) // Added a name for clarity
{
    void postUpdate(float p0)
    {
        PlayLayer::postUpdate(p0); // Call base function first

        auto settings = getModSettings();
        updateHue(settings); // Update global hue based on settings

        if (settings.enable)
        {
            if (settings.pastel)
            {
                settings.saturation = 50;
                settings.brightness = 90;
            }

            auto mainColorP1 = getRainbow(settings.offset_color_p1, settings.saturation, settings.brightness);
            auto invertedColorP1 = getRainbow(settings.offset_color_p1 + 180, settings.saturation, settings.brightness);
            auto mainColorP2 = getRainbow(settings.offset_color_p2, settings.saturation, settings.brightness);
            auto invertedColorP2 = getRainbow(settings.offset_color_p2 + 180, settings.saturation, settings.brightness);

            // Apply colors using the helper function
            applyRainbowColors(m_player1, true, settings, mainColorP1, invertedColorP1);
            applyRainbowColors(m_player2, false, settings, mainColorP2, invertedColorP2);
        }
    }
};

class $modify(MyLevelEditorLayer, LevelEditorLayer) // Added a name for clarity
{
    void postUpdate(float p0)
    {
        LevelEditorLayer::postUpdate(p0); // Call base function first

        auto settings = getModSettings();
         // Use editorEnable setting here
        if (!settings.editorEnable) return;

        updateHue(settings); // Update global hue based on settings

        // No pastel setting check in editor? Assuming same logic as PlayLayer for now.
        // If editor should ignore pastel, add a check here.
        if (settings.pastel)
        {
            settings.saturation = 50;
            settings.brightness = 90;
        }

        auto mainColorP1 = getRainbow(settings.offset_color_p1, settings.saturation, settings.brightness);
        auto invertedColorP1 = getRainbow(settings.offset_color_p1 + 180, settings.saturation, settings.brightness);
        auto mainColorP2 = getRainbow(settings.offset_color_p2, settings.saturation, settings.brightness);
        auto invertedColorP2 = getRainbow(settings.offset_color_p2 + 180, settings.saturation, settings.brightness);

        // Apply colors using the helper function
        // Note: Editor might only have m_player1, check needed if m_player2 usage is intended
        applyRainbowColors(m_player1, true, settings, mainColorP1, invertedColorP1);
        if(m_player2) { // Check if player 2 exists in editor context
             applyRainbowColors(m_player2, false, settings, mainColorP2, invertedColorP2);
        }
    }
};

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

        // Fetch setting only once
        bool shortcut = Mod::get()->getSettingValue<bool>("shortcut");

        if (shortcut) // Check before creating sprites/menus
        {
            auto btnSprite = CCSprite::create("btnSprite.png"_spr);
            auto menu = this->getChildByID("guidelines-menu");
             if (menu && btnSprite) { // Check if menu and sprite exist
                auto btn = CCMenuItemSpriteExtra::create(
                    btnSprite, this, menu_selector(SettingsBTN::btnSettings));
                btn->setID("settings-button"_spr);
                btn->setZOrder(10); // Consider if ZOrder is necessary if it's the only item added
                menu->addChild(btn);
                menu->updateLayout();
             }
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
        PauseLayer::customSetup(); // Call base function first

        // Fetch setting only once
        bool shortcut = Mod::get()->getSettingValue<bool>("shortcut");

        // Removed unused winSize and bottomRightPos variables

        if (shortcut) // Check before creating sprites/menus
        {
             auto btnSprite = CCSprite::create("btnSprite.png"_spr);
             auto menu = this->getChildByID("right-button-menu");
             if (menu && btnSprite) { // Check if menu and sprite exist
                auto btn = CCMenuItemSpriteExtra::create(
                    btnSprite, this, menu_selector(OpenSettings::btnSettings));
                btn->setID("settings-button"_spr);
                btn->setZOrder(10); // Consider if ZOrder is necessary
                menu->addChild(btn);
                menu->updateLayout();
             }
        }
    };
};