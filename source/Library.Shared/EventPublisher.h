#pragma once
#include "RTTI.h"
#include "vector.h"

namespace FieaGameEngine {
	class EventSubscriber;

	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);

	public:
		EventPublisher(const EventPublisher& rhs) = default;
		EventPublisher(EventPublisher&& rhs) = default;
		EventPublisher& operator=(const EventPublisher& rhs) = default;
		EventPublisher& operator=(EventPublisher&& rhs) = default;
		virtual ~EventPublisher() = default;

		/// <summary>
		/// Deliver the event's payload to the event's subscribers.
		/// </summary>
		void Deliver() const;

	protected:
		/// <summary>
		/// The protected constructor for the event publisher. Sets up its reference to derived event's subscribers.
		/// </summary>
		/// <param name="subscribers">The vector of subscribers to a derived event.</param>
		EventPublisher(Vector<EventSubscriber*>& subscribers);

	private:
		Vector<EventSubscriber*>* subscribers;
	};
}

