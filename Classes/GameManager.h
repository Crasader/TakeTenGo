#pragma once
#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "AdmobHelper.h"
#include "GameProgressContainer.h"
#include "ScoreManager.h"
#include "Board.h"

namespace TakeTen {

	typedef std::shared_ptr<Board> boardPtr;

	class GameManager {
	public:
		~GameManager();
		static std::shared_ptr<GameManager> getInstance();

		void setSound(bool sound);

		inline void switchSound() {
			setSound(!_isSoundOn);
		}

		void soundSelect(bool force = false);
		void soundRemove();
		void soundUndo();
		void soundWon();

		bool save();
		void newGame(GameDifficuty difficulty);
		void resetGame(GameDifficuty difficulty);

		void setTime(float time);

		inline float getTime(size_t index) const {
			return _gameProgress[index].time;
		}

		float getTime() const;
		boardPtr getNextBoard();

		void won();

		inline bool isSound() const {
			return _isSoundOn;
		}

		inline bool isFirstRun() const {
			return _isFirstRun;
		}

		inline size_t getIndex() const {
			return _gameProgress[_currentGameDifficulty].index;
		}

		inline size_t getIndex(size_t boardIndex) const {
			return _gameProgress[boardIndex].index;
		}

		inline GameDifficuty getDifficulty() const {
			return _currentGameDifficulty;
		}

		static inline GameDifficuty nextDiffuculty(GameDifficuty currentDifficulty) {
			switch (currentDifficulty) {
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
			default:
				return TakeTen::Completed;
			}
			
		}

		inline bool isPause() const {
			return _isPause;
		}

		inline void setPause(bool pause = true) {
			_isPause = pause;
		}

		bool _boardsLoaded;

	private:
		GameManager();

		void init();
		bool load();

		bool _isFirstRun;
		bool _isSoundOn;

		bool _isPause;

		GameProgressContainer _gameProgress[6];
		GameDifficuty _currentGameDifficulty;

		static std::shared_ptr<GameManager> _sharedGameManager;
	};
}

#endif