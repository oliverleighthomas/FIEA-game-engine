#include "pch.h"
#include "ActionIncrement.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(ActionIncrement);

	ActionIncrement::ActionIncrement() :
		Action(TypeIdClass(), "Increment")
	{
	}

	void ActionIncrement::Update(const GameTime&) {
		Datum* target = Search(targetName);
		assert(target != nullptr);

		target->GetInt() += step;
	}

	std::string ActionIncrement::TargetName() const {
		return targetName;
	}

	int ActionIncrement::Step() const {
		return step;
	}

	Vector<FieaGameEngine::Signature> ActionIncrement::Signatures() {
		return Vector<Signature> {
			{ "Name", Datum::DatumTypes::String, 1, offsetof(ActionIncrement, name) },
			{ "Target", Datum::DatumTypes::String, 1, offsetof(ActionIncrement, targetName) },
			{ "Step", Datum::DatumTypes::Integer, 1, offsetof(ActionIncrement, step) }
		};
	}
}
