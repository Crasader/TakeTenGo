#pragma once
#ifndef __TT_CELL_H__
#define __TT_CELL_H__

//#include <functional>
#include "Position.h"

namespace TakeTen {
	class Cell {
	public:
		Cell();
		Cell(Cell&& other);
		Cell(const Cell& other);
		Cell(const Position& position);
		Cell(const Position& position, const char value);

		void reset();
		bool isZero() const;

		bool sameValue(const Cell& other) const;
		std::size_t getHash() const;

		Cell& operator= (const Cell& other) {
			if (this != &other) {
				value = other.value;
				position = other.position;
			}
			return *this;
		}

		Cell& operator= (Cell&& other) {
			if (this != &other) {
				value = other.value;
				position = other.position;
			}
			return *this;
		}

		bool operator ==(const Cell& other) {
			bool val = (value == other.value) || (value == 10 - other.value) || (10 - value == other.value);
			bool pos = position == other.position;
			return val && pos;
		}

		bool operator !=(const Cell& other) {
			return !(*this == other);
		}

		char value;
		Position position;

	private:
		
	};
}



#endif