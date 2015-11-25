#pragma once

struct ToggleInputNote
{
	ToggleInputNote(bool aInputEnabled);

	bool myInputEnabled;
};

inline ToggleInputNote::ToggleInputNote(bool aInputEnabled)
	: myInputEnabled(aInputEnabled)
{

}