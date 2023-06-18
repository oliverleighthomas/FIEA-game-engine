#include "pch.h"
#include "EventSubscriber.h"
#include "EventPublisher.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(EventPublisher);

	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers) :
		subscribers{ &subscribers }
	{
	}

	void EventPublisher::Deliver() const {
		for (EventSubscriber* subscriber : *subscribers) {
			subscriber->Notify(*this);
		}
	}
}