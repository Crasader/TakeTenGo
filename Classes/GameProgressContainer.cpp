#include "GameProgressContainer.h"

namespace TakeTen {
	GameProgressContainer::GameProgressContainer()
		: size(0, 0), index(0), time(0) { }

	GameProgressContainer::GameProgressContainer(const TakeTen::Size& newSize, char newIndex, float newTime)
		: size(newSize), index(newIndex), time(newTime) { }
}