#include "MainMenuScene.h"
#include "PauseScene.h"
#include "DataHandler.h"
#include "GameManager.h"
#include "LanguageManager.h"

#include "BackgroundNode.h"


#include "GameScene.h"

namespace TakeTen {
	GameScene::~GameScene() {
		CCLOG("GAME SCENE DESTRUCTOR");

		removeAllChildrenWithCleanup(true);
		removeFromParentAndCleanup(true);

		//CC_SAFE_RELEASE_NULL(_boardNode);
	}

	cocos2d::Scene* GameScene::createScene(const Size& size) {
		auto scene = cocos2d::Scene::create();
		auto layer = GameScene::create(size);
		scene->addChild(layer);
		return scene;
	}

	GameScene* GameScene::create(const Size& size) {
		GameScene *pRet = new (std::nothrow) GameScene();
		if (pRet && pRet->init(size)) {
			pRet->autorelease();
			return pRet;
		}
		else {
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}

	void GameScene::onExit() {
		LayerColor::onExit();
		TakeTen::GameManager::getInstance()->save();
	}

	bool GameScene::init(const Size& size) {
		if (!LayerColor::initWithColor(cocos2d::Color4B(51, 51, 51, 255))) {
			return false;
		}

		auto bg = BackgroundNode::create();
		this->addChild(bg);

		auto listener = cocos2d::EventListenerKeyboard::create();
		listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		_boardNode = BoardNode::create(CC_CALLBACK_0(GameScene::congratulations, this));
		this->addChild(_boardNode, 10);

		_gameTimer = 0.0f;
		_animationDelayTimer = 0.0f;

		createLabels();
		createMenuButtons();
		createBoard();

		scheduleUpdate();
		return true;
	}

	void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
			menuPause(nullptr);
#else
			menuQuit(nullptr);
#endif
		}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_P) {
			CCLOG("CAPTURE SCREEN");
			captureScreen();
		}
