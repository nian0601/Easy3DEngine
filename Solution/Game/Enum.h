#pragma once

enum class eComponent
{
	NOT_USED,
	GRAPHIC,
	INPUT,
	PHYSICS,
	SCRIPT,
	COLLISION,
	HEALTH,
	EMITTER,
};

enum class eEntityType
{
	PLAYER,
	PICKABLE,
};