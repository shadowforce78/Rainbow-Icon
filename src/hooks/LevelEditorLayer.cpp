#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "../settings.hpp"
#include "../color_utils.hpp"
#include "../color_apply.hpp"

using namespace geode::prelude;

class $modify(MyLevelEditorLayer, LevelEditorLayer)
{
    void postUpdate(float p0)
    {
        LevelEditorLayer::postUpdate(p0);

        auto settings = getModSettings();
        if (!settings.editorEnable)
            return;

        updateHue(settings);

        if (settings.pastel)
        {
            settings.saturation = 50;
            settings.brightness = 90;
        }

        auto mainColorP1 = getRainbow(settings.offset_color_p1, settings.saturation, settings.brightness);
        auto invertedColorP1 = getRainbow(settings.offset_color_p1 + 180, settings.saturation, settings.brightness);
        auto mainColorP2 = getRainbow(settings.offset_color_p2, settings.saturation, settings.brightness);
        auto invertedColorP2 = getRainbow(settings.offset_color_p2 + 180, settings.saturation, settings.brightness);

        applyRainbowColors(m_player1, true, settings, mainColorP1, invertedColorP1);
        if (m_player2)
        {
            applyRainbowColors(m_player2, false, settings, mainColorP2, invertedColorP2);
        }
    }
};
