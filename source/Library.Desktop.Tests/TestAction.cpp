#include "pch.h"
#include "TestAction.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(TestAction);

	TestAction::TestAction() :
		Action(TypeIdClass(), "Test")
	{
	}

	void TestAction::Update(const GameTime&) {
		++updateCount;
	}

	int TestAction::Count() const {
		return updateCount;
	}

	Vector<FieaGameEngine::Signature> TestAction::Signatures() {
		return Vector<Signature> {
			{ "Name", Datum::DatumTypes::String, 1, offsetof(TestAction, name) }
		};
	}
}