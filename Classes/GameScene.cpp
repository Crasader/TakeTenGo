#include "GameScene.h"
#include "MainMenuScene.h"
#include "DataHandler.h"
#include "GameManager.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

GameScene::~GameScene() {
	for (auto& s : _cellButtons) {
		CC_SAFE_RELEASE(s);
	}
		
	this->removeChild(_spriteBatch);
	this->removeChild(_boardNode);
	this->removeChild(_selectionSprite);

	_colors.clear();
	_cellButtons.clear();

	log("GAME SCENE DESTRUCTOR");
}

Scene* GameScene::createScene(const TakeTen::Size& size) {
    auto scene = Scene::create();
	auto layer = GameScene::create(size);
    scene->addChild(layer);
    return scene;
}


void GameScene::onExit() {
	Node::onExit();
	TakeTen::GameManager::getInstance()->save();
}

// on "init" you need to initialize your instance
bool GameScene::init(const TakeTen::Size& size) {
	if (!LayerColor::initWithColor(Color4B(51, 51, 51, 255)))
        return false;
    
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("spriteSheet.plist");
	_spriteBatch = SpriteBatchNode::create("spriteSheet.png");
	
	this->addChild(_spriteBatch);

	_selectionSprite = Sprite::createWithSpriteFrameName("selection.png");
	this->addChild(_selectionSprite);
	_selectionSprite->setVisible(false);
	_selectionSprite->setZOrder(100);

	_boardNode = Node::create();
	this->addChild(_boardNode, 10);

	_dotsNode = Node::create();
	this->addChild(_dotsNode, 1);

	_gameBoard = nullptr;
	_gameTimer = 0.0f;
	_animationDelayTimer = 0.0f;

	_cellsRemoved = false;

	
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(path);

	createLabels();
	createMenuButtons();
	createBoard();
	
	this->scheduleUpdate();
	return true;
}

void GameScene::createLabels() {

	auto labelString = std::to_string(1);

	_countLabel = Label::createWithBMFont("fonts/bitmapFontNumeric.fnt", labelString);
	_countLabel->setAlignment(TextHAlignment::RIGHT);
	_countLabel->setColor(Color3B(53, 65, 71));
	this->addChild(_countLabel, 1);

	_timeLabel = Label::createWithBMFont("fonts/bitmapFontNumeric.fnt", "");
	_timeLabel->setScale(0.75f);
	_timeLabel->setAlignment(TextHAlignment::LEFT);
	_timeLabel->setColor(Color3B(198, 163, 117));
	this->addChild(_timeLabel, 1);
}

