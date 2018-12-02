#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <cstdio>
#include <time.h>
#include <string>
#include "MtRand.h"
#include "Definitions.h"

namespace TakeTen {
	class Tools {
	public:

		static unsigned int generate() {

			static MTRand_int32 irand(static_cast<unsigned long>(time(NULL)));
			return irand();
		}

		static inline unsigned int randRange(const int min, const int max) {
			return (min == max ? min : min + (generate() % (int)(max - min + 1)));
		}

		static inline bool randBool() {
			return generate() > UINT_MAX / 2;
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

		static std::string getTimeString(const float seconds) {

			char timeStr[30];

			if (seconds < 60) {
				int sec = static_cast<int>(seconds);
				sprintf(timeStr, "00:%02d", sec);
			}
			else if (seconds < 3600) {
				int min = static_cast<int>(seconds / 60);
				int sec = static_cast<int>(seconds) % 60;
				sprintf(timeStr, "%02d:%02d", min, sec);
			}
			else {
				int time = static_cast<int>(seconds);
				int hour = time / 3600;
				time = time % 3600;
				int min = time / 60;
				time = time % 60;
				int sec = time;

				sprintf(timeStr, "%02d:%02d:%02d", hour, min, sec);
			}
			
			std::string result(timeStr);
			return result;
		}

		
	};
}





#endif