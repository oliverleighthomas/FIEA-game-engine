#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "GameState.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(ReactionAttributed);

	ReactionAttributed::ReactionAttributed() :
		Reaction(TypeIdClass())
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const std::string subType) :
		Reaction(TypeIdClass()), subType{ subType }
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const std::string subType, const std::string name) :
		Reaction(TypeIdClass(), name), subType{ subType }
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed() {
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	void ReactionAttributed::Notify(const EventPublisher& publisher) {
		assert(publisher.Is(Event<EventMessageAttributed>::TypeIdClass()));

		auto& e = static_cast<const Event<EventMessageAttributed>&>(publisher);
		auto& message = e.Message();

		if (message.GetSubType() == subType) {
			auto arguments = message.AuxiliaryAttributes();
			for (auto& arg : arguments) {
				for (std::size_t i = 0; i < actions->Size(); ++i) {
					actions->GetScope(i)->Append(arg->first).SetFromString(arg->second.ToString());
				}
			}
			Update(GameState::GetGameTime());
		}
	}

	void ReactionAttributed::Update(const GameTime& time) {
		ActionList::Update(time);
	}

	const std::string& ReactionAttributed::GetSubType() const {
		return subType;
	}

	void ReactionAttributed::SetSubType(const std::string& newSubType) {
		subType = newSubType;
	}


	Action* ReactionAttributed::CreateAction(const std::string& className, const std::string& instanceName) {
		auto action = Factory<Scope>::Create(className)->As<Action>();
		assert(action != nullptr);
		action->SetName(instanceName);
		Adopt(*action, "Actions");
		return action;
	}

	ReactionAttributed* ReactionAttributed::Clone() {
		return new ReactionAttributed(subType, name);
	}

	Vector<Signature> ReactionAttributed::Signatures() {
		return Vector<Signature> {
			{ "Name", Datum::DatumTypes::String, 1, offsetof(ReactionAttributed, name) },
			{ "Actions", Datum::DatumTypes::Table, 1, offsetof(ReactionAttributed, actions) },
			{ "SubType", Datum::DatumTypes::String, 1, offsetof(ReactionAttributed, subType) }
		};
	}
}