void GameScene::createMenuButtons() {
	auto menuPos = Vec2(0, -500);

	cocos2d::Vector<cocos2d::MenuItem*> pMenuItems;

	auto undoIcon = Sprite::createWithSpriteFrameName("gamemenu_Undo.png");
	auto undoMainColor = ColorHSB(199, 25, 27);
	auto undoInverted = Sprite::createWithSpriteFrameName("cell.png");
	auto undoInvertedIcon = Sprite::createWithSpriteFrameName("gamemenu_Undo.png");
	undoInvertedIcon->setPosition(undoInverted->getContentSize().width / 2, undoInverted->getContentSize().height / 2);
	undoInverted->addChild(undoInvertedIcon);
	undoInverted->setColor(undoMainColor.toRGB());

	auto pauseMainColor = ColorHSB(144, 22, 46);
	auto pauseIcon = Sprite::createWithSpriteFrameName("gamemenu_Pause.png");
	auto pauseInverted = Sprite::createWithSpriteFrameName("cell.png");
	auto pauseInvertedIcon = Sprite::createWithSpriteFrameName("gamemenu_Pause.png");
	pauseInvertedIcon->setPosition(pauseInverted->getContentSize().width / 2, pauseInverted->getContentSize().height / 2);
	pauseInverted->addChild(pauseInvertedIcon);
	pauseInverted->setColor(pauseMainColor.toRGB());

	auto quitIcon = Sprite::createWithSpriteFrameName("gamemenu_Quit.png");
	auto quitMainColor = ColorHSB(5, 53, 57);
	auto quitInverted = Sprite::createWithSpriteFrameName("cell.png");
	auto quitInvertedIcon = Sprite::createWithSpriteFrameName("gamemenu_Quit.png");
	quitInvertedIcon->setPosition(quitInverted->getContentSize().width / 2, quitInverted->getContentSize().height / 2);
	quitInverted->addChild(quitInvertedIcon);
	quitInverted->setColor(quitMainColor.toRGB());

	_undoMenu = cocos2d::MenuItemSprite::create(undoIcon, undoInverted, CC_CALLBACK_1(GameScene::menuUndo, this));
	_pauseMenu = cocos2d::MenuItemSprite::create(pauseIcon, pauseInverted, CC_CALLBACK_1(GameScene::menuSettings, this));
	auto quitItem = cocos2d::MenuItemSprite::create(quitIcon, quitInverted, CC_CALLBACK_1(GameScene::menuQuit, this));

	_undoMenu->setPosition(menuPos.x - 200, menuPos.y);
	_pauseMenu->setPosition(menuPos.x, menuPos.y);
	quitItem->setPosition(menuPos.x + 200, menuPos.y);

	pMenuItems.pushBack(_undoMenu);
	pMenuItems.pushBack(_pauseMenu);
	pMenuItems.pushBack(quitItem);

	int index = 0;

	for (auto& s : pMenuItems) {
		s->setScaleX(0);
		s->setSkewY(-SKEW_AMOUNT);

		auto delay = DelayTime::create(_animationDelayTimer);
		auto actionScale = ScaleTo::create(CARD_FLIP_TIME, 1.0f, 1.0f);
		auto actionSkew = SkewTo::create(CARD_FLIP_TIME, 0, 0);

		auto easyScale = EaseBackOut::create(actionScale);
		auto easySkew = EaseBackOut::create(actionSkew);

		s->runAction(Sequence::create(delay, easyScale, NULL));
		s->runAction(Sequence::create(delay, easySkew, NULL));

		_animationDelayTimer += ANIMATION_DELAY_TIME;
	}
	cocos2d::Menu* pMenu = cocos2d::Menu::createWithArray(pMenuItems);
	this->addChild(pMenu, 1);
}

void GameScene::createDot(const cocos2d::Vec2& position, const cocos2d::Color3B& color) {

	auto actionDelay = cocos2d::DelayTime::create(CARD_FLIP_TIME / 2);
	auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME / 2, 1.0f, 1.0f);
	auto easyScale = cocos2d::EaseBackOut::create(actionScale);

	auto actionRotate = cocos2d::RotateBy::create(CARD_FLIP_TIME, 90);
	auto easyRotate = cocos2d::EaseBackOut::create(actionRotate);

	auto sequenceScale = cocos2d::Sequence::create(actionDelay->clone(), easyScale, NULL);
	auto sequenceRotate = cocos2d::Sequence::create(actionDelay->clone(), easyRotate, NULL);

	auto empty = Sprite::createWithSpriteFrameName("empty.png");
	empty->setPosition(position);
	empty->setScale(0.0f);
	empty->setRotation(-90.0f);
	empty->setColor(color);
	empty->runAction(sequenceScale);
	empty->runAction(sequenceRotate);
	_dotsNode->addChild(empty, 0);
}

void GameScene::createBoard() {
	
	updateGameCountLabel();
	resetTimers();

	for (auto &c : _boardNode->getChildren()) {
		_eventDispatcher->removeEventListenersForTarget(c, true);
	}

	_boardNode->removeAllChildrenWithCleanup(true);
	_cellButtons.clear();

	TakeTen::Size oldBoardSize;

	if (_gameBoard) {
		oldBoardSize = _gameBoard->getSize();
	}

	auto gameManager = TakeTen::GameManager::getInstance();
	_gameBoard = gameManager->getNextBoard();
	_gameTimer = gameManager->getTime();

	auto newBoardSize = _gameBoard->getSize();

	if (oldBoardSize != newBoardSize) {
		_colors = ColorHSB::calculateColors(newBoardSize);
		_dotsNode->removeAllChildrenWithCleanup(true);
		_rebuildDots = true;
	}
	else {
		_rebuildDots = false;
	}

	CCLOG("CREATE BOARD INDEX == %d", gameManager->getIndex());

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto count = newBoardSize.getCount();
	auto boardContentSize = Size(newBoardSize.getWidht() * SPRITE_SIZE, newBoardSize.getHeight() * SPRITE_SIZE);
	auto boardPosition = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + SPRITE_SIZE);

	_boardNode->setContentSize(boardContentSize);
	_boardNode->setPosition(boardPosition);

	if (_rebuildDots) {
		_dotsNode->setPosition(_boardNode->getPosition());
	}

	auto countLabelPos = _boardNode->getPosition() + origin + Vec2(_boardNode->getContentSize().width / 2, _boardNode->getContentSize().height / 2);
	auto timeLabelPos = _boardNode->getPosition() + origin + Vec2(-_boardNode->getContentSize().width / 2, _boardNode->getContentSize().height / 2);

	_countLabel->setAnchorPoint(Vec2(1, 0.21f));
	_countLabel->setPosition(countLabelPos);

	_timeLabel->setAnchorPoint(Vec2(0, 0.19f));
	_timeLabel->setPosition(timeLabelPos);

	for (auto i = 0; i < count; i++) {
		_cellButtons.push_back(nullptr);
	}

	rebuildBoard();
	
	
}

