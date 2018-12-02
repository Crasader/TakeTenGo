#pragma once
#ifndef __TT_SIZE_H__
#define __TT_SIZE_H__

#include "Position.h"

namespace TakeTen {
	struct Size {
	public:
		Size();
		Size(const unsigned char width, const unsigned char height);
		Size(const Size& other);
		Size(Size&& other);
		~Size();

		unsigned char getCount() const;

		const unsigned char getIndex(const Position& position) const;
		const unsigned char getIndex(const unsigned char column, const unsigned char row) const;

		Size& operator= (const Size& other) {
			if (this != &other) {
				width = other.width;
				height = other.height;
				_count = other._count;
			}

			return *this;
		}

		Size& operator= (Size&& other) {
			if (this != &other) {
				width = other.width;
				height = other.height;
				_count = other._count;
			}

			return *this;
		}

		bool operator== (const Size& other) {
			return (width == other.width) && (height == other.height);
		}

		bool operator!= (const Size& other) {
			return (width != other.width) || (height != other.height);
		}

		std::size_t getHash() const;

		unsigned char width;
		unsigned char height;

	private:
		unsigned char _count;
	};
}



#endif