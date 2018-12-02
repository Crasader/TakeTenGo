#pragma once
#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include <vector>
#include "cocos2d.h"

#include "CellSprite.h"
#include "Size.h"
#include "BoardNode.h"

namespace TakeTen {
	class HelpScene : public cocos2d::LayerColor {
	public:

		struct BoardHolder {
			TakeTen::Size size;
			std::vector<char> data;
		};

		static cocos2d::Scene* createScene();
		virtual bool init();
		CREATE_FUNC(HelpScene);

		void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	private:

		~HelpScene();

		void menuNext(cocos2d::Ref* pSender);
		void menuBack(cocos2d::Ref* pSender);
		void createMenuButtons();
		void createLabels();
		void updateBoard();
		void updateHelpText();
		void mainMenuLoader(float dt = 0.0f);

		bool touchesBegan(cocos2d::Touch* touch, cocos2d::Event* cEvent);
		void touchesMoved(cocos2d::Touch* touch, cocos2d::Event* cEvent);
		void touchesEnd(cocos2d::Touch* touch, cocos2d::Event* cEvent);

		cocos2d::Sprite* _selectionSprite;
		CellSprite* _firstCell;
		cocos2d::Label* _helpText;
		cocos2d::Label* _headerText;
		int _currentHelpSet;

		BoardNode* _boardNode;
	};
}

#endif