#include "SimpleAudioEngine.h"

#include "Definitions.h"

#include "ColorHSB.h"
#include "GameManager.h"
#include "MainMenuScene.h"
#include "LanguageManager.h"
#include "MenuButton.h"

#include "BackgroundNode.h"
#include "Tools.h"

#include "HelpScene.h"

namespace TakeTen {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	static std::vector<HelpScene::BoardHolder> _tutorialBoards;
#else
	static const std::vector<HelpScene::BoardHolder> _tutorialBoards {
			{ Size(1, 1), { 1 } },
			{ Size(2, 1), { 2, 2 } },
			{ Size(1, 2), { 3, 7 } },
			{ Size(3, 2), { 4, 6, 5, -1, -1, 5 } },
			{ Size(3, 3), { -1, -1, 8, 7, -1, 3, -1, -1, 2 } },
			{ Size(3, 3), { 1, -1, 3, 1, 2, 8, -1, -1, 3 } },
};
#endif
	
	HelpScene::~HelpScene() {
		removeAllChildrenWithCleanup(true);
	}

	cocos2d::Scene* HelpScene::createScene() {
		auto scene = cocos2d::Scene::create();
		auto layer = HelpScene::create();
		scene->addChild(layer);
		return scene;
	}

	bool HelpScene::init() {

		CCLOG("HELP INIT");

		if (!LayerColor::initWithColor(cocos2d::Color4B(51, 51, 51, 255))) {
			return false;
		}

		

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		_tutorialBoards.clear();

		char a0[1] = { 1 } ;
		char a1[2] = { 2, 2 };
		char a2[2] = { 3, 7 };
		char a3[6] = { 4, 6, 5, -1, -1, 5 };
		char a4[9] = { -1, -1, 8, 7, -1, 3, -1, -1, 2 };
		char a5[9] = { 1, -1, 3, 1, 2, 8, -1, -1, 3 };

		std::vector<char> v0(1);
		std::vector<char> v1(2);
		std::vector<char> v2(2);
		std::vector<char> v3(6);
		std::vector<char> v4(9);
		std::vector<char> v5(9);

		for (auto i = 0; i != 1; ++i) {
			v0[i] = a0[i];
		}

		for (auto i = 0; i != 2; ++i) {
			v1[i] = a1[i];
			v2[i] = a2[i];
		}

		for (auto i = 0; i != 6; ++i) {
			v3[i] = a3[i];
		}

		for (auto i = 0; i != 9; ++i) {
			v4[i] = a4[i];
			v5[i] = a5[i];
		}

		HelpScene::BoardHolder bh0;
		bh0.size = Size(1, 1);
		bh0.data = v0;

		HelpScene::BoardHolder bh1;
		bh1.size = Size(2, 1);
		bh1.data = v1;

		HelpScene::BoardHolder bh2;
		bh2.size = Size(1, 2);
		bh2.data = v2;

		HelpScene::BoardHolder bh3;
		bh3.size = Size(3, 2);
		bh3.data = v3;

		HelpScene::BoardHolder bh4;
		bh4.size = Size(3, 3);
		bh4.data = v4;

		HelpScene::BoardHolder bh5;
		bh5.size = Size(3, 3);
		bh5.data = v5;

		_tutorialBoards.push_back(bh0);
		_tutorialBoards.push_back(bh1);
		_tutorialBoards.push_back(bh2);
		_tutorialBoards.push_back(bh3);
		_tutorialBoards.push_back(bh4);
		_tutorialBoards.push_back(bh5);
#endif

		auto bg = BackgroundNode::create();
		this->addChild(bg);

		auto listener = cocos2d::EventListenerKeyboard::create();
		listener->onKeyReleased = CC_CALLBACK_2(HelpScene::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		_currentHelpSet = 0;

		auto spriteBatch = cocos2d::SpriteBatchNode::create(SPRITE_SHIT_PNG);
		auto cache = cocos2d::SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(SPRITE_SHIT_PLIST);
		this->addChild(spriteBatch);

		_boardNode = BoardNode::create(CC_CALLBACK_0(HelpScene::updateBoard, this));
		this->addChild(_boardNode);

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		auto cellPosition = cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2 + SPRITE_SIZE);

		_selectionSprite = cocos2d::Sprite::createWithSpriteFrameName(CELL_SELECTION);
		_selectionSprite->setVisible(false);
		_selectionSprite->setPosition(cellPosition);

		_firstCell = CellSprite::create(1, Position(1, 1), Size(3, 3), ColorHSB(200, 23, 57));
		_firstCell->setPosition(cellPosition);
		_firstCell->setBasePosition(cellPosition);

		Tools::flipInItem(_firstCell, ANIMATION_DELAY_TIME);

		_boardNode->addChild(_selectionSprite, 100);
		_boardNode->addChild(_firstCell, 50);

		auto listener1 = cocos2d::EventListenerTouchOneByOne::create();
		listener1->setSwallowTouches(true);

		listener1->onTouchBegan = CC_CALLBACK_2(HelpScene::touchesBegan, this);
		listener1->onTouchMoved = CC_CALLBACK_2(HelpScene::touchesMoved, this);
		listener1->onTouchEnded = CC_CALLBACK_2(HelpScene::touchesEnd, this);
		listener1->onTouchCancelled = CC_CALLBACK_2(HelpScene::touchesEnd, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, _firstCell);

		createMenuButtons();
		createLabels();
		updateHelpText();

		return true;
	}

