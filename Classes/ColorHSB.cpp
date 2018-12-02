#include "ColorHSB.h"

namespace TakeTen {
	ColorHSB::ColorHSB() 
		: h(0), s(0), b(0) { }

	ColorHSB::ColorHSB(const int hh, const int ss, const int bb) 
		: h(hh), s(ss), b(bb) { }

	ColorHSB::ColorHSB(const ColorHSB& other) 
		: h(other.h), s(other.s), b(other.b) { }

	ColorHSB::ColorHSB(const cocos2d::Color3B& rgb) {
		*this = RGBToHSB(rgb);
	}

	ColorHSB::~ColorHSB() {

	}

	ColorHSB ColorHSB::RGBToHSB(const cocos2d::Color3B& rgb) {
		auto rd = rgb.r / 255.0;
		auto gd = rgb.g / 255.0;
		auto bd = rgb.b / 255.0;
		auto max = TakeTen::Tools::doubleMaximum(rd, gd, bd);
		auto min = TakeTen::Tools::doubleMinimum(rd, gd, bd);

		double h = 0;
		auto v = max;

		auto d = max - min;
		auto s = max == 0 ? 0.0 : d / max;

		if (max == min) {
			h = 0;
		}
		else {
			if (max == rd) {
				h = (gd - bd) / d + (gd < bd ? 6 : 0);
			}
			else if (max == gd) {
				h = (bd - rd) / d + 2;
			}
			else if (max == bd) {
				h = (rd - gd) / d + 4;
			}
			h /= 6;
		}

		auto bb = static_cast<int>(v * 100);
		auto ss = static_cast<int>(s * 100);
		auto hh = static_cast<int>(h * 360);

		return ColorHSB(hh, ss, bb);
	}

	cocos2d::Color3B ColorHSB::HSBToRGB(const ColorHSB& hsl) {
		return HSBToRGB(hsl.h, hsl.s, hsl.b);
	}

	cocos2d::Color3B ColorHSB::HSBToRGB(int hh, int ss, int vv) {
		auto v = vv / 100.0;
		auto s = ss / 100.0;
		auto h = hh / 360.0;

		auto i = static_cast<int>(h * 6);
		auto f = h * 6 - i;
		auto p = v * (1 - s);
		auto q = v * (1 - f * s);
		auto t = v * (1 - (1 - f) * s);

		double r = 0;
		double g = 0;
		double b = 0;

		switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}

		auto R = static_cast<unsigned int>(r * 255);
		auto G = static_cast<unsigned int>(g * 255);
		auto B = static_cast<unsigned int>(b * 255);

		return cocos2d::Color3B(R, G, B);
	}

	cocos2d::Color3B ColorHSB::toRGB() const {
		return HSBToRGB(h, s, b);
	}

	std::vector<ColorHSB> ColorHSB::calculateColors(const TakeTen::Size& boardSize, std::vector<ColorHSB> &colors) {
		auto count = boardSize.getCount();
		
		auto index0 = 0;
		auto index1 = boardSize.getIndex(boardSize.width - 1, 0);
		auto index2 = boardSize.getIndex(0, boardSize.height - 1);
		auto index3 = boardSize.getIndex(boardSize.width - 1, boardSize.height - 1);

		auto size = boardSize.width - 1.0f;
		auto color = colors[index0];
		auto step = ColorHSB::calculateColorStep(colors[index0], colors[index1], size);

		for (auto i = index0; i != index1; ++i) {
			colors[i] = color;
			color += step;
		}

		color = colors[index2];
		step = ColorHSB::calculateColorStep(colors[index2], colors[index3], size);

		for (auto i = index2; i != index3; ++i) {
			colors[i] = color;
			color += step;
		}

		size = boardSize.height - 1.0f;
		for (auto c = 0; c != boardSize.width; ++c) {
			auto c1 = colors[boardSize.getIndex(c, 0)];
			auto c2 = colors[boardSize.getIndex(c, boardSize.height - 1)];
			color = c1;
			step = ColorHSB::calculateColorStep(c1, c2, size);
			for (auto r = 0; r != boardSize.height; ++r) {
				colors[boardSize.getIndex(c, r)] = color;
				color += step;
			}
		}

		return colors;
	}

	ColorHSB ColorHSB::calculateColorStep(const ColorHSB& fromColor, const ColorHSB& toColor, const float stepCount) {
		return (toColor - fromColor) / stepCount;
	}

	ColorHSB ColorHSB::lerp(const ColorHSB& color1, const ColorHSB& color2, float alpha) {
		auto h = static_cast<unsigned int>((1 - alpha)*color1.h + alpha*color2.h);
		auto s = static_cast<unsigned int>((1 - alpha)*color1.s + alpha*color2.s);
		auto b = static_cast<unsigned int>((1 - alpha)*color1.b + alpha*color2.b);
		return ColorHSB(h, s, b);
	}
}

