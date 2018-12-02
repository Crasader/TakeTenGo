#pragma once
#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <cstdio>
#include <time.h>
#include <string>
#include <stdarg.h>
#include <memory> 
#include "MtRand.h"
#include "Definitions.h"
#include "cocos2d.h"

namespace TakeTen {
	class Tools {
	public:
			

		static std::string getFilePath(const std::string& fileName) {
			//return cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
			auto path = cocos2d::FileUtils::getInstance()->getWritablePath();
			path.append(fileName);
			return path;

		}

		static void flipInItem(cocos2d::Node* item, float delayTimer, float scaleTo = 1.0f);

		static void cropSprite(cocos2d::Sprite* sprite, float amount, /*out*/ cocos2d::Rect& rect);

		/*
		static std::string stringFormat(const std::string fmt_str, ...) {
			int final_n, n = static_cast<int>(fmt_str.size()) * 2;
			std::string str;
			std::unique_ptr<char[]> formatted;
			va_list ap;
			while (1) {
				formatted.reset(new char[n]);
				strcpy(&formatted[0], fmt_str.c_str());
				va_start(ap, fmt_str);
				final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
				va_end(ap);
				if (final_n < 0 || final_n >= n) {
					n += abs(final_n - n + 1);
				}
				else {
					break;
				}
					
			}
			return std::string(formatted.get());
		}
		*/
		static unsigned int generate() {

			static MTRand_int32 irand(static_cast<unsigned long>(time(nullptr)));
			return irand();
		}

		static inline unsigned int randRange(const int min, const int max) {
			return (min == max ? min : min + (generate() % (int)(max - min + 1)));
		}

		static inline bool randBool() {
			return generate() % 2 != 0;
		}

		static inline bool randBool(const unsigned int chance) {
			return randRange(0, 100) < chance;
		}

		static inline float lerp(const float a, const float b, const float delta) {
			return ((1.0f - delta)*a + b*delta);
		}

		static inline double doubleMaximum(double r, double g, double b) {
			if (r > g) {
				if (r > b) return r;
				else return b;
			}
			else {
				if (g > b) return g;
				else return b;
			}
			return 0;
		}

		static inline double doubleMinimum(double r, double g, double b) {
			if (r < g) {
				if (r < b) return r;
				else return b;
			}
			else {
				if (g < b) return g;
				else return b;
			}
			return 0;
		}

		static std::string getTimeString(const int seconds) {

			char buff[100];
			if (seconds < 60) {
				int sec = seconds;
				sprintf(buff, "00:%02d", sec);
				std::string buffAsStdStr(buff);
				return buffAsStdStr;
			}
			else if (seconds < 3600) {
				int min = seconds / 60;
				int sec = seconds % 60;
				sprintf(buff, "%02d:%02d", min, sec);
				std::string buffAsStdStr(buff);
				return buffAsStdStr;
			}

			int time = seconds;
			int hour = time / 3600;
			time = time % 3600;
			int min = time / 60;
			time = time % 60;
			int sec = time;
			sprintf(buff, "%02d:%02d:%02d", hour, min, sec);

			std::string buffAsStdStr(buff);
			return buffAsStdStr;
		}

		
	};
}





#endif