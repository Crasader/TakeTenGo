#pragma once
#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <string>
#include <limits.h>
#include "cocos2d.h"
#include "Tools.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#define AMAZON						0
#define SLIDEME						0
#else
#define AMAZON						1
#define SLIDEME						0
#endif

#define USE_FIVE					true

#define ARRAY_COUNT(a)				(sizeof((a))/sizeof((a[0])))

#define BOARDS_SIZES				{ { 3, 3 }, { 3, 4 }, { 4, 4 }, { 3, 6 }, { 4, 5 }, { 5, 6 } }
#define BOARDS_TO_GENERATE			{ 200, 1000, 1000, 1000, 1000, 1000 }
#define GENERATE_BOARDS				0

#define PLAYER_FILE					TakeTen::Tools::getFilePath("taketengoplayer.dat")
#define BOARDS_FILE					cocos2d::FileUtils::getInstance()->fullPathForFilename("boards.dat")

#define SPRITE_SHIT_PLIST			"spriteSheet.plist"
#define SPRITE_SHIT_PNG				"spriteSheet.png"

#define LOGO_IMAGE					"tt_logo.png"

#define MENU_CELL					"tt_MenuCell.png"
#define ICON_MENU_UNDO				"tt_MenuUndo.png"
#define ICON_MENU_PAUSE				"tt_MenuPause.png"
#define ICON_MENU_PLAY				"tt_MenuPlay.png"
#define ICON_MENU_SOUND_ON			"tt_MenuSoundOn.png"
#define ICON_MENU_SOUND_OFF			"tt_MenuSoundOff.png"
#define ICON_MENU_HELP				"tt_MenuHelp.png"
#define ICON_MENU_SCORE				"tt_MenuScores.png"
#define ICON_MENU_RATE				"tt_MenuRate.png"
#define ICON_MENU_QUIT				"tt_MenuQuit.png"
#define ICON_MENU_GOLD_LEFT			"tt_MenuDiffGoldLeft.png"
#define ICON_MENU_GOLD_RIGHT		"tt_MenuDiffGoldRight.png"
#define ICON_MENU_DIFFICULTY_LVL	"tt_MenuDiff%02d.png"

#define CELL_SPRITE					"tt_Cell.png"
#define CELL_DIGITS					"tt_Num%02d.png"
#define CELL_SELECTION				"tt_Selection.png"
#define CELL_EMPTY					"tt_EmptyCell.png"

#define SOUND_INTRO					"ah.wav"
#define SOUND_TICK					"tick.wav"
#define SOUND_WON					"congratulations.wav"
#define SOUND_UNDO					"undo.wav"
#define SOUND_REMOVE				"remove.wav"

enum DifficultyTags
{
	MENU_PRACTICE_TAG, MENU_EASY_TAG, MENU_MEDIUM_TAG,
	MENU_HARD_TAG, MENU_ULTIMATE_TAG, MENU_HARDCORE_TAG
};

#define MENU_BUTTONS_Y				-550

#define CELL_TAG					5987987

#define UI_FONT_FILE_PATH			"fonts/ui/uiFont.fnt"
#define FONT_FILE_PATH				TakeTen::LanguageManager::getInstance()->getFont()


#define SELECTION_HIDE_ACT_TAG		145567

#define HALF_UINTMAX				UINT_MAX / 2

#define BG_SPEED					80.0f
#define BG_FADE_TIME				13.0f

#define SPLASH_DISPLAY_TIME			2.5f

#define SCENE_TRANSITION			cocos2d::TransitionFade::create 
#define TRANSITION_TIME				0.45f

#define CONGRATULATION_TIME			0.5f
#define INTRO_TIME					0.55f

#define CARD_FLIP_TIME				0.75f
#define ANIMATION_DELAY_TIME		CARD_FLIP_TIME / 3
#define SKEW_AMOUNT					12
#define PRESSED_SCALE				0.85f
#define SPRITE_SIZE					200.0f
#define MENU_SIZE_WIDTH				400.0f
#define MENU_SIZE_HEIGHT			300.0f

#define UNDO_COST					0.0f
#define HINT_COST					0.0f

#define TOTAL_HELP_PAGES			6

#define GET_STRING(i)				TakeTen::LanguageManager::getInstance()->getStringForKey(i)
#define SELECT_DIFFICULTY_STRING	GET_STRING("SelectDifficulty")
#define HELP_HEADER					GET_STRING("HelpHeader")
#define PAUSE_STRING				GET_STRING("Pause")
#define PAUSE_TIME					GET_STRING("PauseTime")
#define PAUSE_PROGRESS				GET_STRING("PauseProgress")
#define WORLD_BEST					GET_STRING("PauseBestTime")
#define SCORES_STRING				GET_STRING("ScoresHeader")
#define SCORES_PLAYER				GET_STRING("ScoresPlayer")
#define SCORES_WORLD				GET_STRING("ScoresWorld")
#define REPLAY_STRING				GET_STRING("Replay")

#define DIFFICULTY(i)				GET_STRING("Difficulty" + std::to_string(i))

#define FONT_SCALE_FACTOR			cocos2d::Director::getInstance()->getContentScaleFactor()

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#if AMAZON
#define RATE_LINK					"http://www.amazon.com/gp/mas/dl/android?p=com.shadowbiz.taketengo"
#else
#define RATE_LINK					"market://details?id=com.shadowbiz.taketengo"
#endif
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#define RATE_LINK					"http://windowsphone.com/s?appId=93cfefdb-439b-4dcf-b486-cd1cbd28f6cf"
#else 
#define RATE_LINK					"http://www.shadowbizgame.com"
#endif

namespace TakeTen {
	enum GameDifficuty {
		Practice, Easy, Medium, Hard, Ultimate, Hardcore, Completed
	};
}




#endif
