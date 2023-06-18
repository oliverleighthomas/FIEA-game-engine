#pragma once
#include "ActionList.h"
#include "EventSubscriber.h"

namespace FieaGameEngine {
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);
	public:
		Reaction(const Reaction& rhs) = default;
		Reaction(Reaction&& rhs) noexcept = default;
		Reaction& operator=(const Reaction& rhs) = default;
		Reaction& operator=(Reaction&& rhs) noexcept = default;
		virtual ~Reaction() = default;

		/// <summary>
		/// Notify the reaction that it is time to update.
		/// </summary>
		/// <param name="publisher">The event publisher to send updates from.</param>
		virtual void Notify(const EventPublisher& publisher) = 0;
		/// <summary>
		/// Update the reaction.
		/// </summary>
		/// <param name="time">The current game time.</param>
		virtual void Update(const GameTime& time) = 0;

	protected:
		Reaction(RTTI::IdType idType);
		Reaction(RTTI::IdType idType, const std::string& name);
	};
}