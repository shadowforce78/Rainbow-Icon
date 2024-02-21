#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
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
cocos2d::_ccColor3B getRainbow(float offset)
{
	float R;
	float G;
	float B;

	float hue = fmod(g + offset, 360);
	// geode::log << hue;
	float sat = 1;
	float vc = 1;
	HSVtoRGB(R, G, B, hue, sat, vc);

	cocos2d::_ccColor3B out;
	out.r = R * 255;
	out.g = G * 255;
	out.b = B * 255;
	return out;
};

class $modify(PlayLayer){
	void postUpdate(float p0){

		PlayLayer::postUpdate(p0);

if (g >= 360)
{
	g = 0;
}
else
{
	g += 0.5;
}

auto rainbowColor = getRainbow(0);
auto rainbowColor2 = getRainbow(180);
bool enable = Mod::get()->getSettingValue<bool>("enable");

if (enable == true)
{
	if (m_player1)
	{
		m_player1->setColor(rainbowColor);
		m_player1->setSecondColor(rainbowColor);

		if (m_player1->m_waveTrail)
		{
			m_player1->m_waveTrail->setColor(rainbowColor);
		}
	}

	if (m_player2)
	{
		m_player2->setColor(rainbowColor2);
		m_player2->setSecondColor(rainbowColor2);

		if (m_player2->m_waveTrail)
		{
			m_player2->m_waveTrail->setColor(rainbowColor2);
		}
	}
}
}
}
;