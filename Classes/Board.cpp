#include "Board.h"
#include "Definitions.h"
#include "cocos2d.h"

namespace TakeTen {
	Board::Board() : _size(3, 3) {
	}

	Board::Board(const int size)
		: _size(size, size) {
	}

	Board::Board(const int width, const int height)
		: _size(width, height) {
	}

	Board::Board(const Size& size) 
		: _size(size) {
	}

	Board::Board(const Board& other) 
		: _size(other._size), _cells(other._cells),
		_foundPairs(other._foundPairs), _selectedPair(other._selectedPair),
		_initialPairs (other._initialPairs) {
	}

	Board::Board(Board&& other)
		: _size(other._size), _cells(other._cells),
		_foundPairs(other._foundPairs), _selectedPair(other._selectedPair),
		_initialPairs(other._initialPairs) {
	}

	Board::Board(const Size& size, std::vector<char>& values) 
		: _size(size) {

		unsigned char column = _size.getWidht();
		unsigned char row = _size.getHeight();

		for (auto r = 0; r != row; ++r)
			for (auto c = 0; c != column; ++c) {
				auto index = getIndex(c, r);
				_cells.push_back(Cell(Position(c, r), values[index]));
			}

		findPairs();
	}

	Board::~Board() {
		_cells.clear();
		_foundPairs.clear();
	}

	const Size& Board::getSize() const {
		return _size;
	}

	std::vector<Cell> Board::getCells() {
		return _cells;
	}

	Cell& Board::getCellByIndex(const int index) {
		return _cells[index];
	}

	Cell& Board::getCellByPosition(const Position& position) {
		auto index = getIndex(position);
		return _cells[index];
	}

	bool Board::selectCell(const int index, Pair& pair) {
		return selectCell(_cells[index], pair);
	}

	bool Board::selectCell(const Position& position, Pair& pair) {
		return selectCell(getCellByPosition(position), pair);
	}

	bool Board::selectCell(const Cell& cell, Pair& foundPair) {

		auto cell1 = _selectedPair.getCell1();
		auto cell2 = _selectedPair.getCell2();

		if (cell1 == cell || cell2 == cell) {
			_selectedPair = Pair();
			_selectedPair.setCell1(cell);
			return false;
		}

		if (cell1.getValue() < 0) {
			_selectedPair.setCell1(cell);
			return false;
		}

		if (cell2.getValue() < 0) {
			_selectedPair.setCell2(cell);
		}

		if (removePair(foundPair)) {
			return true;
		}

		_selectedPair = Pair();
		_selectedPair.setCell1(cell);

		return false;
	}

	bool Board::removePair(Pair& pair) {
		for (auto foundPair : _foundPairs) {
			if (foundPair == _selectedPair) {
				addToUndo();
				pair = _selectedPair;
				auto cell1 = _selectedPair.getCell1();
				auto cell2 = _selectedPair.getCell2();
				auto index1 = getIndex(cell1);
				auto index2 = getIndex(cell2);
				_cells[index1].reset();
				_cells[index2].reset();
				_selectedPair = Pair();
				findPairs();
				return true;
			}
		}
		return false;
	}

	void Board::reset() {

		_selectedPair = Pair();

		_initialPairs.clear();
		_foundPairs.clear();
		_cells.clear();
		
		unsigned char column = _size.getWidht();
		unsigned char row = _size.getHeight();

		for (auto r = 0; r != row; ++r)
			for (auto c = 0; c != column; ++c)
				_cells.push_back(Cell(Position(c, r)));

		if (false) //TTRANDBOOL
			if (column == row)
				if (Tools::randBool())
					for (auto i = 1; i != column - 1; ++i)
						_cells[getIndex(i, i)] = Cell(Position(i, i), -1);
				else 
					for (auto i = 1; i != column - 1; ++i)
						_cells[getIndex(column - i - 1, i)] = Cell(Position(column - i - 1, i), -1);

		if (_size.getCount() % 2 != 0) {
			column = _size.getWidht() / 2;
			row = _size.getHeight() / 2;
			_cells[getIndex(column, row)] = Cell(Position(column, row), -1);
		}
	}

	bool Board::isEmpty() const {
		for (auto& c : _cells) {
			if (c.getValue() > 0)
				return false;
		}
		return true;
	}

