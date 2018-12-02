#include "cocos2d.h"
#include "Tools.h"
#include "Board.h"

namespace TakeTen {

	Board::Board(const Board& other) 
		: _size(other._size), _cells(other._cells),
		_foundPairs(other._foundPairs), _selectedPair(other._selectedPair),
		_initialPairs (other._initialPairs) { }

	Board::Board(Board&& other)
		: _size(other._size), _cells(other._cells),
		_foundPairs(other._foundPairs), _selectedPair(other._selectedPair),
		_initialPairs(other._initialPairs) { }

	Board::Board(const Size& size, std::vector<char> values) : _size(size) {
		auto column = _size.width;
		auto row = _size.height;
		for (auto r = 0; r != row; ++r) {
			for (auto c = 0; c != column; ++c) {
				auto index = getIndex(c, r);
				_cells.push_back(Cell(Position(c, r), values[index]));
			}
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
		auto cell1 = _selectedPair.cell1;
		auto cell2 = _selectedPair.cell2;
		if (cell1 == cell || cell2 == cell) {
			_selectedPair = Pair();
			_selectedPair.cell1 = cell;
			return false;
		}
		if (cell1.value < 0) {
			_selectedPair.cell1 = cell;
			return false;
		}
		if (cell2.value < 0) {
			_selectedPair.cell2 = cell;
		}
		if (removePair(foundPair)) {
			return true;
		}
		_selectedPair = Pair();
		_selectedPair.cell1 = cell;
		return false;
	}

	bool Board::removePair(Pair& pair) {
		for (auto foundPair : _foundPairs) {
			if (foundPair == _selectedPair) {
				addToUndo();
				pair = _selectedPair;
				auto cell1 = _selectedPair.cell1;
				auto cell2 = _selectedPair.cell2;
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

	

	bool Board::isEmpty() const {
		for (auto& c : _cells) {
			if (c.value > 0) {
				return false;
			}
		}
		return true;
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

	bool Board::checkRight(const Cell& cell, Cell& result) {
		bool found = false;
		auto columnsCount = _size.width;
		auto position(cell.position);
		auto start = position.column + 1;
		if (start == columnsCount) {
			return found;
		}
		for (auto c = start; c != columnsCount; ++c) {
			auto index = getIndex(c, position.row);
			auto foundCell = _cells[index];
			if (foundCell.value < 1)  {
				continue;
			}
			if (foundCell.sameValue(cell)) {
				result = _cells[index];
				found = true;
				break;
			}
			else {
				break;
			}

		}
		return found;
	}

	bool Board::checkDown(const Cell& cell, Cell& result) {
		bool found = false;
		auto rowsCount = _size.height;
		auto position(cell.position);
		auto start = position.row + 1;
		if (start == rowsCount) {
			return found;
		}
		for (auto r = start; r != rowsCount; ++r) {
			auto index = getIndex(position.column, r);
			auto foundCell = _cells[index];
			if (foundCell.value < 1) {
				continue;
			}
			if (foundCell.sameValue(cell)) {
				result = _cells[index];
				found = true;
				break;
			}
			else {
				break;
			}
		}
		return found;
	}

	const int Board::getIndex(const Cell& cell) const {
		return getIndex(cell.position);
	}

	const int Board::getIndex(const Position& position) const {
		return _size.width*position.row + position.column;
	}

	const int Board::getIndex(const int column, const int row) const {
		return _size.width * row + column;
	}

	void Board::printBoard() {
		cocos2d::log("Board #%d", getHash());
	}

	void Board::addToUndo() {
		std::vector<char> data;
		for (auto& c : _cells) {
			data.push_back(c.value);
		}
		_undoData.push_back(data);
	}

	void Board::undo() {
		if (canUndo()) {
			auto data = _undoData.back();
			_undoData.pop_back();
			for (auto i = 0; i != _cells.size(); ++i) {
				_cells[i].value = data[i];
			}
			findPairs();
		}
	}

#if GENERATE_BOARDS

	Board::Board() : _size(3, 3) {	}

	Board::Board(const int size)
		: _size(size, size) { }

	Board::Board(const int width, const int height)
		: _size(width, height) { }

	Board::Board(const Size& size) 
		: _size(size) { }

	void Board::reset() {
		_selectedPair = Pair();

		_initialPairs.clear();
		_foundPairs.clear();
		_cells.clear();

		auto column = _size.width;
		auto row = _size.height;

		for (auto r = 0; r != row; ++r) {
			for (auto c = 0; c != column; ++c) {
				_cells.push_back(Cell(Position(c, r)));
			}
		}
		if (_size.getCount() % 2 != 0) {
			column = _size.width / 2;
			row = _size.height / 2;
			_cells[getIndex(column, row)] = Cell(Position(column, row), -1);
		}
	}

	void Board::init() {

		reset();

		bool isColumn;
		bool succeeded = true;

		char index1 = 0;
		char index2 = 0;

		auto width = _size.width;
		auto height = _size.height;

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
			if (!cell.isZero()) {
				continue;
			}

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

			if (_cells[index1].value < 0) {
				continue;
			}
			if (_cells[index2].value < 0) {
				continue;
			}
			if (!_cells[index1].isZero()) {
				continue;
			}
			if (!_cells[index2].isZero()) {
				continue;
			}

			_cells[index1] = pair.cell1;
			_cells[index2] = pair.cell2;

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

					if (canBeColumn && canBeRow) {
						isColumn = Tools::randBool();
					}
					else if (canBeColumn) {
						isColumn = true;
					}
					else if (canBeRow) {
						isColumn = false;
					}
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

					_cells[index1] = pair.cell1;
					_cells[index2] = pair.cell2;

					_initialPairs.push_back(pair);
				}
				++index1;
			}
			if (!succeeded) {
				break;
			}
		}
		if (_size.width > 3 || _size.height > 3)
		{
			succeeded = checkNumbers();
		}
		if (succeeded) {
			findPairs();
		}
		else {
			init();
		}
	}

	bool Board::solve(Pair& tmpPair) {
		//CCLOG("Total pairs: %d", _foundPairs.size());
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

	bool Board::checkNumbers() const {
		int numbers[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		int count = _size.getCount();

		for (auto i = 0; i != count; ++i) {
			if (_cells[i].value == 0)
			{
				return false;
			}
			auto index = _cells[i].value - 1;
			if (index < 0) {
				continue;
			}
			numbers[index]++;
		}
		for (auto i = 0; i != 9; ++i) {
			if (!USE_FIVE)
			{
				if (i == 4) continue;
			}
			if (numbers[i] == 0) {
				return false;
			}
		}
		return true;
	}

	bool Board::isFull() const {
		int count = _size.getCount();
		for (auto i = 0; i != count; ++i) {
			if (_cells[i].isZero()) {
				return false;
			}
		}
		return true;
	}

	bool Board::rowIsFull(const unsigned char row) const {
		auto startIndex = getIndex(0, row);
		auto endIndex = startIndex + _size.height;
		for (auto index = startIndex; index != endIndex; ++index) {
			if (_cells[index].isZero()) {
				return false;
			}
		}
		return true;
	}

	bool Board::allRowsAreFull() const {
		for (auto r = 0; r != _size.height; ++r) {
			if (!rowIsFull(r)) {
				return false;
			}
		}
			
		return true;
	}

	bool Board::columnIsFull(const unsigned char column) const {
		auto width = _size.width;
		auto height = _size.height;
		for (auto r = 0; r < width; r++) {
			auto index = height * r + column;
			if (_cells[index].isZero()) {
				return false;
			}
		}
		return true;
	}

	bool Board::allColumnAreFull() const {
		auto width = _size.width;
		for (auto c = 0; c < width; c++) {
			if (!columnIsFull(c)) {
				return false;
			}
		}
		return true;
	}

	std::vector<unsigned char> Board::getEmptyRows(const unsigned char column) const {
		std::vector<unsigned char> result;
		auto height = _size.height;
		for (auto r = 0; r < height; r++) {
			if (_cells[getIndex(column, r)].isZero()) {
				result.push_back(r);
			}
		}
		return result;
	}

	std::vector<unsigned char> Board::getEmptyColumns(const unsigned char row) const {
		std::vector<unsigned char> result;
		auto width = _size.width;
		for (auto c = 0; c < width; c++) {
			if (_cells[getIndex(c, row)].isZero()) {
				result.push_back(c);
			}
		}
		return result;
	}

#endif

	
}




