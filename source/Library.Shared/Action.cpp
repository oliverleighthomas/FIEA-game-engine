#include "pch.h"
#include "Action.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(Action);

	Action::Action(RTTI::IdType idType) :
		Attributed(idType)
	{
	}

	Action::Action(RTTI::IdType idType, const std::string& name) :
		Attributed(idType), name(name)
	{
	}

	const std::string& Action::Name() const {
		return name;
	}

	void Action::SetName(const std::string& newName) {
		name = newName;
	}

	Vector<Signature> Action::Signatures() {
		return Vector<Signature> {
			{ "Name", Datum::DatumTypes::String, 1, offsetof(Action, name) },
		};
	}
}