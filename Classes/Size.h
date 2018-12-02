#ifndef __TT_SIZE_H__
#define __TT_SIZE_H__

#include "Position.h"
#include <functional>

namespace TakeTen {
	class Size
	{
	public:
		Size();
		Size(const unsigned char width, const unsigned char height);
		Size(const Size& other);
		Size(Size&& other);
		~Size();

		unsigned char getWidht() const;
		unsigned char getHeight() const;
		unsigned char getCount() const;

		const unsigned char getIndex(const Position& position) const {
			return _width*position.getRow() + position.getColumn();
		}

		const unsigned char getIndex(const unsigned char column, const unsigned char row) const {
			return _width * row + column;
		}

		Size& operator= (const Size& other) {
			if (this != &other) {
				_width = other._width;
				_height = other._height;
				_count = other._count;
			}

			return *this;
		}

		Size& operator= (Size&& other) {
			if (this != &other) {
				_width = other._width;
				_height = other._height;
				_count = other._count;
			}

			return *this;
		}

		bool operator== (const Size& other) {
			return (_width == other._width) && (_height == other._height);
		}

		bool operator!= (const Size& other) {
			return (_width != other._width) || (_height != other._height);
		}

		std::size_t getHash() const {
			size_t hash = (std::hash<int>()(_width) ^ std::hash<int>()(_height << 1)) >> 1;
			return hash;
		}

	private:
		unsigned char _width;
		unsigned char _height;
		unsigned char _count;
	};
}



#endif