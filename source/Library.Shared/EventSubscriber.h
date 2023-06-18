#pragma once
#include "EventPublisher.h"

namespace FieaGameEngine {
	class EventSubscriber {

	public:
		virtual ~EventSubscriber() = default;

		virtual void Notify(const EventPublisher& publisher) = 0;
	};
}