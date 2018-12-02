#include "Definitions.h"
#include "ScoreManager.h"
#include "GameManager.h"
#include "ColorHSB.h"
#include "MenuButton.h"
#include "LanguageManager.h"

#include "BackgroundNode.h"

#include "Tools.h"

#include "ScoresScene.h"


namespace TakeTen {
	cocos2d::Scene* ScoresScene::createScene() {
		auto scene = cocos2d::Scene::create();
		auto layer = ScoresScene::create();
		scene->addChild(layer);
		return scene;
	}

	bool ScoresScene::init() {
		if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(51, 51, 51, 255))) {
			return false;
		}

		auto bg = BackgroundNode::create();
		this->addChild(bg);

		_delayTimer = 0.0f;

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		
		auto alignment = cocos2d::TextHAlignment::CENTER;
		auto color = cocos2d::Color3B(198, 163, 117);

		auto listener = cocos2d::EventListenerKeyboard::create();
		listener->onKeyReleased = CC_CALLBACK_2(ScoresScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		auto scoreLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, SCORES_STRING);
		scoreLabel->setAlignment(alignment);
		scoreLabel->setColor(color);
		scoreLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 850);
		scoreLabel->setScale(FONT_SCALE_FACTOR);
		addChild(scoreLabel, 1);
		
		Tools::flipInItem(scoreLabel, _delayTimer, FONT_SCALE_FACTOR);
		_delayTimer += ANIMATION_DELAY_TIME;

		auto menuPos = cocos2d::Vec2(0, MENU_BUTTONS_Y);

		cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;
		auto pressedColor = ColorHSB(9, 25, 127);

		auto returnMenu = MenuButton::create(ICON_MENU_QUIT, pressedColor, 0, CC_CALLBACK_1(ScoresScene::menuBack, this));
		returnMenu->setPosition(menuPos.x, menuPos.y);
		pMenuItems.pushBack(returnMenu);
		auto pMenu = cocos2d::Menu::createWithArray(pMenuItems);
		this->addChild(pMenu, 1);

		auto playerPos = cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2 + 710);
		auto playerLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, SCORES_PLAYER);
		playerLabel->setAlignment(alignment);
		playerLabel->setColor(cocos2d::Color3B(112, 135, 147));
		playerLabel->setPosition(playerPos);
		playerLabel->setScale(0.35f * FONT_SCALE_FACTOR);
		addChild(playerLabel, 1);

		Tools::flipInItem(playerLabel, _delayTimer, 0.35f * FONT_SCALE_FACTOR);
		_delayTimer += ANIMATION_DELAY_TIME;

		auto worldPos = cocos2d::Vec2(visibleSize.width / 2 + 320, visibleSize.height / 2 + 710);
		auto worldLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, SCORES_WORLD);
		worldLabel->setAlignment(alignment);
		worldLabel->setColor(cocos2d::Color3B(112, 135, 147));
		worldLabel->setPosition(worldPos);
		worldLabel->setScale(0.35f * FONT_SCALE_FACTOR);
		addChild(worldLabel, 1);

		Tools::flipInItem(worldLabel, _delayTimer, 0.35f * FONT_SCALE_FACTOR);
		_delayTimer += ANIMATION_DELAY_TIME;

		ColorHSB colors[2] = { ColorHSB(144, 22, 46), ColorHSB(5, 53, 57) };

		_delayTimer = 0.0f;

		auto labelPos = cocos2d::Vec2(visibleSize.width / 2 - 320, visibleSize.height / 2 + 580);
		for (auto i = 0; i != 6; ++i) {
			auto diffLabel = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, DIFFICULTY(i));
			auto offset = cocos2d::Vec2(0, i * 180.0f);
			diffLabel->setAlignment(alignment);
			diffLabel->setColor(ColorHSB::lerp(colors[0], colors[1], i / 5.0f).toRGB());
			diffLabel->setPosition(labelPos - offset);
			diffLabel->setScale(0.35f * FONT_SCALE_FACTOR);
			addChild(diffLabel, 1);

			Tools::flipInItem(diffLabel, _delayTimer, 0.35f * FONT_SCALE_FACTOR);
			_delayTimer += ANIMATION_DELAY_TIME;
		}

		createPlayerLabels();
		createWorldLabels();

		return true;
	}

	void ScoresScene::createPlayerLabels() {

		_delayTimer = ANIMATION_DELAY_TIME * 2;

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		auto alignment = cocos2d::TextHAlignment::CENTER;
		auto color = cocos2d::Color3B(198, 163, 117);

		auto labelPos = cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2 + 580);

		std::string timeStr;

		

		for (auto i = 0; i != 6; ++i) {

			auto index = GameManager::getInstance()->getIndex(i);
			const size_t boards[6] = BOARDS_TO_GENERATE;
			if (index != boards[i]) {
				timeStr = "-";
			}
			else {
				auto time = static_cast<int>(GameManager::getInstance()->getTime(i));
				timeStr = Tools::getTimeString(time);
			}

			

			auto label = cocos2d::Label::createWithBMFont(UI_FONT_FILE_PATH, timeStr);
			auto offset = cocos2d::Vec2(0, i * 180.0f);
			label->setAlignment(alignment);
			label->setColor(color);
			label->setPosition(labelPos - offset);
			label->setScale(0.5f * FONT_SCALE_FACTOR);
			addChild(label, 1);

			Tools::flipInItem(label, _delayTimer, 0.5f * FONT_SCALE_FACTOR);
			_delayTimer += ANIMATION_DELAY_TIME;

		}
	}

	void ScoresScene::createWorldLabels() {

		_delayTimer = ANIMATION_DELAY_TIME * 4;

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		auto alignment = cocos2d::TextHAlignment::CENTER;
		auto color = cocos2d::Color3B(198, 163, 117);

		std::string timeStr;
		auto labelPos = cocos2d::Vec2(visibleSize.width / 2 + 320, visibleSize.height / 2 + 580);
		for (auto i = 0; i != 6; ++i) {

			auto time = ScoreManager::getInstance()->getScore(i);
			if (time < 10) {
				timeStr = "-";
			}
			else {
				auto index = GameManager::getInstance()->getIndex(i);
				const size_t boards[6] = BOARDS_TO_GENERATE;
				if (index == boards[i]) {
					auto timePlayer = static_cast<int>(GameManager::getInstance()->getTime(i));
					if (time > timePlayer) {
						ScoreManager::getInstance()->postScore(i, timePlayer);
						time = timePlayer;
					}
				}


				timeStr = Tools::getTimeString(time);
			}
			
			auto label = cocos2d::Label::createWithBMFont(UI_FONT_FILE_PATH, timeStr);
			auto offset = cocos2d::Vec2(0, i * 180.0f);
			label->setAlignment(alignment);
			label->setColor(color);
			label->setPosition(labelPos - offset);
			label->setScale(0.5f * FONT_SCALE_FACTOR);
			addChild(label, 1);

			Tools::flipInItem(label, _delayTimer, 0.5f * FONT_SCALE_FACTOR);
			_delayTimer += ANIMATION_DELAY_TIME;
		}
	}

	void ScoresScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
			menuBack(nullptr);
		}
	}

	void ScoresScene::menuBack(Ref* pSender) {
		GameManager::getInstance()->soundSelect();
		cocos2d::Director::getInstance()->popScene();
	}
}