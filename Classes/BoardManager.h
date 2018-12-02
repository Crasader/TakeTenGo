#pragma once
#ifndef __BOARD_MANAGER_H__
#define __BOARD_MANAGER_H__

#include <vector>
#include "Board.h"
#include "BoardsStorageContainer.h"

namespace TakeTen {

	class BoardManager {
		typedef std::shared_ptr<BoardsStorageContainer> boardStoragePtr;
		typedef std::vector <boardStoragePtr> vectorOfBoardStorage;

	public:
		~BoardManager();
		static std::shared_ptr<BoardManager> getInstance();
		bool init();
		boardPtr getBoard(const Size& size, const size_t index);
	
	private:
		BoardManager();
		vectorOfBoardStorage _boardsStorage;
		static std::shared_ptr<BoardManager> _sharedBoardManager;

#if GENERATE_BOARDS
	public:
		void generateBoards();
#endif
	};
}


#endif