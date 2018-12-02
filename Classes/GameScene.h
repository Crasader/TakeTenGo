#pragma once
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

//#include <vector>
//#include <memory>
#include "cocos2d.h"

#include "CellSprite.h"
#include "MenuButton.h"
#include "Board.h"
#include "ColorHSB.h"
#include "BoardNode.h"

namespace TakeTen {
	class GameScene : public cocos2d::LayerColor {
	public:

		void newGame();

		static cocos2d::Scene* createScene(const Size& size);
		virtual bool init(const Size& size);

		static GameScene* create(const Size& size);
		void onExit();

		void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	private:
		~GameScene();

		//bool touchesBegan(cocos2d::Touch* touch, cocos2d::Event* cEvent);
		//void touchesMoved(cocos2d::Touch* touch, cocos2d::Event* cEvent);
		//void touchesEnd(cocos2d::Touch* touch, cocos2d::Event* cEvent);

		//bool selectCell(CellSprite* cell);

		void createLabels();
		void createMenuButtons();
		void createBoard();

		void congratulations();

		void menuCloseCallback(cocos2d::Ref* pSender);
		void menuUndo(cocos2d::Ref* pSender);
		void menuQuit(cocos2d::Ref* pSender);
		void menuPause(cocos2d::Ref* pSender);

		void update(float dt) override;

		void updateLabelsPositions();
		void updateTimeLabel();

		void updateMenuButtons();
		void introAnimationCompleted();

		cocos2d::Label* _timeLabel;
		BoardNode* _boardNode;

		float _gameTimer;
		float _animationDelayTimer;

		MenuButton* _quitMenu;
		MenuButton* _undoMenu;
		MenuButton* _pauseMenu;

		bool _won;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
		void afterScreenCaptured(bool succeed, const std::string& outputFile);
		void captureScreen();
#endif
	};
}
#endif 


