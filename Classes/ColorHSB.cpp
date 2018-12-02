#include "ColorHSB.h"

ColorHSB::ColorHSB() : h(0), s(0), b(0) {

}

ColorHSB::ColorHSB(const int hh, const int ss, const int bb) : h(hh), s(ss), b(bb) { }

ColorHSB::ColorHSB(const ColorHSB& other) : h(other.h), s(other.s), b(other.b) { }

ColorHSB::ColorHSB(ColorHSB&& other) : h(other.h), s(other.s), b(other.b) { }

ColorHSB::ColorHSB(const cocos2d::Color3B& rgb) {
	RGBToHSB(rgb, h, s, b);
}

ColorHSB::~ColorHSB() {

}

cocos2d::Color3B ColorHSB::toRGB() const {
	return HSBToRGB(h, s, b);
}

std::vector<ColorHSB> ColorHSB::calculateColors(const TakeTen::Size& boardSize) {

	auto count = boardSize.getCount();
	std::vector<ColorHSB> colors;

	for (auto i = 0; i != count; ++i)
		colors.push_back(ColorHSB());

	auto index0 = 0;
	auto index1 = boardSize.getIndex(boardSize.getWidht() - 1, 0);
	auto index2 = boardSize.getIndex(0, boardSize.getHeight() - 1);
	auto index3 = boardSize.getIndex(boardSize.getWidht() - 1, boardSize.getHeight() - 1);

	colors[index0] = ColorHSB(199, 25, 27);
	colors[index1] = ColorHSB(33, 40, 77);
	colors[index2] = ColorHSB(200, 23, 57);
	colors[index3] = ColorHSB(5, 53, 57);

	float size = boardSize.getWidht() - 1.0f;
	ColorHSB color = colors[index0];
	ColorHSB step = ColorHSB::calculateColorStep(colors[index0], colors[index1], size);

	for (auto i = index0; i != index1; ++i) {
		colors[i] = color;
		color += step;// hStep;
	}

	color = colors[index2];
	step = ColorHSB::calculateColorStep(colors[index2], colors[index3], size);

	for (auto i = index2; i != index3; ++i) {
		colors[i] = color;
		color += step;
	}

	size = boardSize.getHeight() - 1.0f;
	for (auto c = 0; c != boardSize.getWidht(); ++c) {
		auto c1 = colors[boardSize.getIndex(c, 0)];
		auto c2 = colors[boardSize.getIndex(c, boardSize.getHeight() - 1)];
		color = c1;
		step = ColorHSB::calculateColorStep(c1, c2, size);
		for (auto r = 0; r != boardSize.getHeight(); ++r) {
			colors[boardSize.getIndex(c, r)] = color;
			color += step;
		}
	}

	return colors;
}