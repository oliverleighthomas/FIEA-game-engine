#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(ActionList);

	ActionList::ActionList() :
		Action(TypeIdClass())
	{
		actions = Find("Actions");
		assert(actions != nullptr);
	}

	ActionList::ActionList(const std::string& name) :
		Action(TypeIdClass(), name)
	{
		actions = Find("Actions");
		assert(actions != nullptr);
	}

	ActionList::ActionList(RTTI::IdType idType) :
		Action(idType)
	{
		actions = Find("Actions");
		assert(actions != nullptr);
	}

	ActionList::ActionList(RTTI::IdType idType, const std::string& name) :
		Action(idType, name)
	{
		actions = Find("Actions");
		assert(actions != nullptr);
	}

	void ActionList::Update(const GameTime& time) {
		for (size_t i = 0; i < actions->Size(); ++i) {
			Scope& action = (*actions)[i];
			action.As<Action>()->Update(time);
		}
	}

	Datum* ActionList::Actions() const {
		return actions;
	}

	Vector<Signature> ActionList::Signatures() {
		return Vector<Signature> {
			{ "Name", Datum::DatumTypes::String, 1, offsetof(ActionList, name) },
			{ "Actions", Datum::DatumTypes::Table, 1, offsetof(ActionList, actions) },
		};
	}

}