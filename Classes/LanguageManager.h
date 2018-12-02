#pragma once
#ifndef __LANGUAGE_MANAGER_H__
#define __LANGUAGE_MANAGER_H__

#include <string>

#include "cocos2d.h"

#include "json\rapidjson.h"
#include "json\document.h"

namespace TakeTen {
	class LanguageManager {
	public:
		static std::shared_ptr<LanguageManager> getInstance();

		std::string getStringForKey(const std::string key);
		std::string getString(const std::string key);
		std::string getFont();


	private:
		rapidjson::Document _document; // current document with language data
		LanguageManager(); // constructor is private

		static std::shared_ptr<LanguageManager> _instance;
	};
}
#endif