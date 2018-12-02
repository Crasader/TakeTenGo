#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <vector>
#include <memory>
#include "cocos2d.h"
#include "CellSprite.h"
#include "Board.h"
#include "ColorHSB.h"

class GameScene : public cocos2d::LayerColor
{
public:

	void newGame();

    static cocos2d::Scene* createScene(const TakeTen::Size& size);
	virtual bool init(const TakeTen::Size& size);
	
	static GameScene* create(const TakeTen::Size& size) { 
		GameScene *pRet = new GameScene();
		if (pRet && pRet->init(size)) {
			pRet->autorelease();
			return pRet;
		}
		else {
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}

	void onExit();

private:
	~GameScene();

	bool touchesBegan(cocos2d::Touch* touch, cocos2d::Event* cEvent);
	void touchesMoved(cocos2d::Touch* touch, cocos2d::Event* cEvent);
	void touchesEnd(cocos2d::Touch* touch, cocos2d::Event* cEvent);

	void selectCell(CellSprite* cell);

	void createLabels();
	void createMenuButtons();
	void createBoard();

	void congratulations();

	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuUndo(cocos2d::Ref* pSender);
	void menuQuit(cocos2d::Ref* pSender);
	void menuHint(cocos2d::Ref* pSender);
	void menuSettings(cocos2d::Ref* pSender);
	void update(float dt) override;

	void updateTimeLabel();
	void updateGameCountLabel();

	void updateMenuButtons();

	void resetTimers();

	void rebuildBoard();

	void createDot(const cocos2d::Vec2& position, const cocos2d::Color3B& color);

	void unselectAll();

	CellSprite* _lastSelectedCell;

	std::vector<ColorHSB> _colors;
	std::vector<CellSprite*> _cellButtons;

	cocos2d::Node* _dotsNode;
	cocos2d::Node* _boardNode;
	cocos2d::SpriteBatchNode* _spriteBatch;

	cocos2d::Sprite* _selectionSprite;

	cocos2d::Label* _timeLabel;
	cocos2d::Label* _countLabel;
	
	std::shared_ptr<TakeTen::Board> _gameBoard;

	float _gameTimer;
	float _animationDelayTimer;

	cocos2d::MenuItem* _undoMenu;
	cocos2d::MenuItem* _pauseMenu;

	bool _won;
	bool _rebuildDots;

	bool _cellsRemoved;
};

#endif // __HELLOWORLD_SCENE_H__
