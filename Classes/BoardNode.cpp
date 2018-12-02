#include <math.h>
#include "Definitions.h"
#include "Size.h"
#include "BoardNode.h"
#include "ColorHSB.h"
#include "GameManager.h"

namespace TakeTen {

	BoardNode::BoardNode()
		: _board(nullptr), _cellsNode(nullptr) { }

	BoardNode::BoardNode(boardPtr board)
		: _board(board), _cellsNode(nullptr) { }

	BoardNode::~BoardNode() {
		CCLOG("BOARD NODE DESTRUCTOR");

		removeAllChildrenWithCleanup(true);
		removeFromParentAndCleanup(true);

		_colors.clear();
		_cellButtons.clear();
	}

	BoardNode* BoardNode::create(const std::function<void()>& solvedCallBack) {
		auto boardNode = new (std::nothrow) BoardNode();
		if (boardNode->init(solvedCallBack)) {
			boardNode->autorelease();
			return boardNode;
		}
		CCLOG("ERROR CREATING BOARD NODE");
		CC_SAFE_DELETE(boardNode);
		return nullptr;
	}

	BoardNode* BoardNode::create(boardPtr board, const std::function<void()>& solvedCallBack) {
		auto boardNode = new (std::nothrow) BoardNode(board);

		if (boardNode->init(solvedCallBack)) {
			boardNode->autorelease();
			return boardNode;
		}

		CCLOG("ERROR CREATING BOARD NODE");
		CC_SAFE_DELETE(boardNode);
		return nullptr;
	}

	bool BoardNode::init(const std::function<void()>& solvedCallBack) {

		cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SPRITE_SHIT_PLIST);
		_dotsNode = Node::create();
		_cellsNode = cocos2d::Node::create();
		_spriteBatch = cocos2d::SpriteBatchNode::create(SPRITE_SHIT_PNG);
		_selectionSprite = cocos2d::Sprite::createWithSpriteFrameName(CELL_SELECTION);
		_selectionSprite->setVisible(false);
		_selectionSprite->setLocalZOrder(100);

		_boardSolved = solvedCallBack;

		this->addChild(_spriteBatch);
		this->addChild(_selectionSprite, 100);
		this->addChild(_cellsNode, 0);
		this->addChild(_dotsNode, -100);

		scheduleUpdate();

