#pragma once
#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine {
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);

	public:
		/// <summary>
		/// The default constructor for an increment action.
		/// </summary>
		ActionIncrement();
		ActionIncrement(const ActionIncrement& rhs) = default;
		ActionIncrement(ActionIncrement&& rhs) = default;
		ActionIncrement& operator=(const ActionIncrement& rhs) = default;
		ActionIncrement& operator=(ActionIncrement&& rhs) = default;
		~ActionIncrement() = default;

		/// <summary>
		/// The name of the object to increment.
		/// </summary>
		/// <returns>The name of the object to increment.</returns>
		std::string TargetName() const;
		/// <summary>
		/// The amount to increment the target by.
		/// </summary>
		/// <returns>The amount to increment the target by.</returns>
		int Step() const;

		/// <summary>
		/// Increment the target.
		/// </summary>
		virtual void Update(const GameTime&) override;
		/// <summary>
		/// The signatures of attributes for the ActionIncrement class.
		/// </summary>
		/// <returns>A vector of signatures.</returns>
		static Vector<FieaGameEngine::Signature> Signatures();

	private:
		std::string targetName{ "" };
		int step{ 0 };
	};
	CONCRETE_FACTORY(ActionIncrement, Scope);
}