#endif
	}

	void GameScene::createLabels() {
		_timeLabel = cocos2d::Label::createWithBMFont(UI_FONT_FILE_PATH, "TIME");
		_timeLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
		_timeLabel->setColor(cocos2d::Color3B(198, 163, 117));
		_timeLabel->setScale(1.0f * FONT_SCALE_FACTOR);
		addChild(_timeLabel, 1);
	}

	void GameScene::createMenuButtons() {
		auto menuPos = cocos2d::Vec2(0, MENU_BUTTONS_Y);

		cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;
		auto pressedColor = ColorHSB(9, 25, 127);

		_undoMenu = TakeTen::MenuButton::create(ICON_MENU_UNDO, pressedColor, _animationDelayTimer += ANIMATION_DELAY_TIME, CC_CALLBACK_1(GameScene::menuUndo, this));
		_pauseMenu = TakeTen::MenuButton::create(ICON_MENU_PAUSE, pressedColor, _animationDelayTimer += ANIMATION_DELAY_TIME, CC_CALLBACK_1(GameScene::menuPause, this));
		_quitMenu = TakeTen::MenuButton::create(ICON_MENU_QUIT, pressedColor, _animationDelayTimer += ANIMATION_DELAY_TIME, CC_CALLBACK_1(GameScene::menuQuit, this));

		_undoMenu->setPosition(menuPos.x - 200, menuPos.y);
		_pauseMenu->setPosition(menuPos.x, menuPos.y);
		_quitMenu->setPosition(menuPos.x + 200, menuPos.y);

		pMenuItems.pushBack(_undoMenu);
		pMenuItems.pushBack(_pauseMenu);
		pMenuItems.pushBack(_quitMenu);

		auto pMenu = cocos2d::Menu::createWithArray(pMenuItems);

		addChild(pMenu, 1);
	}

	void GameScene::createBoard() {

		_won = false;
		_animationDelayTimer = 0;

		auto gameManager = GameManager::getInstance();
		auto nextBoard = gameManager->getNextBoard();

		_gameTimer = gameManager->getTime();
		_boardNode->createBoard(nextBoard);

		updateMenuButtons();
		updateLabelsPositions();
		updateTimeLabel();

		gameManager->setPause(false);
		_pauseMenu->enable();
	}

	void GameScene::updateLabelsPositions() {
		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		auto offset = cocos2d::Vec2(-_boardNode->getContentSize().width / 2, _boardNode->getContentSize().height / 2);
		auto timeLabelPos = _boardNode->getPosition() + origin + offset;
		_timeLabel->setAnchorPoint(cocos2d::Vec2(0, 0.17f));
		_timeLabel->setPosition(timeLabelPos);
	}

	void GameScene::introAnimationCompleted() {
		GameManager::getInstance()->setPause(false);
		_pauseMenu->enable();
	}

	void GameScene::newGame() {

		if (GameManager::getInstance()->getDifficulty() == GameDifficuty::Completed) {
			menuQuit(nullptr);
			return;
		}

		_boardNode->hideSelection();
		createBoard();
	}

	void GameScene::congratulations() {

		auto actionDelay = cocos2d::DelayTime::create(CONGRATULATION_TIME);
		auto newGameAction = cocos2d::CallFunc::create(CC_CALLBACK_0(GameScene::newGame, this));

		auto sequence = cocos2d::Sequence::create(actionDelay, newGameAction, nullptr);

		auto gameManager = GameManager::getInstance();
		gameManager->won();
		gameManager->setPause();

		runAction(sequence);
		_won = true;
	}

	void GameScene::update(float dt) {

		static float secondsTimer = 1.0f;

		if (_won || GameManager::getInstance()->isPause()) {
			secondsTimer = 0;
			return;
		}

		secondsTimer += dt;
		_gameTimer += dt;

		if (secondsTimer >= 1) {
			updateTimeLabel();
			secondsTimer = 0.0f;
		}
	}

	void GameScene::updateMenuButtons() {
		if (_boardNode->getBoard()->canUndo()) {
			_undoMenu->enable();
		}
		else {
			_undoMenu->disable();
		}
	}

	void GameScene::updateTimeLabel() {
		auto timeString = Tools::getTimeString(static_cast<int>(_gameTimer));
		GameManager::getInstance()->setTime(_gameTimer);
		_timeLabel->setString(timeString);
		updateMenuButtons();
	}

	void GameScene::menuPause(Ref* pSender) {
		GameManager::getInstance()->soundSelect();
		auto pauseScene = PauseScene::createScene();
		cocos2d::Director::getInstance()->pushScene(SCENE_TRANSITION(TRANSITION_TIME, pauseScene));
	}

	void GameScene::menuUndo(Ref* pSender) {

		auto board = _boardNode->getBoard();

		if (board->canUndo()) {
			GameManager::getInstance()->soundUndo();
			_gameTimer += UNDO_COST;
			_boardNode->rebuildBoard(true);
			updateTimeLabel();
			updateMenuButtons();
		}
	}

	void GameScene::menuQuit(Ref* pSender) {
		GameManager::getInstance()->soundSelect();
		if (Tools::randBool()) {
			if (ScoreManager::getInstance()->showFullscreenAds()) {
				AdmobHelper::showFullscreenAd();
			}
		}
		auto gameScene = MainMenuScene::createScene();
		cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, gameScene));
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	void GameScene::afterScreenCaptured(bool succeed, const std::string& outputFile)
	{
		if (succeed) {
			CCLOG("Capture screen done %s", outputFile.c_str());
		}
		else {
			CCLOG("Capture screen failed.");
		}
	}

	void GameScene::captureScreen() {
		cocos2d::utils::captureScreen(CC_CALLBACK_2(GameScene::afterScreenCaptured, this), "CaptureScreenTest.png");
	}
#endif
}

