#include <memory>
#include <vector>

#include "cocos2d.h"

#include "Definitions.h"
#include "DataHandler.h"

#include "BoardManager.h"

namespace TakeTen {

	//static Size _sizes[6] = { Size(3, 3), Size(3, 4), Size(3, 5), Size(4, 4), Size(4, 5), Size(5, 5) };
	std::shared_ptr<BoardManager> BoardManager::_sharedBoardManager = nullptr;

	BoardManager::BoardManager() {
		const Size sizes[6] = BOARDS_SIZES;
		for (auto i = 0; i != 6; ++i) {
			boardStoragePtr boardRecord(new BoardsStorageContainer(sizes[i]));
			_boardsStorage.push_back(boardRecord);
		}
	}

	BoardManager::~BoardManager() {
		_boardsStorage.clear();
	}

	bool BoardManager::init() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		clock_t startTime = clock();
#endif
		CCLOG("BOARD MANAGER >> INIT...");
		auto dataHandler = DataHandler::getInstance();
		vectorOfBoards boardsToLoad;
		CCLOG("BOARD MANAGER >> READING BOARDS...");
		auto loaded = dataHandler->readBoards(boardsToLoad);
		if (loaded) {
			CCLOG("BOARD MANAGER >> LOADED... ADDING BOARDS TO CONTAINER...");
			/*
			for (auto &b : boardsToLoad) {
				for (const auto& bs : _boardsStorage) {
					if (bs->getSize() == b->getSize()) {
						bs->addBoard(b);
						break;
					}
				}
			}
			*/

			std::size_t boardsToloadIndex = 0;
			std::size_t boardStorageIndex = 0;
			auto bs = _boardsStorage[boardStorageIndex++];
			
			while (boardsToloadIndex < boardsToLoad.size()) {
				auto b = boardsToLoad[boardsToloadIndex];
				if (bs->getSize() == b->getSize()) {
					bs->addBoard(b);
					boardsToloadIndex++;
					continue;
				}
				else {
					bs = _boardsStorage[boardStorageIndex++];
				}

			}

			CCLOG("BOARD MANAGER >> DONE...");
			return true;
		}
		CCLOG("BOARD MANAGER >> NOT LOADED...");
		return false;
	}

	boardPtr BoardManager::getBoard(const Size& size, const size_t index) {
		boardPtr result = nullptr;
		for (auto bs : _boardsStorage) {
			auto bsSize = bs->getSize();
			if (bsSize == size) {
				return bs->getBoard(index);
			}
		}
		CCASSERT(result, "Board manager error");
		return nullptr;
	}

#if GENERATE_BOARDS

	const int _generateCount[6] = BOARDS_TO_GENERATE;

	std::shared_ptr<BoardManager> BoardManager::getInstance() {
		if (!_sharedBoardManager) {
			CCLOG("BOARD MANAGER >> CREATING INSTANCE...");
			_sharedBoardManager = std::shared_ptr<BoardManager>(new (std::nothrow) BoardManager());
			_sharedBoardManager->generateBoards();
			auto loaded = _sharedBoardManager->init();
			if (!loaded) {
				CCLOG("BOARD MANAGER >> LOADING FAIL");
				return nullptr;
			}
		}
		return _sharedBoardManager;
	}

	void BoardManager::generateBoards() {
		CCLOG("BOARD STORAGE EMPTY SIZE... %d", _boardsStorage.size());
		CCLOG("GENERATE BOARDS...");
		const Size sizes[6] = BOARDS_SIZES;
		for (auto i = 0; i != 6; ++i) {
			CCLOG("GENERATING %d, %d...", sizes[i].width, sizes[i].height);
			_boardsStorage[i]->addMoreBoards(_generateCount[i]);
		}
		DataHandler::getInstance()->saveBoards(_boardsStorage);
	}
#else
	std::shared_ptr<BoardManager> BoardManager::getInstance() {
		if (!_sharedBoardManager) {
			CCLOG("BOARD MANAGER >> CREATING INSTANCE...");
			_sharedBoardManager = std::shared_ptr<BoardManager>(new (std::nothrow) BoardManager());
			auto loaded = _sharedBoardManager->init();
			if (!loaded) {
				CCLOG("BOARD MANAGER >> LOADING FAIL");
				return nullptr;
			}
		}
		return _sharedBoardManager;
	}
#endif
}
