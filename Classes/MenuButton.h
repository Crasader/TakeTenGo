#pragma once
#ifndef __MENU_BUTTON_H__
#define __MENU_BUTTON_H__

#include <string>
#include "cocos2d.h"
#include "ColorHSB.h"

namespace TakeTen {
	class MenuButton : public cocos2d::MenuItemSprite {
	public:
		~MenuButton();

		static MenuButton* create(const std::string& sprite, const std::string& pressedSprite, const ColorHSB& pressedColor, const cocos2d::ccMenuCallback& callBack);
		static MenuButton* create(const std::string& sprite, const ColorHSB& pressedColor, const float delayTimer, const cocos2d::ccMenuCallback& callBack);

		void disable();
		void enable();
	};
}




#endif // !__MENU_BUTTON_H__

