#include "Definitions.h"
#include "BoardsStorage.h"

namespace TakeTen {

	BoardsStorage::BoardsStorage(const Size& size) 
		: _size(size)  {	}

	BoardsStorage::BoardsStorage(const Size& size, const vectorOfBoards& boards) 
		: _size(size), _boards(boards) { }

	BoardsStorage::~BoardsStorage() {
		_boards.clear();
	}
	
	BoardsStorage::BoardsStorage() { }

	bool BoardsStorage::addBoard(const boardPtr& board) {

		bool alreadyInStack = false;
		for (const auto b : _boards) {
			if (*b == *board) {
				alreadyInStack = true;
				return false;
			}
		}
		_boards.push_back(board);
		return true;
	}

	void BoardsStorage::addMoreBoards() {
		
		int count = BOARDS_TO_GENERATE;
		while (count > 0) {
			boardPtr newBoard(new TakeTen::Board(_size));
			newBoard->init();
			if (newBoard->canBeSolved()) {
				if (addBoard(newBoard)) {
					count--;
				}
			}
		}

		cocos2d::log("ADDED %d BOARDS of (%d, %d)", BOARDS_TO_GENERATE, _size.getWidht(), _size.getHeight());
	}

	Size& BoardsStorage::getSize() {
		return _size;
	}
}