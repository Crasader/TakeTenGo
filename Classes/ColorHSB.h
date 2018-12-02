#pragma once
#ifndef __COLOR_HSB_H__
#define __COLOR_HSB_H__

#include <vector>
#include "Definitions.h"
#include "Size.h"
#include "Tools.h"
#include "cocos2d.h"

namespace TakeTen {
	struct ColorHSB {
		ColorHSB();
		ColorHSB(const int h, const int s, const int l);
		ColorHSB(const cocos2d::Color3B& rgb);
		ColorHSB(const ColorHSB& other);
		~ColorHSB();

		cocos2d::Color3B toRGB() const;

		static cocos2d::Color3B HSBToRGB(const ColorHSB& hsl);
		static ColorHSB RGBToHSB(const cocos2d::Color3B& rgb);
		static cocos2d::Color3B HSBToRGB(int hh, int ss, int vv);
		static ColorHSB lerp(const ColorHSB& color1, const ColorHSB& color2, float alpha);
		static ColorHSB calculateColorStep(const ColorHSB& fromColor, const ColorHSB& toColor, const float stepCount);
		static std::vector<ColorHSB> calculateColors(const TakeTen::Size& size, std::vector<ColorHSB> &colors);

		ColorHSB& operator +=(const ColorHSB& other) {
			h += other.h;
			s += other.s;
			b += other.b;
			return *this;
		}

		ColorHSB operator +(const ColorHSB& other) const {
			return ColorHSB(h + other.h, s + other.s, b + other.b);
		}

		ColorHSB operator -(const ColorHSB& other) const {
			return ColorHSB(h - other.h, s - other.s, b - other.b);
		}

		ColorHSB operator /(const float other) const {
			auto hh = static_cast<int>(h / other);
			auto ss = static_cast<int>(s / other);
			auto bb = static_cast<int>(b / other);
			return ColorHSB(hh, ss, bb);
		}

		int h;
		int s;
		int b;

	};
}
#endif