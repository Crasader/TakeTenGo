#ifndef __BOARDS_STORAGE_H__
#define __BOARDS_STORAGE_H__

#include <memory>
#include <vector>
#include "Size.h"
#include "Board.h"
#include "cocos2d.h"

namespace TakeTen {
	typedef std::shared_ptr<Board> boardPtr;
	typedef std::vector <boardPtr> vectorOfBoards;

	class BoardsStorage {
	public:
		BoardsStorage(const Size& size);
		BoardsStorage(const Size& size, const vectorOfBoards& boards);
		~BoardsStorage();

		Size& getSize();
		inline vectorOfBoards getBoards() { return _boards; }
		inline boardPtr getBoard(const size_t index) { return _boards[index]; }

		bool addBoard(const boardPtr& board);
		void addMoreBoards();

	private:
		BoardsStorage();

		Size _size;
		vectorOfBoards _boards;
	};
}


#endif