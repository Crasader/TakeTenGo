#include "cocos2d.h"
#include "Definitions.h"
#include "BoardsStorageContainer.h"

namespace TakeTen {

	BoardsStorageContainer::BoardsStorageContainer(const Size& size) 
		: _size(size)  { }

	BoardsStorageContainer::BoardsStorageContainer(const Size& size, const vectorOfBoards& boards) 
		: _size(size), _boards(boards) { }

	BoardsStorageContainer::~BoardsStorageContainer() {
		_boards.clear();
	}
	
	BoardsStorageContainer::BoardsStorageContainer() { }

	Size& BoardsStorageContainer::getSize() {
		return _size;
	}

#if GENERATE_BOARDS
	void BoardsStorageContainer::addMoreBoards(int count) {
		auto left = count;
		while (left > 0) {
			boardPtr newBoard(new TakeTen::Board(_size));
			newBoard->init();
			if (newBoard->canBeSolved()) {
				if (addBoard(newBoard)) {
					left--;
					if (left % 10 == 0)
					{
						//CCLOG("%d BOARDS LEFT", left);
					}
				}
			}
			
		}
		CCLOG("ADDED %d BOARDS of (%d, %d)", count, _size.width, _size.height);
	}

	bool BoardsStorageContainer::addBoard(const boardPtr& board) {
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
#else 
	bool BoardsStorageContainer::addBoard(const boardPtr& board) {
		_boards.push_back(board);
		return true;
	}
#endif
}