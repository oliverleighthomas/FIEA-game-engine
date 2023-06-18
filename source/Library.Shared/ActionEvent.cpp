#include "pch.h"
#include "EventMessageAttributed.h"
#include "GameState.h"
#include "Event.h"
#include "ActionEvent.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(ActionEvent);

	ActionEvent::ActionEvent() :
		Action(TypeIdClass(), "ActionEvent")
	{
	}

	const std::string& ActionEvent::GetSubType() const {
		return subType;
	}

	void ActionEvent::SetSubType(const std::string& newSubType) {
		subType = newSubType;
	}

	uint32_t ActionEvent::GetDelay() const {
		return delay;
	}

	void ActionEvent::SetDelay(uint32_t newDelay) {
		delay = newDelay;
	}

	void ActionEvent::Update(const GameTime& time) {
		EventMessageAttributed e(subType);

		for (size_t i = 0; i < AuxiliaryAttributes().Size(); ++i) {
			e.Append(AuxiliaryAttributes()[i]->first) = AuxiliaryAttributes()[i]->second;
		}

		GameState::GetEventQueue()->Enqueue(std::make_shared<Event<EventMessageAttributed>>(std::move(e)), time, std::chrono::milliseconds(delay));
	}

	ActionEvent* ActionEvent::Clone() {
		return new ActionEvent(*this);
	}

	Vector<Signature> ActionEvent::Signatures() {
		return Vector<Signature>{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(ActionEvent, name) },
			{ "Subtype", Datum::DatumTypes::String, 1, offsetof(ActionEvent, subType) },
			{ "Delay", Datum::DatumTypes::Integer, 1, offsetof(ActionEvent, delay) }
		};
	}
}

