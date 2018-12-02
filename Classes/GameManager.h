#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "Size.h"
#include "Board.h"
#include "Definitions.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <string>

namespace TakeTen {

	struct GameProgress {
		GameProgress();
		GameProgress(const TakeTen::Size& size, char index, float time);
		TakeTen::Size size;
		size_t index;
		float time;
	};

	typedef std::shared_ptr<Board> boardPtr;

	class GameManager {
	public:
		~GameManager();
		static std::shared_ptr<GameManager> GameManager::getInstance();

		bool save();
		void newGame(GameDifficuty difficulty);

		void setTime(float time);

		inline float getTime(size_t index) const {
			return _gameProgress[index].time;
		}

		float getTime() const;
		boardPtr getNextBoard();

		void won();

		inline size_t getIndex() const {
			return _gameProgress[_currentGameDifficulty].index;
		}

		inline size_t getIndex(size_t boardIndex) const {
			return _gameProgress[boardIndex].index;
		}

		static inline GameDifficuty nextDiffuculty(GameDifficuty currentDifficulty) {
			switch (currentDifficulty)
			{
			case TakeTen::Practice:
				return TakeTen::Easy;
			case TakeTen::Easy:
				return TakeTen::Medium;
			case TakeTen::Medium:
				return TakeTen::Hard;
			case TakeTen::Hard:
				return TakeTen::Ultimate;
			case TakeTen::Ultimate:
				return TakeTen::Hardcore;
			}
			return TakeTen::Practice;
		}



	private:
		GameManager();

		std::string getFilePath(const std::string& fileName);

		void init();
		bool load();

		GameProgress _gameProgress[6];
		GameDifficuty _currentGameDifficulty;
	};
}

#endif