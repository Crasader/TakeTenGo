#pragma once
#ifndef __TT_POSITION_H__
#define __TT_POSITION_H__

#include <functional>

namespace TakeTen {
	struct Position {
		Position();
		Position(const unsigned char c, const unsigned char r);
		Position(const Position& other);
		Position(Position&& other);
		~Position();

		void set(const unsigned char c, const unsigned char r);

		Position& operator= (const Position& other) {
			if (this != &other) {
				column = other.column;
				row = other.row;
			}
			return *this;
		}

		Position& operator= (Position&& other) {
			if (this != &other) {
				column = other.column;
				row = other.row;
			}
			return *this;
		}

		bool operator ==(const Position& other) {
			bool result = true;
			if (this != &other) {
				result = column == other.column && row == other.row;
			}
			return result;
		}

		std::size_t getHash() const;

		unsigned char column;
		unsigned char row;

	};
}




#endif