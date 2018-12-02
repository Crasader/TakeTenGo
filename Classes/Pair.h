#ifndef __TT_PAIR_H__
#define __TT_PAIR_H__

#include "Cell.h"

namespace TakeTen {
	class Pair {
	public:

		Pair();
		Pair(Cell& cell1, Cell& cell2);
		Pair(Position& position1, Position& position2);
		Pair(const Pair& other);
		Pair(Pair&& other);
		~Pair();

		void reset();

		Cell& getCell1();
		Cell& getCell2();

		void setCell1(const Cell& cell);
		void setCell2(const Cell& cell);

		Pair& operator= (const Pair& other) {
			if (this != &other) {
				_cell1 = other._cell1;
				_cell2 = other._cell2;
			}
			return *this;
		}

		Pair& operator= (Pair&& other) {
			if (this != &other) {
				_cell1 = other._cell1;
				_cell2 = other._cell2;
			}
			return *this;
		}

		bool operator== (const Pair& other) {
			return (_cell1 == other._cell1 && _cell2 == other._cell2) || (_cell1 == other._cell2 && _cell2 == other._cell1);
		}

	private:
		Cell _cell1;
		Cell _cell2;

	};
}




#endif