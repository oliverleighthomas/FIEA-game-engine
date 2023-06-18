#pragma once
#include <memory>
#include "GameTime.h"
#include "vector.h"
#include "EventPublisher.h"

namespace FieaGameEngine {
	class EventQueue
	{
	private:
		/// <summary>
		/// An entry for the queue. Contains a pointer to an event and the time at which it expires.
		/// </summary>
		struct QueueEntry {
			std::shared_ptr<EventPublisher> publisher;
			std::chrono::steady_clock::time_point expirationTime;
		};

	public:
		EventQueue() = default;
		EventQueue(const EventQueue& rhs) = default;
		EventQueue(EventQueue&& rhs) noexcept = default;
		EventQueue& operator=(const EventQueue& rhs) = default;
		EventQueue& operator=(EventQueue&& rhs) noexcept = default;
		~EventQueue() = default;

		/// <summary>
		/// Enqueue an event.
		/// </summary>
		/// <param name="publisher">The event to enqueue.</param>
		/// <param name="time">A reference to a GameTime object, so we can get the game's current time.</param>
		/// <param name="delay">The delay until the event should be called.</param>
		void Enqueue(std::shared_ptr<EventPublisher> publisher, const GameTime& time, std::chrono::milliseconds delay = std::chrono::milliseconds(0));

		/// <summary>
		/// Call all events that are ready to expire.
		/// </summary>
		/// <param name="time">A reference to a GameTime object, so we can get the game's current time.</param>
		void Update(const GameTime& time);

		/// <summary>
		/// Clear the queue.
		/// </summary>
		void Clear();

		/// <summary>
		/// Checks if the queue is empty.
		/// </summary>
		/// <returns>True if the queue is empty, false if it is not.</returns>
		bool IsEmpty() const;
		/// <summary>
		/// The size of the queue.
		/// </summary>
		/// <returns>The size of the queue.</returns>
		size_t Size() const;

		/// <summary>
		/// The events currently enqueued.
		/// </summary>
		/// <returns>A vector of events currently enqueued.</returns>
		const Vector<QueueEntry>& Events() const;

	private:
		bool isUpdating{ false };

		Vector<QueueEntry> events;
		Vector<QueueEntry> pendingEvents;
	};
}