		return true;
	}

	void BoardNode::addCell(CellSprite* cell, int zOrder) {
		_cellsNode->addChild(cell, zOrder);
	}

	void BoardNode::showSelection(bool show) {
		_selectionSprite->setVisible(show);
	}

	void BoardNode::hideSelection() {
		_selectionSprite->setVisible(false);
	}

	void BoardNode::setSelectionPosition(const cocos2d::Vec2& position) {
		_selectionSprite->setPosition(position);
	}

	void BoardNode::removeCells() {
		_cellsNode->removeAllChildrenWithCleanup(true);
	}

	void BoardNode::updateBoard(const boardPtr newBoard, /*out*/ bool& shouldUpdate) {

		CCLOG("UPDATE BOARD FNC");

		removeCells();

		TakeTen::Size oldBoardSize;
		if (_board) {
			oldBoardSize = _board->getSize();
		}

		_board = newBoard;
		auto newBoardSize = newBoard->getSize();

		shouldUpdate = oldBoardSize != newBoardSize;

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

		auto count = newBoardSize.getCount();
		auto boardContentSize = cocos2d::Size(newBoardSize.width * SPRITE_SIZE, newBoardSize.height * SPRITE_SIZE);
		auto boardPosition = cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + SPRITE_SIZE);

		setContentSize(boardContentSize);
		setPosition(boardPosition);

		if (shouldUpdate) {
			_dotsNode->removeAllChildrenWithCleanup(true);
		}

	}

	void BoardNode::rebuildBoard(bool isUndo) {
		if (isUndo) {
			_animationDelayTimer = -ANIMATION_DELAY_TIME;

			CCLOG("REBULID BOARD UNDO...");
			_board->undo();

		}

		hideSelection();
		_lastSelectedCell = nullptr;

		auto size = _board->getSize();
		auto count = size.getCount();

		Position cellPosition;
		Cell cell;
		char cellValue;
		int index = 0;

		updateColors();

		for (auto i = 0; i != count; ++i) {
			if (_cellButtons[i]) {
				continue;
			}

			cell = _board->getCellByIndex(i);
			cellValue = cell.value;
			cellPosition = cell.position;

			if (_rebuildDots) {
				auto dotPosition = CellSprite::cellPositionToVec2(cellPosition, _board->getSize());
				createDot(dotPosition, cocos2d::Color3B(100, 100, 100));
			}

			if (cellValue == -1) {
				continue;
			}

			auto cellSprite = CellSprite::create(cellValue, cellPosition, size, _colors[i]);
			addCell(cellSprite, 0);

			auto listener1 = cocos2d::EventListenerTouchOneByOne::create();
			listener1->setSwallowTouches(true);

			listener1->onTouchBegan = CC_CALLBACK_2(BoardNode::touchesBegan, this);
			listener1->onTouchMoved = CC_CALLBACK_2(BoardNode::touchesMoved, this);
			listener1->onTouchEnded = CC_CALLBACK_2(BoardNode::touchesEnd, this);
			listener1->onTouchCancelled = CC_CALLBACK_2(BoardNode::touchesEnd, this);

			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, cellSprite);

			if (isUndo) {
				_animationDelayTimer += ANIMATION_DELAY_TIME;
			}
			else {
				auto delayOrder = std::sqrtf(powf(cellPosition.column, 2) + powf(cellPosition.row, 2));
				_animationDelayTimer = (delayOrder * ANIMATION_DELAY_TIME) + INTRO_TIME;
			}

			cellSprite->show(_animationDelayTimer);
			_cellButtons[i] = cellSprite;

			index++;
		}

		_rebuildDots = false;
	}

	void BoardNode::createBoard(std::shared_ptr<TakeTen::Board> nextBoard) {

		for (auto &c : _cellsNode->getChildren()) {
			_eventDispatcher->removeEventListenersForTarget(c, true);
		}

		_animationDelayTimer = 0.0f;
		_cellButtons.clear();

		TakeTen::Size oldBoardSize;

		updateBoard(nextBoard, _rebuildDots);
		auto newBoardSize = nextBoard->getSize();

		if (_rebuildDots) {
			updateColors();
		}

		auto count = newBoardSize.getCount();
		for (auto i = 0; i != count; ++i) {
			_cellButtons.push_back(nullptr);
		}

		rebuildBoard();
	}

	void BoardNode::updateColors()
	{
		static int rotation = 0;

		static ColorHSB baseColors[4] = {
			ColorHSB(199, 25, 27),
			ColorHSB(33, 40, 77),
			ColorHSB(200, 23, 57),
			ColorHSB(5, 53, 57),
		};

		static std::vector<ColorHSB> colors;

		auto newBoardSize = _board->getSize();
		auto count = newBoardSize.getCount();
		
		if (colors.size() < 1 || colors.size() != count)
		{
			colors.clear();
			for (auto i = 0; i != count; ++i) {
				colors.push_back(ColorHSB());
			}
		}

		auto index0 = 0;
		auto index1 = newBoardSize.getIndex(newBoardSize.width - 1, 0);
		auto index2 = newBoardSize.getIndex(0, newBoardSize.height - 1);
		auto index3 = newBoardSize.getIndex(newBoardSize.width - 1, newBoardSize.height - 1);

		/*
		rotation++;

		

		rotation++;

		if (rotation > 3)
		{
			rotation = 0;
		}

		const ColorHSB tmpColors[4] = {
			ColorHSB(199, 25, 27),
			ColorHSB(33, 40, 77),
			ColorHSB(200, 23, 57),
			ColorHSB(5, 53, 57),
		};

		switch (rotation)
		{
		case 0:
		{
			baseColors[0] = tmpColors[0];
			baseColors[1] = tmpColors[1];
			baseColors[2] = tmpColors[2];
			baseColors[3] = tmpColors[3];
		}
		break;
		case 1:
		{
			baseColors[0] = tmpColors[2];
			baseColors[1] = tmpColors[0];
			baseColors[2] = tmpColors[3];
			baseColors[3] = tmpColors[1];
		}
		break;
		case 2:
		{
			baseColors[0] = tmpColors[3];
			baseColors[1] = tmpColors[2];
			baseColors[2] = tmpColors[1];
			baseColors[3] = tmpColors[0];
		}
		break;
		case 3:
		{
			baseColors[0] = tmpColors[1];
			baseColors[1] = tmpColors[3];
			baseColors[2] = tmpColors[0];
			baseColors[3] = tmpColors[2];
		}
		break;
		}

		colors[index0] = ColorHSB::lerp(baseColors[0], baseColors[2], tick / 100.f);
		colors[index1] = ColorHSB::lerp(baseColors[1], baseColors[0], tick / 100.f);
		colors[index2] = ColorHSB::lerp(baseColors[2], baseColors[3], tick / 100.f);
		colors[index3] = ColorHSB::lerp(baseColors[3], baseColors[1], tick / 100.f);

		*/

		colors[index0] = baseColors[0];
		colors[index1] = baseColors[1];
		colors[index2] = baseColors[2];
		colors[index3] = baseColors[3];

		if (Tools::randBool())
		{
			auto tmp = colors[index0];
			colors[index0] = colors[index1];
			colors[index1] = tmp;
		}
		if (Tools::randBool())
		{
			auto tmp = colors[index1];
			colors[index1] = colors[index2];
			colors[index2] = tmp;
		}
		if (Tools::randBool())
		{
			auto tmp = colors[index2];
			colors[index2] = colors[index3];
			colors[index3] = tmp;
		}
		if (Tools::randBool())
		{
			auto tmp = colors[index3];
			colors[index3] = colors[index0];
			colors[index0] = tmp;
		}

		_colors = ColorHSB::calculateColors(newBoardSize, colors);
	}

	void BoardNode::createDot(const cocos2d::Vec2& position, const cocos2d::Color3B& color) {
		auto actionDelay = cocos2d::DelayTime::create(CARD_FLIP_TIME / 2);
		auto actionScale = cocos2d::ScaleTo::create(CARD_FLIP_TIME / 2, 1.0f, 1.0f);
		auto easyScale = cocos2d::EaseBackOut::create(actionScale);

		auto actionRotate = cocos2d::RotateBy::create(CARD_FLIP_TIME, 90);
		auto easyRotate = cocos2d::EaseBackOut::create(actionRotate);

		auto sequenceScale = cocos2d::Sequence::create(actionDelay->clone(), easyScale, nullptr);
		auto sequenceRotate = cocos2d::Sequence::create(actionDelay->clone(), easyRotate, nullptr);

		auto empty = cocos2d::Sprite::createWithSpriteFrameName(CELL_EMPTY);
		empty->setPosition(position);
		empty->setScale(0.0f);
		empty->setRotation(-90.0f);
		empty->setColor(color);
		empty->runAction(sequenceScale);
		empty->runAction(sequenceRotate);
		_dotsNode->addChild(empty, -20);
	}

	bool BoardNode::touchesBegan(cocos2d::Touch* touch, cocos2d::Event* cEvent) {

		if (TakeTen::GameManager::getInstance()->isPause()) {
			return false;
		}

		auto target = static_cast<CellSprite*>(cEvent->getCurrentTarget());

		if (target) {
			auto locationInNode = target->convertToNodeSpace(touch->getLocation());
			auto size = target->getContentSize();
			for (auto &cell : _cellButtons) {
				if (cell == target) {
					auto rect = cocos2d::Rect(0, 0, size.width, size.height);
					if (rect.containsPoint(locationInNode)) {
						setSelectionPosition(cell->getBasePosition());
						selectCell(cell);
						return true;
					}
				}
			}
		}
		return false;
	}

	void BoardNode::touchesMoved(cocos2d::Touch* touch, cocos2d::Event* cEvent) {

		if (TakeTen::GameManager::getInstance()->isPause()) {
			return;
		}

		for (auto &cell : _cellButtons) {
			if (cell) {
				auto locationInNode = cell->convertToNodeSpace(touch->getLocation());
				auto size = cell->getContentSize();
				auto rect = cocos2d::Rect(0, 0, size.width, size.height);
				if (rect.containsPoint(locationInNode)) {
					if (!cell->isSelected()) {
						selectCell(cell);
						break;
					}
				}
				else {
					cell->unselect();
				}
			}
		}
		if (_lastSelectedCell) {
			showSelection();
			setSelectionPosition(_lastSelectedCell->getBasePosition());
		}
	}

	void BoardNode::touchesEnd(cocos2d::Touch* touch, cocos2d::Event* cEvent) {

		if (TakeTen::GameManager::getInstance()->isPause()) {
			return;
		}

		unselectAll();

		return;

		if (_lastSelectedCell) {
			showSelection();
			setSelectionPosition(_lastSelectedCell->getBasePosition());
		}
		else {
			hideSelection();
		}
	}

	bool BoardNode::selectCell(CellSprite* cell) {
		TakeTen::Pair pair;

		stopActionByTag(SELECTION_HIDE_ACT_TAG);

		_lastSelectedCell = cell;
		cell->select();

		showSelection();
		setSelectionPosition(cell->getBasePosition());

		auto gameManager = TakeTen::GameManager::getInstance();

		if (_board->selectCell(cell->getCellPosition(), pair)) {
			auto index1 = _board->getIndex(pair.cell1);
			auto index2 = _board->getIndex(pair.cell2);

			auto cell1 = _cellButtons[index1];
			auto cell2 = _cellButtons[index2];

			if (cell1) {
				_eventDispatcher->removeEventListenersForTarget(cell1, true);
				cell1->hide();
			}
			if (cell2) {
				_eventDispatcher->removeEventListenersForTarget(cell2, true);
				cell2->hide(ANIMATION_DELAY_TIME);
			}

			_cellButtons[index1] = nullptr;
			_cellButtons[index2] = nullptr;

			auto delayFn = cocos2d::DelayTime::create(ANIMATION_DELAY_TIME);
			auto hideFn = cocos2d::CallFunc::create([&](){
				this->hideSelection();
				if (this->getBoard()->isEmpty()) {
					_boardSolved();
				}
			});

			_lastSelectedCell = nullptr;
			gameManager->soundRemove();

			auto selectionHideSequence = cocos2d::Sequence::create(delayFn, hideFn, nullptr);
			selectionHideSequence->setTag(SELECTION_HIDE_ACT_TAG);

			runAction(selectionHideSequence);
			unselectAll();
			return true;
		}
		else {
			gameManager->soundSelect();
			return false;
		}
	}

	void BoardNode::unselectAll() {
		for (auto &cell : _cellButtons) {
			if (cell) {
				if (cell->isSelected()) {
					cell->unselect();
				}
			}
		}
	}
}



