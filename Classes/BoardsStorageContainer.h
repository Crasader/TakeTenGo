#pragma once
#ifndef __BOARDS_STORAGE_H__
#define __BOARDS_STORAGE_H__

#include "Size.h"
#include "Board.h"

namespace TakeTen {

	class BoardsStorageContainer {
	public:
		BoardsStorageContainer(const Size& size);
		BoardsStorageContainer(const Size& size, const vectorOfBoards& boards);
		~BoardsStorageContainer();

		Size& getSize();
		inline vectorOfBoards getBoards() { return _boards; }
		inline boardPtr getBoard(const size_t index) { 
			return boardPtr(new Board(*_boards[index]));
		}

		bool addBoard(const boardPtr& board);
	private:
		BoardsStorageContainer();

		Size _size;
		vectorOfBoards _boards;

#if GENERATE_BOARDS
	public:
		void addMoreBoards(int count);
#endif
	};
}


#endif