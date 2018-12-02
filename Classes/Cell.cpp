#include "Cell.h"

namespace TakeTen {
	Cell::Cell() 
		: value(0), position() { }
	
	Cell::Cell(const Cell& other) 
		: value(other.value), position(other.position) { }
	
	Cell::Cell(Cell&& other) 
		: value(other.value), position(other.position) { }
	
	Cell::Cell(const Position& position) 
		: value(0), position(position) { }
	
	Cell::Cell(const Position& position, const char value) 
		: value(value), position(position) { }
	
	void Cell::reset() {
		value = -1;
	}

	bool Cell::isZero() const {
		return value == 0;
	}

	bool Cell::sameValue(const Cell& other) const {
		return value == other.value || value == 10 - other.value || 10 - value == other.value;
	}

	std::size_t Cell::getHash() const {
		size_t hash = (std::hash<int>()(value) ^ (position.getHash() << 1)) >> 1;
		return hash;
	}
}

