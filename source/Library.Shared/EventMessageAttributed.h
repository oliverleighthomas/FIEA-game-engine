#pragma once
#include "Attributed.h"
#include "vector.h"
#include "TypeManager.h"

namespace FieaGameEngine {
	class GameState;

	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

	public:
		/// <summary>
		/// Default constructor for EventMessageAttributed.
		/// </summary>
		EventMessageAttributed();
		/// <summary>
		/// A constructor for EventMessageAttributed that takes in a subtype name.
		/// </summary>
		/// <param name="subType">The name of the EventMessageAttributed.</param>
		EventMessageAttributed(const std::string& subType);
		EventMessageAttributed(const EventMessageAttributed& rhs) = default;
		EventMessageAttributed(EventMessageAttributed&& rhs) noexcept = default;
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs) noexcept = default;
		~EventMessageAttributed() = default;

		/// <summary>
		/// Get the subtype name for the EventMessageAttributed.
		/// </summary>
		/// <returns>The name of the EventMessageAttributed's subtype.</returns>
		const std::string& GetSubType() const;
		/// <summary>
		/// Set the subtype name for the EventMessageAttributed.
		/// </summary>
		/// <param name="newSubType">The name to change the EventMessageAttributed's subtype to.</param>
		void SetSubType(const std::string& newSubType);

		/// <summary>
		/// Clone the EventMessageAttributed.
		/// </summary>
		/// <returns>A pointer to the created EventMessageAttributed.</returns>
		virtual EventMessageAttributed* Clone() override;

		/// <summary>
		/// Get the signatures of the EventMessageAttributed.
		/// </summary>
		/// <returns>A vector of signatures associated with the event.</returns>
		static Vector<Signature> Signatures();

	private:
		std::string subType;
	};
}

