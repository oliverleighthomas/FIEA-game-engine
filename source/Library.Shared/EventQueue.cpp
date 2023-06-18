#include "pch.h"
#include "EventQueue.h"
#include <algorithm>
#include "Event.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine {
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> publisher, const GameTime& time, std::chrono::milliseconds delay) {
		auto expirationTime = time.CurrentTime() + delay;

		if (isUpdating) pendingEvents.PushBack({ std::move(publisher), expirationTime});
		else events.PushBack({ std::move(publisher), expirationTime });
	}

	void EventQueue::Update(const GameTime& time) {
		isUpdating = true;
		auto currTime = time.CurrentTime();

		auto expiringEventsStart = std::partition(events.begin(), events.end(), [&currTime](const QueueEntry& entry) {
			return entry.expirationTime > currTime;
			});

		for (auto iter = expiringEventsStart; iter != events.end(); ++iter) {
			(*iter).publisher->Deliver();
		}
		events.RemoveRange(expiringEventsStart, events.end());

		isUpdating = false;

		if (!pendingEvents.IsEmpty()) {
			for (auto& pendingEvent : pendingEvents) {
				events.PushBack(std::move(pendingEvent));
			}
			pendingEvents.Clear();
		}
	}

	void EventQueue::Clear() {
		events.Clear();
		events.ShrinkToFit();
	}

	bool EventQueue::IsEmpty() const {
		return events.IsEmpty();
	}

	size_t EventQueue::Size() const {
		return events.Size();
	}

	const Vector<EventQueue::QueueEntry>& EventQueue::Events() const {
		return events;
	}
}