#pragma once
#include "GameObject.h"
#include "GameTime.h"

namespace UnitTests {
	class TestGameObject : public FieaGameEngine::GameObject
	{
		RTTI_DECLARATIONS(TestGameObject, GameObject);
	public:
		virtual void Update(const FieaGameEngine::GameTime& time) override;
		int timesUpdated{ 0 };
	};
	CONCRETE_FACTORY(TestGameObject, FieaGameEngine::Scope);
}