	void Board::init() {
		reset();

		bool isColumn;
		bool succeeded = true;

		char index1 = 0;
		char index2 = 0;

		auto width = _size.getWidht();
		auto height = _size.getHeight();

		auto randoms = 1;

		Position position1;
		Position position2;

		while (randoms > 0) {
			
			isColumn = Tools::randBool();

			position1 = Position();
			position2 = Position();

			auto c = Tools::randRange(0, width - 1);
			auto r = Tools::randRange(0, height - 1);

			auto cell = getCellByPosition(Position(c, r));
			if (!cell.isZero()) 
				continue;

			if (isColumn) {
				auto columns = getEmptyColumns(r);
				auto column = c;
				do {
					column = columns[Tools::randRange(0, columns.size() - 1)];
				} while (column == c);
				position1 = Position(c, r);
				position2 = Position(column, r);
			}
			else {
				auto rows = getEmptyRows(c);
				auto row = r;
				do {
					row = rows[Tools::randRange(0, rows.size() - 1)];
				} while (row == r);
				position1 = Position(c, r);
				position2 = Position(c, row);
			}

			auto pair = Pair(position1, position2);
			index1 = getIndex(position1);
			index2 = getIndex(position2);

			if (_cells[index1].getValue() < 0)
				continue;
			if (_cells[index2].getValue() < 0)
				continue;
			if (!_cells[index1].isZero())
				continue;
			if (!_cells[index2].isZero())
				continue;

			_cells[index1] = pair.getCell1();
			_cells[index2] = pair.getCell2();

			_initialPairs.push_back(pair);

			randoms--;
		}

		index1 = 0;
		index2 = 0;

		for (auto r = 0; r != height; ++r) {
			for (auto c = 0; c != width; ++c) {
				if (_cells[index1].isZero()) {
					auto columns = getEmptyColumns(r);
					auto rows = getEmptyRows(c);

					bool canBeColumn = columns.size() > 1;
					bool canBeRow = rows.size() > 1;

					if (canBeColumn && canBeRow)
						isColumn = Tools::randBool();
					else if (canBeColumn)
						isColumn = true;
					else if (canBeRow)
						isColumn = false;
					else {
						succeeded = false;
						break;
					}
					if (isColumn) {
						auto column = c;
						do {
							column = columns.at(Tools::randRange(0, columns.size()-1));
						} while (column == c);
						position1 = Position(c, r);
						position2 = Position(column, r);
					}
					else {
						auto row = r;
						do {
							row = rows.at(Tools::randRange(0, rows.size() - 1));
						} while (row == r);
						position1 = Position(c, r);
						position2 = Position(c, row);
					}

					auto pair = Pair(position1, position2);
					index2 = getIndex(position2);

					_cells[index1] = pair.getCell1();
					_cells[index2] = pair.getCell2();

					_initialPairs.push_back(pair);
				}
				++index1;

				
			}
			if (!succeeded) 
				break;
		}

/*
// 		
// 		if (succeeded)
// 			if (_size.getCount() > 9)
// 				if (!checkNumbers())
// 					succeeded = false;
*/
		
		if (!succeeded) 
			init();
		else
			findPairs();
	}

	void Board::findPairs() {
		_foundPairs.clear();
		if (isEmpty()) {
			return;
		}

		
		Cell foundCell;
		for (auto i = 0; i < _size.getCount(); i++) {
			auto cell = _cells[i];
			if (checkRight(cell, foundCell))
				_foundPairs.push_back(Pair(cell, foundCell));
			if (checkDown(cell, foundCell))
				_foundPairs.push_back(Pair(cell, foundCell));
		}
	}

	bool Board::solve(Pair& tmpPair) {

		CCLOG("Total pairs: %d", _foundPairs.size());
		auto found = _foundPairs.size() > 0;
		if (found) {
			auto index = TakeTen::Tools::randRange(0, _foundPairs.size() - 1);
			tmpPair = _foundPairs[index];
			//_selectedPair = tmpPair;
			//found = removePair(tmpPair);
		}

		return found;

		/*
		auto found = false;
		for (auto& foundPair : _foundPairs) {
			for (auto& pair : _initialPairs) {
				if (pair == foundPair) {
					_selectedPair = pair;
					found = removePair(tmpPair);
					break;
				}
			}
			if (found) {
				break;
			}
		}
		if (found) {
			return true;
		}
		return false;
		*/
	}

	bool Board::canBeSolved() {
		auto storedCells = _cells;
		auto pair = TakeTen::Pair();
		while (!isEmpty()) {
			if (!solve(pair)) {
				return false;
			}
		}
		_cells = storedCells;
		findPairs();
		return true;
	}

