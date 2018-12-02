#include "cocos2d.h"
#include "Definitions.h"
#include "Tools.h"
#include "BoardsStorageContainer.h"
#include "DataHandler.h"

namespace TakeTen {

	std::shared_ptr<DataHandler> DataHandler::_sharedDataHandler = nullptr;

	std::shared_ptr<DataHandler> DataHandler::getInstance() {

		if (!_sharedDataHandler) {
			CCLOG("DATA HANDLER >> CREATING INSTANCE...");
			_sharedDataHandler = std::shared_ptr<DataHandler>(new(std::nothrow) DataHandler);
			CCASSERT(_sharedDataHandler, "FATAL: Not enough memory");
		}
		return _sharedDataHandler;
	}

	bool DataHandler::deserialize(std::istream &inBoard, vectorOfBoards& boards) {
		int totalSizesCount;
		inBoard.read(reinterpret_cast<char*>(&totalSizesCount), sizeof(int));

		CCLOG("DATA HANDLER >> TOTAL SIZES STORED == %d", totalSizesCount);

		auto resave = false;

		for (auto i = 0; i != totalSizesCount; ++i) {
			Size boardSize;
			inBoard.read(reinterpret_cast<char*>(&boardSize), sizeof(TakeTen::Size));
			int sizeCount;
			inBoard.read(reinterpret_cast<char*>(&sizeCount), sizeof(int));
			CCLOG("TOTAL %d BOARDS OF SIZE %d, %d STORED...", sizeCount, boardSize.width, boardSize.height);

			if (sizeCount == 0) {
				resave = true;
				continue;
			}

			for (auto o = 0; o != sizeCount; ++o) {
				int cellCount;
				char cells[100];
				inBoard.read(reinterpret_cast<char*>(&cellCount), sizeof(int));
				inBoard.read(reinterpret_cast<char*>(&cells[0]), sizeof(char) * cellCount);
				std::vector<char> values;
				for (auto c = 0; c != cellCount; ++c) {
					values.push_back(cells[c]);
				}

				boardPtr newBoard(new Board(boardSize, values));
				boards.push_back(newBoard);
			}
		}

		return resave;
	}

	bool DataHandler::readBoards(vectorOfBoards& boards) {
		CCLOG("DATA HANDLER >> LOADING BOARDS...");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		auto data = cocos2d::FileUtils::getInstance()->getDataFromFile(BOARDS_FILE);
		std::stringstream inBoard;
		inBoard.write(reinterpret_cast<char*>(data.getBytes()), data.getSize());
		deserialize(inBoard, boards);
		inBoard.clear();
#else
		std::ifstream inBoard(BOARDS_FILE, std::ios::in | std::ios::binary);
		if (!inBoard) {
			CCLOG("DATA HANDLER >> LOADING BOARDS FAILED...");
			return false;
		}

#if GENERATE_BOARDS
		auto resave = deserialize(inBoard, boards);
		inBoard.close();

		if (resave) {
			Size sizes[6] = { Size(3, 3), Size(3, 4), Size(3, 5), Size(4, 4), Size(4, 5), Size(5, 5) };
			typedef std::shared_ptr<BoardsStorageContainer> boardStoragePtr;
			typedef std::vector <boardStoragePtr> vectorOfBoardStorage;

			vectorOfBoardStorage containers;

			for (auto i = 0; i != 6; ++i) {
				boardStoragePtr storage(new BoardsStorageContainer(sizes[i]));
				containers.push_back(storage);
			}

			for (auto b : boards) {
				for (auto s : containers) {
					if (b->getSize().width == s->getSize().width) {
						if (b->getSize().height == s->getSize().height) {
							s->addBoard(b);
						}
					}
				}
			}
			saveBoards(containers);
		}
#else
		deserialize(inBoard, boards);
		inBoard.close();
#endif
#endif
		CCLOG("DATA HANDLER >> SUCCEED %d BOARD LOADED...", boards.size());
		return true;
	}

#if GENERATE_BOARDS
	bool DataHandler::saveBoards(const vectorOfBoardStorage& boards) {

		std::ofstream outBoard(BOARDS_FILE, std::ios::out | std::ios::trunc | std::ios::binary);

		if (!outBoard) {
			CCLOG("DATA HANDLER >> ERROR SAVING BOARDS");
			return false;
		}

		CCLOG("DATA HANDLER >> SAVING...");

		int totalBoards = boards.size();
		outBoard.write(reinterpret_cast<const char*>(&totalBoards), sizeof(totalBoards));

		CCLOG("TOTAL SIZES == %d...", totalBoards);

		for (const auto& bs : boards) {
			Size boardSize = bs->getSize();
			outBoard.write(reinterpret_cast<const char*>(&boardSize), sizeof(Size));
			int sizeCount = bs->getBoards().size();
			outBoard.write(reinterpret_cast<const char*>(&sizeCount), sizeof(int));

			CCLOG("TOTAL %d BOARDS OF SIZE %d, %d SAVED", sizeCount, boardSize.width, boardSize.height);
			for (auto& b : bs->getBoards()) {
				std::vector<char> values;
				for (auto& c : b->getCells()) {
					values.push_back(c.value);
				}
				int cellCount = values.size();
				outBoard.write(reinterpret_cast<const char*>(&cellCount), sizeof(int));
				outBoard.write(reinterpret_cast<const char*>(&values[0]), sizeof(char) * cellCount);
			}
		}
		outBoard.close();
		CCLOG("BOARDS SAVED...");
		CCLOG("*********************");
		return true;
	}
#endif

}

