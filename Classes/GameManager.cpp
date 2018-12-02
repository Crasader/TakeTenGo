#include "GameManager.h"
#include "BoardManager.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Size.h"
#include "cocos2d.h"

namespace TakeTen {
	
	GameProgress::GameProgress()
		: size(0, 0), index(0), time(0) { }

	GameProgress::GameProgress(const TakeTen::Size& newSize, char newIndex, float newTime)
		: size(newSize), index(newIndex), time(newTime) { }

	static std::shared_ptr<GameManager> _sharedGameManager = nullptr;

	GameManager::GameManager() {
		_gameProgress[0] = GameProgress(Size(3, 3), 0, 0);
		_gameProgress[1] = GameProgress(Size(3, 4), 0, 0);
		_gameProgress[2] = GameProgress(Size(3, 5), 0, 0);
		_gameProgress[3] = GameProgress(Size(4, 4), 0, 0);
		_gameProgress[4] = GameProgress(Size(4, 5), 0, 0);
		_gameProgress[5] = GameProgress(Size(5, 5), 0, 0);
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
			_sharedGameManager = std::shared_ptr<GameManager>(new (std::nothrow) GameManager());
			CCASSERT(_sharedGameManager, "FATAL: Not enough memory");
			_sharedGameManager->init();
		}
		return _sharedGameManager;
	}

	void GameManager::init() {
		BoardManager::getInstance();

		if (!load()) {
			CCLOG("SAVING EMPTY...");
			save();
		}
	}

	std::string GameManager::getFilePath(const std::string& fileName) {
		return cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
	}

	bool GameManager::save() {
		std::ofstream outBoard(getFilePath("player.dat"), std::ios::out | std::ios::trunc | std::ios::binary);
		if (!outBoard) {
			return false;
		}
		CCLOG("SAVING PLAYER...");
		for (auto i = 0; i != 6; ++i) {
			CCLOG("----SIZE %d, %d, INDEX == %d ---- TIME == %f", _gameProgress[i].size.getWidht(), _gameProgress[i].size.getHeight(), _gameProgress[i].index, _gameProgress[i].time);
			outBoard.write(reinterpret_cast<char*>(&_gameProgress[i]), sizeof(GameProgress));
		}
		return true;
	}

	bool GameManager::load() {

		auto boardManager = BoardManager::getInstance();

		std::ifstream inBoard(getFilePath("player.dat"), std::ios::in | std::ios::binary);
		if (!inBoard) {
			CCLOG("LOADING PLAYER FAILED");
			return false;
		}
		CCLOG("LOADING PLAYER...");
		for (auto i = 0; i != 6; ++i) {
			inBoard.read(reinterpret_cast<char*>(&_gameProgress[i]), sizeof(GameProgress));
			CCLOG("----SIZE %d, %d, INDEX == %d ---- TIME == %f", _gameProgress[i].size.getWidht(), _gameProgress[i].size.getHeight(), _gameProgress[i].index, _gameProgress[i].time);
		}
		return true;
	}

	void GameManager::won() {
		_gameProgress[_currentGameDifficulty].index++;
		if (_gameProgress[_currentGameDifficulty].index == 100) {
			//_gameProgress[_currentGameDifficulty].index = 0;
			_currentGameDifficulty = nextDiffuculty(_currentGameDifficulty);
			CCLOG("NEXT DIFFICULTY");
		}
		save();
	}

	void GameManager::newGame(GameDifficuty difficulty) {

		if (_gameProgress[difficulty].index == 100) {
			return;
		}

		if (_currentGameDifficulty == TakeTen::Completed) {
			auto gameScene = MainMenuScene::createScene();
			cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, gameScene));
		}

		auto boardManager = BoardManager::getInstance();

		_currentGameDifficulty = difficulty;

		auto size = _gameProgress[difficulty].size;
		auto gameScene = GameScene::createScene(size);

		cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, gameScene));
	}

}


