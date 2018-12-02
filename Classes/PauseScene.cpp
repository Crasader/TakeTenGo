#include "Definitions.h"
#include "GameManager.h"
#include "LanguageManager.h"
#include "MainMenuScene.h"
#include "ScoreManager.h"

#include "BackgroundNode.h"

#include "PauseScene.h"

namespace TakeTen {
	cocos2d::Scene* PauseScene::createScene() {
		auto scene = cocos2d::Scene::create();
		auto layer = PauseScene::create();
		scene->addChild(layer);
		return scene;
	}

	bool PauseScene::init() {
		if (!LayerColor::initWithColor(cocos2d::Color4B(51, 51, 51, 255))) {
			return false;
		}

		auto bg = BackgroundNode::create();
		this->addChild(bg);

		auto alignment = cocos2d::TextHAlignment::CENTER;
		auto color = cocos2d::Color3B(198, 163, 117);
		auto gameManager = GameManager::getInstance();

		auto listener = cocos2d::EventListenerKeyboard::create();
		listener->onKeyReleased = CC_CALLBACK_2(PauseScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		auto spriteBatch = cocos2d::SpriteBatchNode::create(SPRITE_SHIT_PNG);
		auto cache = cocos2d::SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(SPRITE_SHIT_PLIST);
		this->addChild(spriteBatch);
		
		auto pauseLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, DIFFICULTY(gameManager->getDifficulty()));
		//auto pauseLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, PAUSE_STRING);
		pauseLabel->setAlignment(alignment);
		pauseLabel->setColor(color);
		pauseLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 700);
		pauseLabel->setScale(FONT_SCALE_FACTOR);
		addChild(pauseLabel, 1);

		color = cocos2d::Color3B(112, 135, 147);

		auto progressStringLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, PAUSE_PROGRESS);
		progressStringLabel->setAlignment(alignment);
		progressStringLabel->setColor(color);
		progressStringLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 510);
		progressStringLabel->setScale(0.6f * FONT_SCALE_FACTOR);
		addChild(progressStringLabel, 1);

		auto gameindex = static_cast<float>(gameManager->getIndex());
		const size_t boards[6] = BOARDS_TO_GENERATE;
		auto totalBoards = static_cast<float>(boards[gameManager->getDifficulty()]);
		auto progress = static_cast<int>((gameindex / totalBoards) * 100.0f);
		auto progressStr = std::to_string(progress) + "%";

		auto progressLabel = cocos2d::Label::createWithBMFont(UI_FONT_FILE_PATH, progressStr);
		progressLabel->setAlignment(alignment);
		progressLabel->setColor(color);
		progressLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 390);
		progressLabel->setScale(FONT_SCALE_FACTOR);
		addChild(progressLabel, 1);

		color = cocos2d::Color3B(92, 119, 103);

		auto timeStringLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, PAUSE_TIME);
		timeStringLabel->setAlignment(alignment);
		timeStringLabel->setColor(color);
		timeStringLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 210);
		timeStringLabel->setScale(0.6f * FONT_SCALE_FACTOR);
		addChild(timeStringLabel, 1);

		auto time = static_cast<int>(gameManager->getTime());
		auto timeStr = Tools::getTimeString(time);

		auto timeLabel = cocos2d::Label::createWithBMFont(UI_FONT_FILE_PATH, timeStr);
		timeLabel->setAlignment(alignment);
		timeLabel->setColor(color);
		timeLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 90);
		timeLabel->setScale(FONT_SCALE_FACTOR);
		addChild(timeLabel, 1);

		color = cocos2d::Color3B(147, 76, 68);

		auto difficulty = static_cast<int>(gameManager->getDifficulty());
		auto bestTime = ScoreManager::getInstance()->getScore(difficulty);
		if (bestTime > 0) {
			auto bestTimeStringLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, WORLD_BEST);
			bestTimeStringLabel->setAlignment(alignment);
			bestTimeStringLabel->setColor(color);
			bestTimeStringLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 70);
			bestTimeStringLabel->setScale(0.6f * FONT_SCALE_FACTOR);
			addChild(bestTimeStringLabel, 1);

			auto bestTimeStr = Tools::getTimeString(bestTime);

			auto bestTimeLabel = cocos2d::Label::createWithBMFont(UI_FONT_FILE_PATH, bestTimeStr);
			bestTimeLabel->setAlignment(alignment);
			bestTimeLabel->setColor(color);
			bestTimeLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 190);
			bestTimeLabel->setScale(FONT_SCALE_FACTOR);
			addChild(bestTimeLabel, 1);
		}



		auto menuPos = cocos2d::Vec2(0, MENU_BUTTONS_Y);

		cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;
		auto pressedColor = ColorHSB(9, 25, 127);

		auto returnMenu = TakeTen::MenuButton::create(ICON_MENU_QUIT, pressedColor, 0, CC_CALLBACK_1(PauseScene::menuExit, this));
		_soundOnButton = TakeTen::MenuButton::create(ICON_MENU_SOUND_ON, pressedColor, ANIMATION_DELAY_TIME, CC_CALLBACK_1(PauseScene::menuSound, this));
		_soundOffButton = TakeTen::MenuButton::create(ICON_MENU_SOUND_OFF, pressedColor, ANIMATION_DELAY_TIME, CC_CALLBACK_1(PauseScene::menuSound, this));
		auto playMenu = TakeTen::MenuButton::create(ICON_MENU_PLAY, pressedColor, ANIMATION_DELAY_TIME * 2, CC_CALLBACK_1(PauseScene::menuPlay, this));

		returnMenu->setPosition(menuPos.x - 200, menuPos.y);
		_soundOnButton->setPosition(menuPos.x, menuPos.y);
		_soundOffButton->setPosition(menuPos.x, menuPos.y);
		playMenu->setPosition(menuPos.x + 200, menuPos.y);

		if (gameManager->isSound()) {
			_soundOffButton->setVisible(false);
		}
		else {
			_soundOnButton->setVisible(false);
		}

		pMenuItems.pushBack(returnMenu);
		pMenuItems.pushBack(playMenu);
		pMenuItems.pushBack(_soundOnButton);
		pMenuItems.pushBack(_soundOffButton);

		auto pMenu = cocos2d::Menu::createWithArray(pMenuItems);
		addChild(pMenu, 1);

		return true;
	}


	void PauseScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
			menuPlay(nullptr);
		}
	}


	void PauseScene::menuPlay(Ref* pSender) {
		GameManager::getInstance()->soundSelect();
		cocos2d::Director::getInstance()->popScene();
	}

	void PauseScene::menuExit(Ref* pSender) {
		GameManager::getInstance()->soundSelect();

		if (Tools::randBool()) {
			if (ScoreManager::getInstance()->showFullscreenAds()) {
				AdmobHelper::showFullscreenAd();
			}
		}

		auto mainMenu = MainMenuScene::createScene();
		cocos2d::Director::getInstance()->popScene();
		cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, mainMenu));
	}

	void PauseScene::menuSound(Ref* pSender) {
		GameManager::getInstance()->soundSelect(true);
		GameManager::getInstance()->switchSound();

		if (GameManager::getInstance()->isSound()) {
			_soundOffButton->setVisible(false);
			_soundOnButton->setVisible(true);
		}
		else {
			_soundOnButton->setVisible(false);
			_soundOffButton->setVisible(true);
		}

	}
}

