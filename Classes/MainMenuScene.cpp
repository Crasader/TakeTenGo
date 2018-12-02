#include <string.h>
#include "SimpleAudioEngine.h"
#include "HelpScene.h"
#include "LanguageManager.h"
#include "GameScene.h"
#include "ScoresScene.h"
#include "Definitions.h"
#include "MenuButton.h"
#include "Tools.h"
#include "GameManager.h"

#include "BackgroundNode.h"

#include "AdmobHelper.h"
#include "MessageNode.h"

#include "MainMenuScene.h"

namespace TakeTen {

	cocos2d::Scene* MainMenuScene::createScene() {
		auto scene = cocos2d::Scene::create();
		auto layer = MainMenuScene::create();
		scene->addChild(layer);
		return scene;
	}

	void MainMenuScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
			cocos2d::Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
		}
	}

	bool MainMenuScene::init() {
		if (!LayerColor::initWithColor(cocos2d::Color4B(51, 51, 51, 255))) {
			return false;
		}



		auto bg = BackgroundNode::create();
		this->addChild(bg);

		auto listener = cocos2d::EventListenerKeyboard::create();
		listener->onKeyReleased = CC_CALLBACK_2(MainMenuScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SPRITE_SHIT_PLIST);
		_spriteBatch = cocos2d::SpriteBatchNode::create(SPRITE_SHIT_PNG);
		this->addChild(_spriteBatch);

		_lastMenuItem = nullptr;

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

		cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;
		auto pressedColor = ColorHSB(9, 25, 127);
		auto delayTimer = 0.0f;

		auto centerPosition = cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
		auto menuPos = cocos2d::Vec2(0, 200);

		CCLOG("%s", FONT_FILE_PATH.c_str());

		auto selectDifficultyLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, SELECT_DIFFICULTY_STRING);
		selectDifficultyLabel->setScale(0.5f * FONT_SCALE_FACTOR);
		selectDifficultyLabel->setColor(cocos2d::Color3B(198, 163, 117));
		selectDifficultyLabel->setLineHeight(40);
		selectDifficultyLabel->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
		auto offset = cocos2d::Vec2(0, 560) ;
		selectDifficultyLabel->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2) + offset + menuPos);

		this->addChild(selectDifficultyLabel, 100);

		Tools::flipInItem(selectDifficultyLabel, ANIMATION_DELAY_TIME, 0.5f * FONT_SCALE_FACTOR);


		auto menuHeight = 1.25f * MENU_SIZE_HEIGHT * 3;

		ColorHSB colors[2] = { ColorHSB(5, 53, 57), ColorHSB(144, 22, 46) };

		auto index = 0;
		auto iconPosition = cocos2d::Vec2(MENU_SIZE_WIDTH / 2, MENU_SIZE_HEIGHT / 2);

		for (auto r = 0; r != 3; ++r) {
			for (auto c = 0; c != 2; ++c) {

				auto y = MENU_SIZE_HEIGHT * (r - 1);

				auto sprite = cocos2d::Sprite::createWithSpriteFrameName(MENU_CELL);
				auto spritePressed = cocos2d::Sprite::createWithSpriteFrameName(MENU_CELL);
				auto color = ColorHSB(114, 100, 74);
				auto pressedColor = ColorHSB(9, 25, 127);

				char buff[100];
				sprintf(buff, ICON_MENU_DIFFICULTY_LVL, (index + 1));
				std::string buffAsStdStr(buff);

				auto diffSprite = cocos2d::Sprite::createWithSpriteFrameName(buffAsStdStr);
				diffSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
				sprite->addChild(diffSprite, 10);

				auto name = index % 2 ? ICON_MENU_GOLD_RIGHT : ICON_MENU_GOLD_LEFT;
				auto progressSprite = cocos2d::Sprite::createWithSpriteFrameName(name);
				progressSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0));

				auto boardIndex = GameManager::getInstance()->getIndex(index);
				const size_t boards[6] = BOARDS_TO_GENERATE;
				auto amount = GameManager::getInstance()->getIndex(index) / static_cast<float>(boards[index]);

				cocos2d::Rect rect;
				Tools::cropSprite(progressSprite, amount, rect);

				diffSprite->setPosition(iconPosition - cocos2d::Vec2(0, rect.size.height / 2));
				progressSprite->setPosition(iconPosition - cocos2d::Vec2(0, rect.size.height / 2));

				sprite->addChild(progressSprite, 15);
				sprite->setColor(ColorHSB::lerp(colors[0], colors[1], amount).toRGB());
				spritePressed->setColor(pressedColor.toRGB());

				auto menuItem = cocos2d::MenuItemSprite::create(sprite, spritePressed, CC_CALLBACK_1(MainMenuScene::gotoGameScene, this));
				menuItem->setTag(index);
				menuItem->setPosition((static_cast<float>(c)-0.5f) * 400, menuPos.y - y);

				Tools::flipInItem(menuItem, delayTimer);
				pMenuItems.pushBack(menuItem);

				delayTimer += ANIMATION_DELAY_TIME;
				index++;
			}
		}

		delayTimer = ANIMATION_DELAY_TIME * 3;

