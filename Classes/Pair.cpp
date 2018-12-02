#include "Pair.h"
#include "Definitions.h"

namespace TakeTen {

	Pair::Pair() 
		: _cell1(Position(-1, -1), -1), _cell2(Position(-2, -2), -2) {

	}

	Pair::Pair(const Pair& other)
		: _cell1(other._cell1), _cell2(other._cell2) { }

	Pair::Pair(Pair&& other) 
		: _cell1(other._cell1), _cell2(other._cell2) { }

	Pair::Pair(Cell& cell1, Cell& cell2) 
		: _cell1(cell1), _cell2(cell2) { }

	Pair::Pair(Position& position1, Position& position2) {
		static int value = 1;
		auto value1 = value++;// TTRAND(1, 9);
		if (value > 9)
			value = 1;
		auto value2 = Tools::randBool() ? value1 : 10 - value1;
		_cell1 = Cell(position1, value1);
		_cell2 = Cell(position2, value2);
	}

	Pair::~Pair() {

	}

	void Pair::setCell1(const Cell& cell) {
		_cell1 = cell;
	}

	void Pair::setCell2(const Cell& cell) {
		_cell2 = cell;
	}

	void Pair::reset() {
		_cell1.reset();
		_cell2.reset();
	}

	Cell& Pair::getCell1() {
		return _cell1;
	}

	Cell& Pair::getCell2() {
		return _cell2;
	}

}


