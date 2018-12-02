#include "Position.h"

namespace TakeTen {
	Position::Position() 
		: column(0), row(0) { }

	Position::Position(const unsigned char c, const unsigned char r)
		: column(c), row(r) { }

	Position::Position(Position&& other) 
		: column(other.column), row(other.row) { }

	Position::Position(const Position& other)
		: column(other.column), row(other.row) { }

	Position::~Position() { }

	void Position::set(const unsigned char c, const unsigned char r) {
		column = c;
		row = r;
	}

	std::size_t Position::getHash() const {
			size_t hash = (std::hash<int>()(column) ^ std::hash<int>()(row << 1)) >> 1;
			return hash;
	}
}