#if SLIDEME
		auto scoreMenu = MenuButton::create(ICON_MENU_SCORE, pressedColor, delayTimer, CC_CALLBACK_1(MainMenuScene::gotoScoreScene, this));
		delayTimer += ANIMATION_DELAY_TIME;

		auto helpMenu = MenuButton::create(ICON_MENU_HELP, pressedColor, delayTimer, CC_CALLBACK_1(MainMenuScene::gotoHelpScene, this));
		delayTimer += ANIMATION_DELAY_TIME;

		scoreMenu->setPosition(-100, MENU_BUTTONS_Y);
		helpMenu->setPosition(100, MENU_BUTTONS_Y);

		pMenuItems.pushBack(scoreMenu);
		pMenuItems.pushBack(helpMenu);
#else
		if (GameManager::getInstance()->isFirstRun()) {

			auto scoreMenu = MenuButton::create(ICON_MENU_SCORE, pressedColor, delayTimer, CC_CALLBACK_1(MainMenuScene::gotoScoreScene, this));
			delayTimer += ANIMATION_DELAY_TIME;

			auto helpMenu = MenuButton::create(ICON_MENU_HELP, pressedColor, delayTimer, CC_CALLBACK_1(MainMenuScene::gotoHelpScene, this));
			delayTimer += ANIMATION_DELAY_TIME;

			scoreMenu->setPosition(-100, MENU_BUTTONS_Y);
			helpMenu->setPosition(100, MENU_BUTTONS_Y);

			pMenuItems.pushBack(scoreMenu);
			pMenuItems.pushBack(helpMenu);
		}
		else {
			auto rateMenu = MenuButton::create(ICON_MENU_RATE, pressedColor, delayTimer, CC_CALLBACK_1(MainMenuScene::gotoRate, this));
			delayTimer += ANIMATION_DELAY_TIME;

			auto scoreMenu = MenuButton::create(ICON_MENU_SCORE, pressedColor, delayTimer, CC_CALLBACK_1(MainMenuScene::gotoScoreScene, this));
			delayTimer += ANIMATION_DELAY_TIME;

			auto helpMenu = MenuButton::create(ICON_MENU_HELP, pressedColor, delayTimer, CC_CALLBACK_1(MainMenuScene::gotoHelpScene, this));
			delayTimer += ANIMATION_DELAY_TIME;

			rateMenu->setPosition(-200, MENU_BUTTONS_Y);
			scoreMenu->setPosition(0, MENU_BUTTONS_Y);
			helpMenu->setPosition(200, MENU_BUTTONS_Y);

			pMenuItems.pushBack(helpMenu);
			pMenuItems.pushBack(scoreMenu);
			pMenuItems.pushBack(rateMenu);
		}
