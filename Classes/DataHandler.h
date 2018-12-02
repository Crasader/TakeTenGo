#pragma once
#ifndef __DATA_HANDLER_H__
#define __DATA_HANDLER_H__

#include <fstream>
#include "BoardsStorageContainer.h"

namespace TakeTen {

	typedef std::shared_ptr<BoardsStorageContainer> boardStoragePtr;
	typedef std::vector <boardStoragePtr> vectorOfBoardStorage;
	
	typedef std::shared_ptr<Board> boardPtr;
	typedef std::vector <boardPtr> vectorOfBoards;

	class DataHandler
	{
	public:

		static std::shared_ptr<DataHandler> getInstance();
		bool readBoards(vectorOfBoards& boards);

#if GENERATE_BOARDS
		bool saveBoards(const vectorOfBoardStorage& boards);
#endif

	private:

		static bool deserialize(std::istream &str, vectorOfBoards& boards);
		static std::shared_ptr<DataHandler> _sharedDataHandler;
	};
}



#endif