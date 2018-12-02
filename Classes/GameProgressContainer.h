#pragma once
#ifndef __GAME_PROGRESS_CONTAINER_H__
#define __GAME_PROGRESS_CONTAINER_H__

#include "Size.h"

namespace TakeTen {
	struct GameProgressContainer {
		GameProgressContainer();
		GameProgressContainer(const Size& size, char index, float time);
		Size size;
		size_t index;
		float time;
	};

}

#endif