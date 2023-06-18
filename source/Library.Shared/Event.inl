#pragma once
#include "Event.h"

namespace FieaGameEngine {
	template <typename T>
	RTTI_DEFINITIONS(Event<T>);

	template <typename T>
	Event<T>::Event(const T& message) :
		EventPublisher{ subscribers }, message{ message }
	{
	}

	template <typename T>
	Event<T>::Event(T&& message) :
		EventPublisher{ subscribers }, message{ std::move(message) }
	{
	}

	template <typename T>
	void Event<T>::Subscribe(EventSubscriber& subscriber) {
		Event<T>::subscribers.PushBack(&subscriber);
	}

	template <typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber) {
		Event<T>::subscribers.Remove(&subscriber);
	}

	template <typename T>
	void Event<T>::Clear() {
		Event<T>::subscribers.Clear();
		Event<T>::subscribers.ShrinkToFit();
	}

	template<typename T>
	inline const Vector<EventSubscriber*>& Event<T>::Subscribers() {
		return Event<T>::subscribers;
	}

	template <typename T>
	const T& Event<T>::Message() const {
		return message;
	}
}
