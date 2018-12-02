#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__


#include <limits.h>
#include "cocos2d.h"
#include "Tools.h"

#define MENU_PRACTICE_TAG		0
#define MENU_EASY_TAG			1
#define MENU_MEDIUM_TAG			2
#define MENU_HARD_TAG			3
#define MENU_ULTIMATE_TAG		4
#define MENU_HARDCORE_TAG		5

#define HALF_UINTMAX			UINT_MAX / 2

//TransitionFlipAngular

#define SPLASH_DISPLAY_TIME		0.5f

#define SCENE_TRANSITION		cocos2d::TransitionFade::create 
#define TRANSITION_TIME			0.45f

#define CONGRATULATION_TIME		0.45f

#define CARD_FLIP_TIME			0.75f
#define ANIMATION_DELAY_TIME	CARD_FLIP_TIME / 3
#define SKEW_AMOUNT				12
#define PRESSED_SCALE			0.85f
#define SPRITE_SIZE				200.0f
#define BOARDS_TO_GENERATE		100

#define UNDO_COST				3.0f
#define HINT_COST				5.0f

namespace TakeTen {

	enum GameDifficuty {
		Practice, Easy, Medium, Hard, Ultimate, Hardcore, Completed
	};
	
	
}




#endif
