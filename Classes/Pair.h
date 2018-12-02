#pragma once
#ifndef __TT_PAIR_H__
#define __TT_PAIR_H__

#include "Cell.h"

namespace TakeTen {
	struct Pair {
	public:
		Pair();
		Pair(Cell& cell1, Cell& cell2);
		Pair(Position& position1, Position& position2);
		Pair(const Pair& other);
		Pair(Pair&& other);
		~Pair();

		void reset();

		Pair& operator= (const Pair& other) {
			if (this != &other) {
				cell1 = other.cell1;
				cell2 = other.cell2;
			}
			return *this;
		}

		Pair& operator= (Pair&& other) {
			if (this != &other) {
				cell1 = other.cell1;
				cell2 = other.cell2;
			}
			return *this;
		}

		bool operator== (const Pair& other) {
			return (cell1 == other.cell1 && cell2 == other.cell2) || (cell1 == other.cell2 && cell2 == other.cell1);
		}

		Cell cell1;
		Cell cell2;
	};
}
#endif