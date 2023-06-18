#pragma once
#include "EventPublisher.h"

namespace FieaGameEngine {
	template <typename T>
	class Event final : public EventPublisher {
		RTTI_DECLARATIONS(Event, EventPublisher);

	public:
		/// <summary>
		/// Constructor for an event. Takes a message by reference.
		/// </summary>
		/// <param name="message">The payload to deliver upon calling the event.</param>
		Event(const T& message);
		/// <summary>
		/// Constructor for an event. Moves the message into the event.
		/// </summary>
		/// <param name="message">The payload to deliver upon calling the event.</param>
		Event(T&& message);
		Event(const Event& rhs) = default;
		Event(Event&& rhs) noexcept = default;
		Event& operator=(const Event& rhs) = default;
		Event& operator=(Event&& rhs) noexcept = default;
		~Event() = default;

		/// <summary>
		/// Subscribe the provided object to the event.
		/// </summary>
		/// <param name="subscriber">The object to subscribe to the event.</param>
		static void Subscribe(EventSubscriber& subscriber);
		/// <summary>
		/// Unsubscribe the provided object from the event.
		/// </summary>
		/// <param name="subscriber">The object to unsubscribe from the event.</param>
		static void Unsubscribe(EventSubscriber& subscriber);
		/// <summary>
		/// Clear the event of all subscribers.
		/// </summary>
		static void Clear();

		/// <summary>
		/// The subscribers listening to the event.
		/// </summary>
		/// <returns>A vector of all subscribers to an event.</returns>
		static const Vector<EventSubscriber*>& Subscribers();
		/// <summary>
		/// The payload of an event.
		/// </summary>
		/// <returns>The payload of an event.</returns>
		const T& Message() const;

	private:
		inline static Vector<EventSubscriber*> subscribers;
		T message;
	};
}

#include "Event.inl"