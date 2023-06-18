#include "pch.h"
#include "EventTestSubscribers.h"

namespace UnitTests {
	void SubscriberFoo::Notify(const FieaGameEngine::EventPublisher& publisher) {
		auto eventPointer = publisher.As<FieaGameEngine::Event<Foo>>();
		assert(eventPointer != nullptr);
		foo = eventPointer->Message();
		++count;
	}


	void EnqueueDuringUpdate::Notify(const FieaGameEngine::EventPublisher&) {
		if (queue != nullptr)
		{
			std::shared_ptr<FieaGameEngine::Event<Foo>> toEnqueue = std::make_shared<FieaGameEngine::Event<Foo>>(Foo(5));
			queue->Enqueue(toEnqueue, FieaGameEngine::GameTime(), std::chrono::milliseconds(5));
		}
		count++;
	}
}

