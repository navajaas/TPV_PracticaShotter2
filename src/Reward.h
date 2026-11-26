#pragma once
#include "SceneObject.h"
class Reward: public SceneObject
{
public:
	enum RewardType{
		LEVEL_UP,
		EXTRA_ARROWS
	};
};

