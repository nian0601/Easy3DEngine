#pragma once
#include "Component.h"

class EmitterComponent : public Component
{
public:
	EmitterComponent(Entity& aEntity);
	~EmitterComponent();
};

