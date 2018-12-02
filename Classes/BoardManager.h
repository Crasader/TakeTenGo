#ifndef __BOARD_MANAGER_H__
#define __BOARD_MANAGER_H__

#include <vector>
#include "Board.h"
#include "BoardsStorage.h"

namespace TakeTen {

	typedef std::shared_ptr<BoardsStorage> boardStoragePtr;
	typedef std::vector <boardStoragePtr> vectorOfBoardStorage;

	typedef std::shared_ptr<Board> boardPtr;
	typedef std::vector <boardPtr> vectorOfBoards;

	class BoardManager
	{
	public:
		static std::shared_ptr<BoardManager> BoardManager::getInstance();

		bool init();
		boardPtr getBoard(const Size& size, const size_t index);

		void generateBoards();
	private:

		void fillWithEmpty();
		vectorOfBoardStorage _boardsStorage;

	};
}


#endif