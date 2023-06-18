#pragma once
#include "Action.h"
#include "Factory.h"
#include "TypeManager.h"

namespace FieaGameEngine {
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:
		/// <summary>
		/// The default constructor of an action list.
		/// </summary>
		ActionList();
		/// <summary>
		/// Construct an action list with the given name.
		/// </summary>
		/// <param name="name">The name to give the constructed action list.</param>
		explicit ActionList(const std::string& name);
		ActionList(const ActionList& rhs) = default;
		ActionList(ActionList&& rhs) = default;
		ActionList& operator=(const ActionList& rhs) = default;
		ActionList& operator=(ActionList&& rhs) = default;
		virtual ~ActionList() = default;

		/// <summary>
		/// Go through the list of actions and run "update" on all of them.
		/// </summary>
		virtual void Update(const GameTime& time) override;

		/// <summary>
		/// The datum pointing to the list of actions.
		/// </summary>
		/// <returns>The datum pointing to the list of actions.</returns>
		Datum* Actions() const;

		/// <summary>
		/// The signatures of attributes for the ActionList class.
		/// </summary>
		/// <returns>A vector of signatures.</returns>
		static Vector<Signature> Signatures();

	protected:
		ActionList(RTTI::IdType idType);
		ActionList(RTTI::IdType idType, const std::string& name);

		Datum* actions{ nullptr };
	};
	CONCRETE_FACTORY(ActionList, Scope);
}


