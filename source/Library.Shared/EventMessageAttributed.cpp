#include "pch.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(EventMessageAttributed);

	EventMessageAttributed::EventMessageAttributed() :
		Attributed(TypeIdClass())
	{
	}

	EventMessageAttributed::EventMessageAttributed(const std::string& subType) :
		Attributed(TypeIdClass()), subType(subType)
	{
	}

	const std::string& EventMessageAttributed::GetSubType() const {
		return subType;
	}

	void EventMessageAttributed::SetSubType(const std::string& newSubType) {
		subType = newSubType;
	}

	EventMessageAttributed* EventMessageAttributed::Clone() {
		return new EventMessageAttributed(*this);
	}

	Vector<Signature> EventMessageAttributed::Signatures() {
		return Vector<Signature>{
			{ "SubType", Datum::DatumTypes::String, 1, offsetof(EventMessageAttributed, subType) },
		};
	}
}