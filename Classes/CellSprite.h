#pragma once
#ifndef __CELL_SPRITE_H__
#define __CELL_SPRITE_H__

#include "cocos2d.h"
#include "Position.h"
#include "Cell.h"
#include "Size.h"
#include "ColorHSB.h"

namespace TakeTen {
	class CellSprite : public cocos2d::Sprite {
	public:

		~CellSprite();

		static CellSprite* create(const unsigned char value, const Position& position, const Size& boardSize, const ColorHSB& color);

		Position& getCellPosition();

		void select();
		void unselect();

		bool isSelected() const {
			return _isSelected;
		}

		void pop();
		void show(float delayTimer);

		void hide(float delayTimer = 0.0f);

		static cocos2d::Vec2 cellPositionToVec2(const Position& position, const Size& boardSize);

		inline const cocos2d::Vec2& getBasePosition() const {
			return _basePosition;
		}

		inline void setBasePosition(const cocos2d::Vec2& basePosition) {
			_basePosition = basePosition;
		}

		unsigned char getValue() const { return _value; }

	private:

		void playSound();

		void resetZOrder();

		bool _isSelected;
		unsigned char _value;
		cocos2d::Sprite* _digitSprite;

		cocos2d::Vec2 _basePosition;
		Position _cellPosition;

		int _zOrder;

		CellSprite();
		

		Position _position;
	};
}





#endif