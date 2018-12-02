#ifndef __CELL_SPRITE_H__
#define __CELL_SPRITE_H__

#include "cocos2d.h"
#include "Position.h"
#include "Cell.h"
#include "Size.h"
#include "ColorHSB.h"

class CellSprite : public cocos2d::Sprite
{
public:
	static CellSprite* create(const unsigned char value, const TakeTen::Position& position, const TakeTen::Size& boardSize, const ColorHSB& color);

	TakeTen::Position& getCellPosition();
	
	void select();
	void unselect();

	bool isSelected() const {
		return _isSelected;
	}

	void pop();
	void show(float delayTimer);

	void hide();

	static cocos2d::Vec2 cellPositionToVec2(const TakeTen::Position& position, const TakeTen::Size& boardSize);

private:
	
	void playSound();

	bool _isSelected;
	unsigned char _value;
	cocos2d::Sprite* _digitSprite;

	CellSprite();
	~CellSprite();

	TakeTen::Position _position;

};



#endif