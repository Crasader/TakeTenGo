#include "Definitions.h"
#include "LanguageManager.h"

namespace TakeTen {

	std::shared_ptr<LanguageManager> LanguageManager::_instance = nullptr;
	LanguageManager::LanguageManager() {
		std::string fileName;
		switch (cocos2d::Application::getInstance()->getCurrentLanguage()) 	{
		case cocos2d::LanguageType::RUSSIAN:
			fileName = "ru.json";
			break;
		case cocos2d::LanguageType::GERMAN:		
			fileName = "de.json";
			break;
		case cocos2d::LanguageType::PORTUGUESE:		
			fileName = "pt.json";
			break;
		case cocos2d::LanguageType::SPANISH:		
			fileName = "es.json";
			break;
		case cocos2d::LanguageType::ITALIAN:		
			fileName = "it.json";
			break;
		case cocos2d::LanguageType::CHINESE:
			fileName = "zh.json";
			break;
		case cocos2d::LanguageType::JAPANESE:
			fileName = "ja.json";
			break;
		case cocos2d::LanguageType::KOREAN:
			fileName = "ko.json";
			break;
		case cocos2d::LanguageType::FRENCH:		
			fileName = "fr.json";
			break;
		case cocos2d::LanguageType::DUTCH:			
			fileName = "du.json";
			break;
			/*
			case cocos2d::LanguageType::POLISH:
			fileName = "po.json";
			break;
			case  LanguageType::HUNGARIAN:
			fileName = "hu.json";
			break;
			case  LanguageType::ARABIC:
			fileName = "ar.json";
			break;
			case  LanguageType::NORWEGIAN:		***
			fileName = "nw.json";
			break;
			
			*/
		default:
			fileName = "en.json";
			break;
		};

		auto clearContent = cocos2d::FileUtils::getInstance()->getStringFromFile(fileName);
		_document.Parse<0>(clearContent.c_str());
		if (_document.HasParseError()) {
			CCLOG("Language file parsing error!");
			return;
		}
	}

	std::string LanguageManager::getFont() {

		switch (cocos2d::Application::getInstance()->getCurrentLanguage()) 	{
		case  cocos2d::LanguageType::CHINESE:
			return "fonts/zh/bitmapFont.fnt";
		case  cocos2d::LanguageType::JAPANESE:
			return "fonts/ja/bitmapFont.fnt";
		case  cocos2d::LanguageType::KOREAN:
			return "fonts/ko/bitmapFont.fnt";
		default:
			return UI_FONT_FILE_PATH;
		}
		return UI_FONT_FILE_PATH;
	}

	std::shared_ptr<LanguageManager> LanguageManager::getInstance() {
		if (!_instance) {
			_instance = std::shared_ptr<LanguageManager>(new (std::nothrow)LanguageManager());
			CCASSERT(_instance, "FATAL: Not enough memory");
		}
		return _instance;
	}

	std::string LanguageManager::getStringForKey(const std::string key) {
		return _document[key.c_str()].GetString();
	}

	std::string  LanguageManager::getString(const std::string key) {
		return getInstance()->getStringForKey(key);
	}

}