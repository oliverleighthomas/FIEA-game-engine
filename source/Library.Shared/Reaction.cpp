#include "pch.h"
#include "Reaction.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(Reaction);

	Reaction::Reaction(RTTI::IdType idType) :
		ActionList{ idType }
	{
	}

	Reaction::Reaction(RTTI::IdType idType, const std::string& name) :
		ActionList{ idType, name }
	{
	}
}