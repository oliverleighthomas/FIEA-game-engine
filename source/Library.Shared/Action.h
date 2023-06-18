#pragma once
#include "Attributed.h"
#include "TypeManager.h"

namespace FieaGameEngine {
	class GameTime;

	class Action : public Attributed {
		RTTI_DECLARATIONS(Action, Attributed);

	public:
		Action(const Action& rhs) = default;
		Action(Action&& rhs) = default;
		Action& operator=(const Action& rhs) = default;
		Action& operator=(Action&& rhs) = default;
		virtual ~Action() = default;

		/// <summary>
		/// Update the action.
		/// </summary>
		virtual void Update(const GameTime& time) = 0;

		/// <summary>
		/// Get the action's name.
		/// </summary>
		/// <returns>The name of the action.</returns>
		const std::string& Name() const;
		/// <summary>
		/// Change the action's name.
		/// </summary>
		/// <param name="newName">The name to change the action's current name to.</param>
		void SetName(const std::string& newName);

		/// <summary>
		/// The signatures of attributes for the Action class.
		/// </summary>
		/// <returns>A vector of signatures.</returns>
		static Vector<Signature> Signatures();

	protected:
		/// <summary>
		/// A constructor for an action that takes in an RTTI type.
		/// </summary>
		/// <param name="idType">The type of the action.</param>
		Action(RTTI::IdType idType);
		/// <summary>
		/// A constructor for an action that takes in an RTTI type and a name.
		/// </summary>
		/// <param name="idType">The type of the action.</param>
		/// <param name="name">The name of the action.</param>
		Action(RTTI::IdType idType, const std::string& name);

		std::string name;
	};
}