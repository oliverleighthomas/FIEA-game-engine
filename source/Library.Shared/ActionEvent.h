#pragma once
#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine {
	class ActionEvent : public Action {
	RTTI_DECLARATIONS(ActionEvent, Action);

	public:
		/// <summary>
		/// Default constructor for the ActionEvent.
		/// </summary>
		ActionEvent();
		ActionEvent(const ActionEvent& rhs) = default;
		ActionEvent(ActionEvent&& rhs) noexcept = default;
		ActionEvent& operator=(const ActionEvent& rhs) = default;
		ActionEvent& operator=(ActionEvent&& rhs) noexcept = default;
		~ActionEvent() = default;

		/// <summary>
		/// Get the subtype name for the ActionEvent.
		/// </summary>
		/// <returns>The name of the ActionEvent's subtype.</returns>
		const std::string& GetSubType() const;
		/// <summary>
		/// Set the subtype name for the ActionEvent.
		/// </summary>
		/// <param name="newSubType">The name to change the ActionEvent's subtype to.</param>
		void SetSubType(const std::string& newSubType);

		/// <summary>
		/// Get the delay of the ActionEvent.
		/// </summary>
		/// <returns>The delay of the ActionEvent.</returns>
		uint32_t GetDelay() const;
		/// <summary>
		/// Set the delay of the ActionEvent.
		/// </summary>
		/// <param name="newDelay">The delay to change the ActionEvent's delay to.</param>
		void SetDelay(uint32_t newDelay);

		/// <summary>
		/// Update the ActionEvent. Populates an event messager with the ActionEvent's auxilary attributes and then enqueues it.
		/// </summary>
		/// <param name="time">The current game time.</param>
		void Update(const GameTime& time) override;

		/// <summary>
		/// Clone the ActionEvent.
		/// </summary>
		/// <returns>A pointer to the created ActionEvent.</returns>
		ActionEvent* Clone() override;

		/// <summary>
		/// Get the signatures of the ActionEvent.
		/// </summary>
		/// <returns>A vector of signatures associated with the event.</returns>
		static Vector<Signature> Signatures();

	private:
		std::string subType;
		uint32_t delay;
	};
	CONCRETE_FACTORY(ActionEvent, Scope);
}


