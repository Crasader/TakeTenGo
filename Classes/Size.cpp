#include "Size.h"

namespace TakeTen {
	Size::Size() 
		: width(0), height(0), _count(0) {}

	Size::Size(const unsigned char newWidth, const unsigned char newHeight)
		: width(newWidth), height(newHeight), _count(newWidth * newHeight) { }

	Size::Size(const Size& other) 
		: width(other.width), height(other.height), _count(other.width * other.height) { }

	Size::Size(Size&& other) 
		: width(other.width), height(other.height), _count(other.width * other.height) { }

	Size::~Size() {	}

	unsigned char Size::getCount() const {
		return _count;
	}

	const unsigned char Size::getIndex(const Position& position) const {
		return width*position.row + position.column;
	}

	const unsigned char Size::getIndex(const unsigned char column, const unsigned char row) const {
		return width * row + column;
	}

	std::size_t Size::getHash() const {
		size_t hash = (std::hash<int>()(width) ^ std::hash<int>()(height << 1)) >> 1;
		return hash;
	}

}

