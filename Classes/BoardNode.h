#pragma once
#ifndef __BOARD_NODE_H__
#define __BOARD_NODE_H__

#include <functional>

#include "cocos2d.h"

#include "ColorHSB.h"
#include "CellSprite.h"
#include "BoardManager.h"

namespace TakeTen {
	class BoardNode : public cocos2d::Node {
	public:
		static BoardNode* create(const std::function<void()>& solvedCallBack);
		static BoardNode* create(boardPtr board, const std::function<void()>& solvedCallBack);

		inline boardPtr getBoard() {
			return _board;
		}

		void addCell(CellSprite* cell, int zOrder);
		void removeCells();

		bool init(const std::function<void()>& solvedCallBack);

		void showSelection(bool = true);
		void hideSelection();

		void setSelectionPosition(const cocos2d::Vec2& position);

		void createDot(const cocos2d::Vec2& position, const cocos2d::Color3B& color);

		void createBoard(std::shared_ptr<TakeTen::Board> board);
		void rebuildBoard(bool isUndo = false);

		void updateColors();
		//void update(float dt) override;

		~BoardNode();

	private:
		BoardNode();
		BoardNode(const boardPtr board);

		void updateBoard(boardPtr board, /*out*/ bool& shouldUpdate);
		

		bool touchesBegan(cocos2d::Touch* touch, cocos2d::Event* cEvent);
		void touchesMoved(cocos2d::Touch* touch, cocos2d::Event* cEvent);
		void touchesEnd(cocos2d::Touch* touch, cocos2d::Event* cEvent);
		
		void unselectAll();

		bool selectCell(CellSprite* cell);

	private:

		float _animationDelayTimer;
		bool _rebuildDots;

		CellSprite* _lastSelectedCell;

		std::vector<CellSprite*> _cellButtons;
		std::vector<ColorHSB> _colors;

		cocos2d::SpriteBatchNode* _spriteBatch;
		cocos2d::Sprite* _selectionSprite;
		cocos2d::Node* _cellsNode;
		cocos2d::Node* _dotsNode;

		std::shared_ptr<TakeTen::Board> _board;

		std::function<void()> _boardSolved;
	};
}


#endif


