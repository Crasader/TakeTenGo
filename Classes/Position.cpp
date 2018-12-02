#include "Position.h"

namespace TakeTen {
	Position::Position() 
		: _column(0), _row(0) { }

	Position::Position(const unsigned char c, const unsigned char r)
		: _column(c), _row(r) { }

	Position::Position(Position&& other) 
		: _column(other._column), _row(other._row) { }

	Position::Position(const Position& other)
		: _column(other._column), _row(other._row) { }

	Position::~Position() { }

	unsigned char Position::getColumn() const {
		return _column;
	}

	unsigned char Position::getRow() const {
		return _row;
	}

	void Position::setPosition(const unsigned char c, const unsigned char r) {
		_column = c;
		_row = r;
	}

	void Position::setColumn(const unsigned char c) {
		_column = c;
	}

	void Position::setRow(const unsigned char r) {
		_row = r;
	}
}

