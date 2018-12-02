#include "Size.h"

#include "cocos2d.h"

namespace TakeTen {
	Size::Size() : _width(0), _height(0), _count(0) {}

	Size::~Size() {	}

	Size::Size(const unsigned char width, const unsigned char height)
		: _width(width), _height(height), _count(width * height) { }

	Size::Size(const Size& other) 
		: _width(other._width), _height(other._height), _count(other._width * other._height) { }

	Size::Size(Size&& other) 
		: _width(other._width), _height(other._height), _count(other._width * other._height) { }

	unsigned char Size::getWidht() const {
		return _width;
	}

	unsigned char Size::getHeight() const {
		return _height;
	}

	unsigned char Size::getCount() const {
		return _count;
	}
}

