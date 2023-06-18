#pragma once
#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine {
	class TestAction : public Action
	{
		RTTI_DECLARATIONS(TestAction, Action);

	public:
		TestAction();
		TestAction(const TestAction& rhs) = default;
		TestAction(TestAction&& rhs) = default;
		TestAction& operator=(const TestAction& rhs) = default;
		TestAction& operator=(TestAction&& rhs) = default;
		~TestAction() = default;

		int Count() const;
		
		virtual void Update(const GameTime&) override;
		static Vector<FieaGameEngine::Signature> Signatures();

	private:
		int updateCount{ 0 };
	};
	CONCRETE_FACTORY(TestAction, Scope);
}


