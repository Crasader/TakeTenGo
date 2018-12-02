#include "DataHandler.h"
#include "cocos2d.h"
#include <iostream>
#include <fstream>

namespace TakeTen {

	static std::shared_ptr<DataHandler> _sharedDataHandler = nullptr;

	std::shared_ptr<DataHandler> DataHandler::getInstance() {
		if (!_sharedDataHandler) {
			_sharedDataHandler = std::shared_ptr<DataHandler>(new(std::nothrow) DataHandler());
			CCASSERT(_sharedDataHandler, "FATAL: Not enough memory");
		}
		return _sharedDataHandler;
	}

	std::string DataHandler::getFilePath(const std::string& fileName) {
		return cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
	}

	bool DataHandler::saveBoards(const vectorOfBoardStorage& boards) {

		std::ofstream outBoard(getFilePath("boards.dat"), std::ios::out | std::ios::trunc | std::ios::binary);

		if (!outBoard) {
			return false;
		} 
		
		CCLOG("SAVING...");

		int totalBoards = boards.size();
		outBoard.write(reinterpret_cast<const char*>(&totalBoards), sizeof(totalBoards));

		CCLOG("TOTAL SIZES == %d...", totalBoards);

		for (const auto& bs : boards) {
			Size boardSize = bs->getSize();
			outBoard.write(reinterpret_cast<const char*>(&boardSize), sizeof(Size));
			int sizeCount = bs->getBoards().size();
			outBoard.write(reinterpret_cast<const char*>(&sizeCount), sizeof(int));

			CCLOG("TOTAL %d BOARDS OF SIZE %d, %d SAVED", sizeCount, boardSize.getWidht(), boardSize.getHeight());
			for (auto& b : bs->getBoards()) {
				std::vector<char> values;
				for (auto& c : b->getCells()) {
					values.push_back(c.getValue());
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

	bool DataHandler::readBoards(vectorOfBoards& boards) {

		std::ifstream inBoard(getFilePath("boards.dat"), std::ios::in | std::ios::binary);

		if (!inBoard) {
			CCLOG("LOADING FAILED");
			return false;
		} // end if

		CCLOG("LOADING...");

		int totalSizesCount;
		inBoard.read(reinterpret_cast<char*>(&totalSizesCount), sizeof(int));

		CCLOG("TOTAL SIZES STORED == %d", totalSizesCount);

		for (auto i = 0; i != totalSizesCount; ++i) {
			Size boardSize;
			inBoard.read(reinterpret_cast<char*>(&boardSize), sizeof(Size));
			int sizeCount;
			inBoard.read(reinterpret_cast<char*>(&sizeCount), sizeof(int));
			CCLOG("TOTAL %d BOARDS OF SIZE %d, %d STORED...", sizeCount, boardSize.getWidht(), boardSize.getHeight());

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
		CCLOG("%d BOARD LOADED...", boards.size());
		return true;
	}

}