	void HelpScene::createLabels() {
		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

		_helpText = cocos2d::Label::createWithBMFont(FONT_FILE_PATH, "Help");
		_helpText->setColor(cocos2d::Color3B(198, 163, 117));
		_helpText->setScale(0.45f * FONT_SCALE_FACTOR);
		_helpText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
		_helpText->setMaxLineWidth(900);
		auto offset = cocos2d::Vec2(0, 700);
		auto helpPosition = cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2) + offset;
		_helpText->setPosition(helpPosition);

		addChild(_helpText, 100);
	}

	void HelpScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
			mainMenuLoader();
		}
	}

	void HelpScene::createMenuButtons() {
		auto menuPos = cocos2d::Vec2(0, MENU_BUTTONS_Y);

		cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;
		auto pressedColor = ColorHSB(9, 25, 127);

		auto backMenu = MenuButton::create(ICON_MENU_QUIT, pressedColor, 0, CC_CALLBACK_1(HelpScene::menuBack, this));
		backMenu->setPosition(menuPos.x, menuPos.y);
		pMenuItems.pushBack(backMenu);
		auto pMenu = cocos2d::Menu::createWithArray(pMenuItems);
		addChild(pMenu);
	}

	void HelpScene::updateBoard() {
		if (_currentHelpSet >= TOTAL_HELP_PAGES) {

			auto delay = cocos2d::DelayTime::create(CONGRATULATION_TIME);
			auto nextBoardFn = cocos2d::CallFunc::create([&]() {
				this->mainMenuLoader();
			});
			auto seq = cocos2d::Sequence::create(delay, nextBoardFn, nullptr);
			this->runAction(seq);

			return;
		}

		auto delay = cocos2d::DelayTime::create(CONGRATULATION_TIME);
		auto nextBoardFn = cocos2d::CallFunc::create([&]() {
			auto size = _tutorialBoards[_currentHelpSet].size;
			auto data = _tutorialBoards[_currentHelpSet].data;
			std::shared_ptr<Board> board(new Board(size, data));
			this->_boardNode->createBoard(board);
			this->updateHelpText();
		});

		auto seq = cocos2d::Sequence::create(delay, nextBoardFn, nullptr);
		this->runAction(seq);

	}

	void HelpScene::updateHelpText() {

		char buff[100];
		sprintf(buff, "HelpText%d", _currentHelpSet);
		std::string buffAsStdStr(buff);

		_helpText->setString(GET_STRING(buffAsStdStr));
		_currentHelpSet++;
	}

	void HelpScene::mainMenuLoader(float dt) {
		auto mainMenuScene = MainMenuScene::createScene();
		cocos2d::Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, mainMenuScene));
	}

	void HelpScene::menuNext(Ref* pSender) {
		GameManager::getInstance()->soundSelect();
		updateBoard();
	}

	void HelpScene::menuBack(Ref* pSender) {
		GameManager::getInstance()->soundSelect();
		mainMenuLoader();
	}

	bool HelpScene::touchesBegan(cocos2d::Touch* touch, cocos2d::Event* cEvent) {
		auto target = static_cast<CellSprite*>(cEvent->getCurrentTarget());
		if (target) {
			auto locationInNode = _firstCell->convertToNodeSpace(touch->getLocation());
			auto size = target->getContentSize();
			auto rect = cocos2d::Rect(0, 0, size.width, size.height);
			if (rect.containsPoint(locationInNode)) {
				if (!_firstCell->isSelected()) {
					GameManager::getInstance()->soundSelect();
					_firstCell->select();
					_selectionSprite->setVisible(true);
				}
				return true;
			}
		}
		return false;
	}

	void HelpScene::touchesMoved(cocos2d::Touch* touch, cocos2d::Event* cEvent) {
		auto target = static_cast<CellSprite*>(cEvent->getCurrentTarget());
		if (target) {
			auto locationInNode = _firstCell->convertToNodeSpace(touch->getLocation());
			auto size = target->getContentSize();
			auto rect = cocos2d::Rect(0, 0, size.width, size.height);
			if (rect.containsPoint(locationInNode)) {
				if (!_firstCell->isSelected()) {
					GameManager::getInstance()->soundSelect();
					_firstCell->select();
					_selectionSprite->setVisible(true);
				}
			}
			else
			{
				_selectionSprite->setVisible(false);
				_firstCell->unselect();
			}
		}
	}

	void HelpScene::touchesEnd(cocos2d::Touch* touch, cocos2d::Event* cEvent) {
		auto target = static_cast<CellSprite*>(cEvent->getCurrentTarget());
		if (target) {
			auto locationInNode = _firstCell->convertToNodeSpace(touch->getLocation());
			auto size = target->getContentSize();
			auto rect = cocos2d::Rect(0, 0, size.width, size.height);
			if (rect.containsPoint(locationInNode)) {
				_eventDispatcher->removeEventListenersForTarget(_firstCell, true);
				_firstCell->hide();
				_selectionSprite->setVisible(false);
				_selectionSprite->removeFromParentAndCleanup(true);
				updateBoard();
				return;
			}
		}
		_selectionSprite->setVisible(false);
		_firstCell->unselect();
	}
}

