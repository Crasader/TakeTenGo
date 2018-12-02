#include <fstream>

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Definitions.h"
#include "BoardManager.h"
#include "GameScene.h"
#include "MainMenuScene.h"

#include "ScoreManager.h"

#include "GameManager.h"

namespace TakeTen {
	
	std::shared_ptr<GameManager> GameManager::_sharedGameManager = nullptr;

	GameManager::GameManager() : _isFirstRun(true), _isSoundOn(true), _isPause(false) {
		const Size sizes[6] = BOARDS_SIZES;
		for (auto i = 0; i != 6; ++i)
		{
			_gameProgress[i] = GameProgressContainer(sizes[i], 0, 0);
		}
	}

	void GameManager::setTime(float time) {
		_gameProgress[_currentGameDifficulty].time = time;
	}

	boardPtr GameManager::getNextBoard() {
		auto boardSize = _gameProgress[_currentGameDifficulty].size;
		auto index = _gameProgress[_currentGameDifficulty].index;
		auto result = TakeTen::BoardManager::getInstance()->getBoard(boardSize, index);
		return result;
	}

	float GameManager::getTime() const {
		return _gameProgress[_currentGameDifficulty].time;
	}

	GameManager::~GameManager() {
	}
	
	std::shared_ptr<GameManager> GameManager::getInstance() {
		if (!_sharedGameManager) {
			CCLOG("GAME MANAGER >> CREATING INSTANCE...");
			_sharedGameManager = std::shared_ptr<GameManager>(new (std::nothrow) GameManager());
			_sharedGameManager->init();
		}
		return _sharedGameManager;
	}

	void GameManager::init() {

		CCLOG("GAME MANAGER >> CREATING BOARD MANAGER...");
		BoardManager::getInstance();

		CCLOG("GAME MANAGER >> CREATING SCORE MANAGER...");
		ScoreManager::getInstance();
		if (!load()) {
			CCLOG("SAVING EMPTY...");
			save();
		}

		setSound(_isSoundOn);

		
	}

	void GameManager::setSound(bool sound) {
		_isSoundOn = sound;
	}

	void GameManager::soundSelect(bool force) {
		if (_isSoundOn || force) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_TICK);
		}
	}

	void GameManager::soundUndo() {
		if (_isSoundOn) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_UNDO);
		}
	}

	void GameManager::soundRemove() {
		if (_isSoundOn) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_REMOVE);
		}
	}

	void GameManager::soundWon() {
		if (_isSoundOn) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_WON);
		}
	}
	
	bool GameManager::save() {
		std::ofstream outBoard(PLAYER_FILE, std::ios::out | std::ios::trunc | std::ios::binary);
		if (!outBoard) {
			return false;
		}
		outBoard.write(reinterpret_cast<char*>(&_isFirstRun), sizeof(bool));
		outBoard.write(reinterpret_cast<char*>(&_isSoundOn), sizeof(bool));
		for (auto i = 0; i != 6; ++i) {
			outBoard.write(reinterpret_cast<char*>(&_gameProgress[i]), sizeof(GameProgressContainer));
		}
		return true;
	}

	bool GameManager::load() {

		auto playerFile = "player.dat";
		std::ifstream inBoard(PLAYER_FILE, std::ios::in | std::ios::binary);
		if (!inBoard) {
			CCLOG("GAME MANAGER >> LOADING PLAYER FAILED");
			return false;
		}
		CCLOG("GAME MANAGER >> LOADING PLAYER...");
		
		inBoard.read(reinterpret_cast<char*>(&_isFirstRun), sizeof(bool));
		inBoard.read(reinterpret_cast<char*>(&_isSoundOn), sizeof(bool));

		auto scoreManager = ScoreManager::getInstance();

		for (auto i = 0; i != 6; ++i) {
			inBoard.read(reinterpret_cast<char*>(&_gameProgress[i]), sizeof(GameProgressContainer));
		}
		const Size sizes[6] = BOARDS_SIZES;
		for (auto i = 0; i != 6; ++i)
		{
			if (_gameProgress[i].size != sizes[i])
			{
				_gameProgress[i].size = sizes[i];
				_gameProgress[i].index = 0;
				_gameProgress[i].time = 0;
			}

		}

		CCLOG("GAME MANAGER >> DONE...");

		return true;
	}

	void GameManager::won() {
		soundWon();

		_gameProgress[_currentGameDifficulty].index++;

		save();
		const size_t boards[6] = BOARDS_TO_GENERATE;
		if (_gameProgress[_currentGameDifficulty].index == boards[_currentGameDifficulty]) {
			auto time = static_cast<int>(_gameProgress[_currentGameDifficulty].time);
			ScoreManager::getInstance()->postScore(_currentGameDifficulty, time);
			ScoreManager::getInstance()->update();
			_currentGameDifficulty = nextDiffuculty(_currentGameDifficulty);
			CCLOG("NEXT DIFFICULTY");
		}
		
	}

	void GameManager::resetGame(GameDifficuty difficulty) {

		CCLOG("RESET GAME %d", difficulty);

		_gameProgress[difficulty].index = 0;
		_gameProgress[difficulty].time = 0.0f;

		newGame(difficulty);
	}

	void GameManager::newGame(GameDifficuty difficulty) {
		const size_t boards[6] = BOARDS_TO_GENERATE;
		if (_gameProgress[difficulty].index == boards[difficulty]) {
			CCLOG("GAME MANAGER >> NEW GAME RETURN");
			return;
		}

		_isFirstRun = false;

		if (_currentGameDifficulty == TakeTen::Completed) {
			auto mainMenu = MainMenuScene::createScene();
			cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, mainMenu));
		}

		_currentGameDifficulty = difficulty;

		auto size = _gameProgress[difficulty].size;
		auto gameScene = GameScene::createScene(size);

		cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, gameScene));
	}

}


