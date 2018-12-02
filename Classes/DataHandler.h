#ifndef __DATA_HANDLER_H__
#define __DATA_HANDLER_H__

#include <string>
#include <vector>
#include <memory>

#include "BoardsStorage.h"

namespace TakeTen {

	typedef std::shared_ptr<BoardsStorage> boardStoragePtr;
	typedef std::vector <boardStoragePtr> vectorOfBoardStorage;
	
	typedef std::shared_ptr<Board> boardPtr;
	typedef std::vector <boardPtr> vectorOfBoards;

	class DataHandler
	{
	public:

		static std::shared_ptr<DataHandler> DataHandler::getInstance();

		bool saveBoards(const vectorOfBoardStorage& boards);
		bool readBoards(vectorOfBoards& boards);

	private:
		std::string getFilePath(const std::string& fileName);
	};
}



#endif