#endif
		_mainMenu = cocos2d::Menu::createWithArray(pMenuItems);
		this->addChild(_mainMenu, 100);
		return true;
	}

	void MainMenuScene::gotoRate(Ref* pSender) {
		cocos2d::Application::getInstance()->openURL(RATE_LINK);
	}

	void MainMenuScene::gotoScoreScene(Ref* pSender) {
		GameManager::getInstance()->soundSelect();
		auto scene = ScoresScene::createScene();
		cocos2d::Director::getInstance()->pushScene(SCENE_TRANSITION(TRANSITION_TIME, scene));
	}

	void MainMenuScene::gotoGameScene(Ref* pSender) {

		auto gameManager = GameManager::getInstance();

		gameManager->soundSelect();

		if (_lastMenuItem) {
			_lastMenuItem->setVisible(true);
			Tools::flipInItem(_lastMenuItem, ANIMATION_DELAY_TIME);
			auto delayTime = 0.0f;
			for (auto n : _lastItems)  {
				auto fade = cocos2d::FadeOut::create(delayTime);
				auto hideFn = cocos2d::CallFunc::create(CC_CALLBACK_0(cocos2d::Node::removeFromParent, n));
				auto seq = cocos2d::Sequence::create(fade, hideFn, nullptr);
				n->runAction(seq);
				delayTime += ANIMATION_DELAY_TIME;
			}
			_lastItems.clear();
		}

		auto menuItem = static_cast<cocos2d::MenuItemSprite*>(pSender);
		auto tag = menuItem->getTag();

		auto timer = gameManager->getTime(tag);
		if (timer < 2) {
			gameManager->newGame(static_cast<GameDifficuty>(tag));
			return;
		}

		this->setTag(tag);

		_lastMenuItem = menuItem;

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto center = cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2);

		auto gameindex = static_cast<float>(gameManager->getIndex(tag));
		const size_t boards[6] = BOARDS_TO_GENERATE;
		auto totalBoards = static_cast<float>(boards[tag]);
		auto progress = static_cast<int>((gameindex / totalBoards) * 100.0f);
		auto progressString = " " + std::to_string(progress) + "%";
		auto intTimer = static_cast<int>(timer);
		auto timeString = Tools::getTimeString(intTimer) + progressString;

		menuItem->setVisible(false);

		auto pressedColor = ColorHSB(9, 25, 127);

		auto acceptFn = [](Ref* pSender) {
			auto sender = static_cast<cocos2d::Node*>(pSender);
			auto difficulty = static_cast<TakeTen::GameDifficuty>(sender->getTag());
			cocos2d::Director::getInstance()->popScene();
			GameManager::getInstance()->soundSelect();
			GameManager::getInstance()->resetGame(difficulty);
		};
		auto showMessageFn = [&](Ref* pSender) {
			auto sender = static_cast<cocos2d::Node*>(pSender);
			auto mn = MessageNode::createScene(acceptFn, sender->getTag());
			GameManager::getInstance()->soundSelect();
			cocos2d::Director::getInstance()->pushScene(SCENE_TRANSITION(TRANSITION_TIME, mn));
		};

		auto resetMenu = TakeTen::MenuButton::create(ICON_MENU_UNDO, pressedColor, ANIMATION_DELAY_TIME, showMessageFn);
		resetMenu->setTag(tag);
		
		const size_t boardsToGenerate[6] = BOARDS_TO_GENERATE;
		if (progress == boardsToGenerate[tag]) {
			resetMenu->setPosition(menuItem->getPosition() - cocos2d::Vec2(0, 50));
		}
		else {
			auto playMenu = TakeTen::MenuButton::create(ICON_MENU_PLAY, pressedColor, ANIMATION_DELAY_TIME * 2, [&](Ref* pSender) {
				auto node = static_cast<cocos2d::Node*>(pSender);
				GameManager::getInstance()->soundSelect();
				GameManager::getInstance()->newGame(static_cast<GameDifficuty>(node->getTag()));
			});
			playMenu->setTag(menuItem->getTag());
			playMenu->setPosition(menuItem->getPosition() - cocos2d::Vec2(-100, 50));
			resetMenu->setPosition(menuItem->getPosition() - cocos2d::Vec2(100, 50));
			_mainMenu->addChild(playMenu);
			_lastItems.push_back(playMenu);
		}

		_mainMenu->addChild(resetMenu);
		_lastItems.push_back(resetMenu);

		auto timeLabel = cocos2d::Label::createWithBMFont(UI_FONT_FILE_PATH, timeString);
		timeLabel->setScale(0.5f * FONT_SCALE_FACTOR);
		timeLabel->setColor(cocos2d::Color3B(198, 163, 117));
		timeLabel->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
		timeLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		timeLabel->setPosition(menuItem->getPosition() + center + cocos2d::Vec2(0, 100));

		_lastItems.push_back(timeLabel);

		this->addChild(timeLabel, 0);
		Tools::flipInItem(timeLabel, CARD_FLIP_TIME, 0.5f * FONT_SCALE_FACTOR);
	}

	void MainMenuScene::gotoHelpScene(Ref* pSender) {
		GameManager::getInstance()->soundSelect();
		auto helpScene = HelpScene::createScene();
		cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, helpScene));
	}
}