	bool Board::checkRight(const Cell& cell, Cell& result) {
		bool found = false;
		auto columnsCount = _size.getWidht();
		auto position = cell.getPosition();
		auto start = position.getColumn() + 1;
		if (start == columnsCount)
			return found;
		for (auto c = start; c != columnsCount; ++c) {
			auto index = getIndex(c, position.getRow());
			auto foundCell = _cells[index];
			if (foundCell.getValue() < 1)
				continue;
			if (foundCell.sameValue(cell)) {
				result = _cells[index];
				found = true;
				break;
			}
			else
				break;
		}
		return found;
	}

	bool Board::checkDown(const Cell& cell, Cell& result) {
		bool found = false;
		auto rowsCount = _size.getHeight();
		auto position = cell.getPosition();
		auto start = position.getRow() + 1;
		if (start == rowsCount)
			return found;
		for (auto r = start; r != rowsCount; ++r) {
			auto index = getIndex(position.getColumn(), r);
			auto foundCell = _cells[index];
			if (foundCell.getValue() < 1)
				continue;
			if (foundCell.sameValue(cell)) {
				result = _cells[index];
				found = true;
				break;
			}
			else
				break;
		}
		return found;
	}

	bool Board::checkNumbers() const {
		int numbers[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		int count = _size.getCount();

		for (int i = 0; i != count; ++i) {
			auto index = _cells[i].getValue() - 1;
			if (index < 0)
				continue;
			numbers[index]++;
		}
		for (int i = 0; i < 9; i++)
			if (numbers[i] == 0)
				return false;
		return true;
	}

	const int Board::getIndex(const Cell& cell) const {
		return getIndex(cell.getPosition());
	}

	const int Board::getIndex(const Position& position) const {
		return _size.getWidht()*position.getRow() + position.getColumn();
	}

	const int Board::getIndex(const int column, const int row) const {
		return _size.getWidht() * row + column;
	}

	bool Board::isFull() const {
		int count = _size.getCount();
		for (auto i = 0; i != count; ++i)
			if (_cells[i].isZero())
				return false;
		
		return true;
	}

	bool Board::rowIsFull(const unsigned char row) const {
		auto startIndex = getIndex(0, row);
		auto endIndex = startIndex + _size.getHeight();
		for (auto index = startIndex; index != endIndex; ++index)
			if (_cells[index].isZero())
				return false;
		return true;
	}

	bool Board::allRowsAreFull() const {
		for (auto r = 0; r != _size.getHeight(); ++r)
			if (!rowIsFull(r))
				return false;
		return true;
	}

	bool Board::columnIsFull(const unsigned char column) const {
		auto width = _size.getWidht();
		auto height = _size.getHeight();
		for (auto r = 0; r < width; r++) {
			auto index = height * r + column;
			if (_cells[index].isZero())
				return false;
		}
		return true;
	}

	bool Board::allColumnAreFull() const {
		auto width = _size.getWidht();
		for (auto c = 0; c < width; c++) {
			if (!columnIsFull(c))
				return false;
		}
		return true;
	}

	std::vector<unsigned char> Board::getEmptyRows(const unsigned char column) const {
		std::vector<unsigned char> result;
		auto height = _size.getHeight();
		for (auto r = 0; r < height; r++)
			if (_cells[getIndex(column, r)].isZero())
				result.push_back(r);
		return result;
	}

	std::vector<unsigned char> Board::getEmptyColumns(const unsigned char row) const {
		std::vector<unsigned char> result;
		auto width = _size.getWidht();
		for (auto c = 0; c < width; c++) 
			if (_cells[getIndex(c, row)].isZero())
				result.push_back(c);
		return result;
	}

	void Board::printBoard() {
		CCLOG("*****************");
		std::string result;
		unsigned char c = 0;
		auto count = _size.getCount();
		auto width = _size.getWidht();
		auto height = _size.getHeight();
		for (auto i = 0; i < count; i++) {
			if (c == width) {
				c = 0;
				result += '\n';
			}
			auto value = (_cells[i].getValue() > 0 ? std::to_string(_cells[i].getValue()) : " ");
			result += value + ' ';
			c++;
		}
		CCLOG(result.c_str());
	}

	void Board::addToUndo() {
		std::vector<char> data;
		for (auto& c : _cells) {
			data.push_back(c.getValue());
		}
		_undoData.push_back(data);
	}

	void Board::undo() {
		if (canUndo()) {
			auto data = _undoData.back();
			_undoData.pop_back();
			for (auto i = 0; i != _cells.size(); ++i) {
				_cells[i].setValue(data[i]);
			}
			findPairs();
		}
	}
}




