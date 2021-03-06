#pragma once
#ifndef __TT_BOARD_H__
#define __TT_BOARD_H__

#include "Size.h"
#include "Cell.h"
#include "Pair.h"

namespace TakeTen {

	class Board {
	public:
		Board(Board&& other);
		Board(const Board& other);
		Board(const Size& size, std::vector<char> values);
		
		~Board();

		void printBoard();

		const Size& getSize() const;

		std::vector<Cell> getCells();
		Cell& getCellByIndex(const int index);
		Cell& getCellByPosition(const Position& position);

		bool selectCell(const Cell& cell, Pair& pair);
		bool selectCell(const int cellIndex, Pair& pair);
		bool selectCell(const Position& position, Pair& pair);

		bool isEmpty() const;

		const int getIndex(const Cell& cell) const;
		const int getIndex(const Position& position) const;
		const int getIndex(const int column, const int row) const;

		bool operator== (const Board& other) {

			if (_size != other._size) {
				return false;
			}

			if (_cells.size() != other._cells.size()) {
				return false;
			}

			for (auto i = 0; i != _cells.size(); ++i) {
				if (_cells[i] != other._cells[i]) {
					return false;
				}
			}

			return true;
		}

		std::size_t getHash() const {
			size_t hash = _size.getHash();
			auto shift = 2;
			for (auto c : _cells) {
				hash = (hash ^ (c.getHash() << shift)) >> shift;
			}
			return hash;
		}

		void undo();

		inline bool canUndo() const {
			return _undoData.size() > 0;
		}

	private:
		
		void findPairs();
		bool checkRight(const Cell& cell, Cell& result);
		bool checkDown(const Cell& cell, Cell& result);
		void addToUndo();
		bool removePair(Pair& pair);

	private:

		Size _size;
		std::vector<Cell> _cells;
		std::vector<Pair> _initialPairs;
		std::vector<Pair> _foundPairs;

		std::vector<std::vector<char>> _undoData;

		Pair _selectedPair;

#if GENERATE_BOARDS
	public:
		Board();
		Board(const int size);
		Board(const int width, const int height);
		Board(const Size& size);

		void init();
		bool solve(Pair& pair);
		bool canBeSolved();

		std::vector<Pair> getInitialPairs() {
			return _initialPairs;
		}
	private:
		void reset();
		bool checkNumbers() const;
		bool isFull() const;
		bool rowIsFull(const unsigned char row) const;
		bool allRowsAreFull() const;
		bool columnIsFull(const unsigned char column) const;
		bool allColumnAreFull() const;
		std::vector<unsigned char> getEmptyRows(const unsigned char column) const;
		std::vector<unsigned char> getEmptyColumns(const unsigned char row) const;
#endif
	};

	typedef std::shared_ptr<Board> boardPtr;
	typedef std::vector <boardPtr> vectorOfBoards;
}




#endif