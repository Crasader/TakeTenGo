#ifndef __COLOR_HSB_H__
#define __COLOR_HSB_H__

#include <vector>
#include "Definitions.h"
#include "Size.h"
#include "Tools.h"
#include "cocos2d.h"

struct ColorHSB
{
	ColorHSB();
	ColorHSB(const int h, const int s, const int l);
	ColorHSB(const cocos2d::Color3B& rgb);
	ColorHSB(const ColorHSB& other);
	ColorHSB(ColorHSB&& other);
	~ColorHSB();

	cocos2d::Color3B toRGB() const;

	static inline cocos2d::Color3B HSBToRGB(const ColorHSB& hsl) {
		return HSBToRGB(hsl.h, hsl.s, hsl.b);
	}

	static inline void RGBToHSB(const cocos2d::Color3B& rgb, int& hh, int& ss, int& bb) {
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
			h = 0; // achromatic
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

		bb = static_cast<int>(v * 100);
		ss = static_cast<int>(s * 100);
		hh = static_cast<int>(h * 360);
	}

	static cocos2d::Color3B HSBToRGB(int hh, int ss, int vv) {
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

		auto R = static_cast<UINT>(r * 255);
		auto G = static_cast<UINT>(g * 255);
		auto B = static_cast<UINT>(b * 255);

		return cocos2d::Color3B(R, G, B);
	}

	static inline ColorHSB lerp(const ColorHSB& color1, const ColorHSB& color2, float alpha) {
		auto h = static_cast<UINT>((1 - alpha)*color1.h + alpha*color2.h);
		auto s = static_cast<UINT>((1 - alpha)*color1.s + alpha*color2.s);
		auto b = static_cast<UINT>((1 - alpha)*color1.b + alpha*color2.b);
		return ColorHSB(h,s,b);
	}

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

	static inline ColorHSB calculateColorStep(const ColorHSB& fromColor, const ColorHSB& toColor, const float stepCount) {
		return (toColor - fromColor) / stepCount;
	}

	static std::vector<ColorHSB> calculateColors(const TakeTen::Size& size);

	int h;
	int s;
	int b;

};


#endif