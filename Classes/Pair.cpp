#include "Pair.h"

#include "Definitions.h"

namespace TakeTen {

	Pair::Pair() 
		: cell1(Position(-1, -1), -1), cell2(Position(-2, -2), -2) {

	}

	Pair::Pair(const Pair& other)
		: cell1(other.cell1), cell2(other.cell2) { }

	Pair::Pair(Pair&& other) 
		: cell1(other.cell1), cell2(other.cell2) { }

	Pair::Pair(Cell& cell1, Cell& cell2) 
		: cell1(cell1), cell2(cell2) { }

	Pair::Pair(Position& position1, Position& position2) {
		static int value = 1;
		auto value1 = value++;// TTRAND(1, 9);
		if (!USE_FIVE)
		{
			if (value1 == 5)
			{
				value1 = 6;
			}
		}
		if (value > 9)
		{
			value = 1;
		}
		auto value2 = Tools::randBool(75) ? 10 - value1 : value1;
		cell1 = Cell(position1, value1);
		cell2 = Cell(position2, value2);
	}

	Pair::~Pair() {

	}

	/*
	void Pair::setCell1(const Cell& cell) {
		cell1 = cell;
	}

	void Pair::setCell2(const Cell& cell) {
		cell2 = cell;
	}

	Cell& Pair::getCell1() {
		return cell1;
	}

	Cell& Pair::getCell2() {
		return cell2;
	}
*/


	void Pair::reset() {
		cell1.reset();
		cell2.reset();
	}

	
}