void GameScene::rebuildBoard() {

	_selectionSprite->setVisible(false);
	_lastSelectedCell = nullptr;

	auto size = _gameBoard->getSize();
	auto count = size.getCount();

	TakeTen::Position cellPosition;
	TakeTen::Cell cell;
	char cellValue;
	int index = 0;

	for (auto i = 0; i < count; i++) {

		if (_cellButtons[i])
			continue;

		cell = _gameBoard->getCellByIndex(i);
		cellValue = cell.getValue();

		if (cellValue == -1) {
			continue;
		}

		cellPosition = cell.getPosition();

		auto cellSprite = CellSprite::create(cellValue, cellPosition, size, _colors[i]);
		_boardNode->addChild(cellSprite, 0);

		if (_rebuildDots) {
			createDot(cellSprite->getPosition(), cellSprite->getColor());
		}

		auto listener1 = EventListenerTouchOneByOne::create();
		listener1->setSwallowTouches(true);

		// Example of using a lambda expression to implement onTouchBegan event callback function
		listener1->onTouchBegan = CC_CALLBACK_2(GameScene::touchesBegan, this);
		listener1->onTouchMoved = CC_CALLBACK_2(GameScene::touchesMoved, this);
		listener1->onTouchEnded = CC_CALLBACK_2(GameScene::touchesEnd, this);
		listener1->onTouchCancelled = CC_CALLBACK_2(GameScene::touchesEnd, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, cellSprite);

		_animationDelayTimer = (MIN(index, MAX(cellPosition.getColumn(), cellPosition.getRow())) * ANIMATION_DELAY_TIME);

		//+ _rebuildDots ? CARD_FLIP_TIME : 0.0f
		cellSprite->show(_animationDelayTimer);
		_cellButtons[i] = cellSprite;
		index++;
	}

	updateMenuButtons();

	_won = false;
}

void GameScene::newGame() {
	_selectionSprite->setVisible(false);
	createBoard();
}

void GameScene::congratulations() {

	auto actionDelay = cocos2d::DelayTime::create(CONGRATULATION_TIME);
	auto newGameAction = cocos2d::CallFunc::create(this, callfunc_selector(GameScene::newGame));

	auto sequence = cocos2d::Sequence::create(actionDelay, newGameAction, NULL);

	auto gameManager = TakeTen::GameManager::getInstance();
	gameManager->won();

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("congratulations.wav"); 

	runAction(sequence);
	
	_won = true;
}

void GameScene::selectCell(CellSprite* cell) {
	TakeTen::Pair pair;

	if (_gameBoard->selectCell(cell->getCellPosition(), pair)) {
		auto index1 = _gameBoard->getIndex(pair.getCell1());
		auto index2 = _gameBoard->getIndex(pair.getCell2());

		auto cell1 = _cellButtons[index1];
		auto cell2 = _cellButtons[index2];
		
		if (cell1) { 
			_eventDispatcher->removeEventListenersForTarget(cell1, true);
			cell1->hide();
		}
		if (cell2) {
			_eventDispatcher->removeEventListenersForTarget(cell2, true);
			cell2->hide();
		}

		_cellButtons[index1] = nullptr;
		_cellButtons[index2] = nullptr;

		_selectionSprite->setVisible(false);
		_lastSelectedCell = nullptr;

		_cellsRemoved = true;

		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("remove.wav");

		unselectAll();

		if (_gameBoard->isEmpty()) {
			congratulations();
		}
	}
	else {
		_lastSelectedCell = cell;
		cell->select();
		_selectionSprite->setVisible(true);
		_selectionSprite->setPosition(cell->getPosition() + _boardNode->getPosition());
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tick.wav");
	}

	updateMenuButtons();
}


