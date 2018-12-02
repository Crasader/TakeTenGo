#include "Cell.h"

namespace TakeTen {
	Cell::Cell() : _value(0), _position() { }

	Cell::Cell(const Cell& other) : _value(other._value), _position(other._position) { }

	Cell::Cell(Cell&& other) : _value(other._value), _position(other._position) { }

	Cell::Cell(Position& position) : _value(0), _position(position) { }

	Cell::Cell(Position& position, const char value) : _value(value), _position(position) { }

	Cell::~Cell() { }

	const Position& Cell::getPosition() const {
		return _position;
	}

	void Cell::reset() {
		_value = -1;
	}

	char Cell::getValue() const {
		return _value;
	}

	void Cell::setValue(char value) {
		_value = value;
	}

	bool Cell::isZero() const {
		return _value == 0;
	}



}

