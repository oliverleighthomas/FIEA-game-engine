#pragma once
#include "Reaction.h"
#include "Factory.h"
#include "vector.h"
#include "TypeManager.h"

namespace FieaGameEngine {
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

	public:
		/// <summary>
		/// Default ReactionAttributed constructor.
		/// </summary>
		ReactionAttributed();
		/// <summary>
		/// ReactionAttributed constructor with a parameter for a subtype name.
		/// </summary>
		/// <param name="subType">The name of the ReactionAttributed's subtype.</param>
		explicit ReactionAttributed(const std::string subType);
		/// <summary>
		/// ReactionAttributed constructor with a parameter for a subtype name and the name of the reaction.
		/// </summary>
		/// <param name="subType">The name of the ReactionAttributed's subtype.</param>
		/// <param name="name">The name of the ReactionAttributed.</param>
		explicit ReactionAttributed(const std::string subType, const std::string name);
		ReactionAttributed(const ReactionAttributed& rhs) = default;
		ReactionAttributed(ReactionAttributed&& rhs) noexcept = default;
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = default;
		ReactionAttributed& operator=(ReactionAttributed&& rhs) noexcept = default;
		~ReactionAttributed();

		/// <summary>
		/// Notify the ReactionAttributed that it is time to update.
		/// </summary>
		/// <param name="publisher">The event publisher issuing the update call.</param>
		void Notify(const EventPublisher& publisher) override;
		/// <summary>
		/// Update the ActionList.
		/// </summary>
		/// <param name="time">The current game time.</param>
		void Update(const GameTime& time) override;
		
		/// <summary>
		/// Get the name of the reaction's subtype.
		/// </summary>
		/// <returns>The name of the reaction's subtype.</returns>
		const std::string& GetSubType() const;
		/// <summary>
		/// Set the name of the reaction's subtype.
		/// </summary>
		/// <param name="newSubType">The name to change the reaction subtype name to.</param>
		void SetSubType(const std::string& newSubType);

		/// <summary>
		/// Clone the ReactionAttributed.
		/// </summary>
		/// <returns>A pointer to the newly created ReactionAttributed.</returns>
		ReactionAttributed* Clone() override;

		/// <summary>
		/// Get the signatures of the reaction attributed.
		/// </summary>
		/// <returns>A vector of signatures associated with the reaction.</returns>
		static Vector<Signature> Signatures();

		/// <summary>
		/// Create and add a new action to the action datum.
		/// </summary>
		/// <param name="className">The class of the new action.</param>
		/// <param name="instanceName">The name of the new action.</param>
		/// <returns>A pointer to the newly created action.</returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

	private:
		std::string subType;
	};

	CONCRETE_FACTORY(ReactionAttributed, Scope);
}

