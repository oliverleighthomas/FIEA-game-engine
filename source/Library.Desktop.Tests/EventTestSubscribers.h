#pragma once
#include "EventSubscriber.h"
#include "EventQueue.h"
#include "Event.h"
#include "Foo.h"

namespace UnitTests {
	struct SubscriberFoo : FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& publisher) override;

		Foo foo{ 0 };
		int count{ 0 };
	};

	struct EnqueueDuringUpdate : FieaGameEngine::EventSubscriber {
		void Notify(const FieaGameEngine::EventPublisher& publisher) override;

		FieaGameEngine::EventQueue* queue{ nullptr };
		int count{ 0 };
	};
}

