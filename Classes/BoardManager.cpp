#include <memory>
#include <vector>

#include "Definitions.h"
#include "BoardManager.h"
#include "DataHandler.h"
#include "cocos2d.h"

namespace TakeTen {

	static std::shared_ptr<BoardManager> _sharedBoardManager = nullptr;
	static Size _sizes[6] = { Size(3, 3), Size(3, 4), Size(3, 5), Size(4, 4), Size(4, 5), Size(5, 5) };

	std::shared_ptr<BoardManager> BoardManager::getInstance() {
		if (!_sharedBoardManager) {
			_sharedBoardManager = std::shared_ptr<BoardManager>(new (std::nothrow) BoardManager());
			CCASSERT(_sharedBoardManager, "FATAL: Not enough memory");
			_sharedBoardManager->init();
		}
		return _sharedBoardManager;
	}

	bool BoardManager::init() {

		fillWithEmpty();

		auto dataHandler = DataHandler::getInstance();

		vectorOfBoards boardsToLoad;
		if (dataHandler->readBoards(boardsToLoad)) {
			for (auto &b : boardsToLoad) {
				for (const auto& bs : _boardsStorage) {
					if (bs->getSize() == b->getSize()) {
						bs->addBoard(b);
						break;
					}
				}
			}
			return true;
		}

		//dataHandler->saveBoards(_boardsStorage);

		//return true;
		return false;
	}

	void BoardManager::fillWithEmpty() {
		_boardsStorage.clear();
		for (auto i = 0; i != 6; ++i) {
			boardStoragePtr boardRecord(new BoardsStorage(_sizes[i]));
			_boardsStorage.push_back(boardRecord);
		}
	}

	void BoardManager::generateBoards() {
		fillWithEmpty();
		for (auto i = 0; i != 6; ++i) {
			boardStoragePtr boardRecord(new BoardsStorage(_sizes[i]));
			boardRecord->addMoreBoards();
			_boardsStorage.push_back(boardRecord);
		}
		DataHandler::getInstance()->saveBoards(_boardsStorage);
	}

	boardPtr BoardManager::getBoard(const Size& size, const size_t index) {
		boardPtr result;
		bool found = false;
		for (auto& bs : _boardsStorage) {
			if (bs->getSize() == size) {
				return bs->getBoard(index);
			}
		}
		CCASSERT(found, "Board manager error");
		return result;
	}
}