void GameScene::unselectAll() {
	for (auto &cell : _cellButtons) {
		if (cell) {
			if (cell->isSelected()) {
				cell->unselect();
			}
		}
	}
}

void GameScene::resetTimers() {
	_animationDelayTimer = 0;
}

void GameScene::update(float dt) {

	static float secondsTimer = 1.0f;

	if (_won) {
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
	_undoMenu->setOpacity(_gameBoard->canUndo() ? 255 : 30);
	_undoMenu->setEnabled(_gameBoard->canUndo());
}

void GameScene::updateTimeLabel() {
	auto timeString = TakeTen::Tools::getTimeString(_gameTimer);
	TakeTen::GameManager::getInstance()->setTime(_gameTimer);
	_timeLabel->setString(timeString);
}

void GameScene::updateGameCountLabel() {
	auto index = TakeTen::GameManager::getInstance()->getIndex();
	auto labelString = std::to_string(index);
	_countLabel->setString(labelString);
}

bool GameScene::touchesBegan(cocos2d::Touch* touch, cocos2d::Event* cEvent) {

	_cellsRemoved = false;
	auto target = static_cast<CellSprite*>(cEvent->getCurrentTarget());

	if (target) {
		auto locationInNode = target->convertToNodeSpace(touch->getLocation());
		auto size = target->getContentSize();
		for (auto &cell : _cellButtons) {
			if (cell == target) {
				auto rect = Rect(0, 0, size.width, size.height);
				if (rect.containsPoint(locationInNode)) {
					_selectionSprite->setPosition(cell->getPosition() + _boardNode->getPosition());
					selectCell(cell);
					return true;
				}
				else {
					target->setZOrder(1);
				}
			}
		}
	}
	return false;
}

void GameScene::touchesMoved(cocos2d::Touch* touch, cocos2d::Event* cEvent) {

	if (_cellsRemoved) {
		_selectionSprite->setVisible(false);
		return;
	}

	auto found = false;
	for (auto &cell : _cellButtons) {
		if (cell) {
			auto locationInNode = cell->convertToNodeSpace(touch->getLocation());
			auto size = cell->getContentSize();
			auto rect = Rect(0, 0, size.width, size.height);
			if (rect.containsPoint(locationInNode)) {
				found = true;
				if (!cell->isSelected()) {
					selectCell(cell);
				}
			}
			else {
				cell->unselect();
			}
		}
	}

	if (_lastSelectedCell) {
		_selectionSprite->setVisible(true);
	}
	else {
		_selectionSprite->setVisible(false);
	}
}

void GameScene::touchesEnd(cocos2d::Touch* touch, cocos2d::Event* cEvent) {

	unselectAll();

	if (_lastSelectedCell) {
		_selectionSprite->setVisible(true);
		_selectionSprite->setPosition(_lastSelectedCell->getPosition() + _boardNode->getPosition());
	}
	else {
		_selectionSprite->setVisible(false);
	}
}

void GameScene::menuSettings(Ref* pSender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tick.wav");
	congratulations();
}

void GameScene::menuUndo(Ref* pSender) {
	if (_gameBoard->canUndo()) {
		CCLOG("CAN UNDO");
		_gameBoard->undo();
		rebuildBoard();
		_gameTimer += UNDO_COST;
		updateTimeLabel();
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("undo.wav");
	}
}

void GameScene::menuQuit(Ref* pSender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tick.wav");
	auto gameScene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(TRANSITION_TIME, gameScene));
}

void GameScene::menuHint(Ref* pSender) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tick.wav");
	if (_gameBoard->isEmpty()) return;

	auto pair = TakeTen::Pair();
	bool found = _gameBoard->solve(pair);
	
	if (found) {
		_gameTimer += HINT_COST;
		updateTimeLabel();

		auto index1 = _gameBoard->getIndex(pair.getCell1());
		auto index2 = _gameBoard->getIndex(pair.getCell2());
		
		_cellButtons[index1]->pop();
		_cellButtons[index2]->pop();
	}
	else {
		CCLOG("BOARD %d CANNOT BE SOLVED", _gameBoard->getHash());
		//menuQuit(pSender);
	}
}

void GameScene::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
