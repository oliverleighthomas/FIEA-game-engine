#pragma once
#include "Vector.h"
#include "Attributed.h"
#include "TypeManager.h"
#include "Factory.h"
#include "GameTime.h"
#include "Action.h"

namespace FieaGameEngine {
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	public:
		/// <summary>
		/// Default constructor for a GameObject. Calls GameObject(const std::string&) with an empty string.
		/// </summary>
		GameObject();
		/// <summary>
		/// Constructor for a GameObject that takes in a name parameter.
		/// </summary>
		/// <param name="name">The name for the GameObject</param>
		GameObject(const std::string& name);
		/// <summary>
		/// Copy-constructor for a GameObject.
		/// </summary>
		/// <param name="rhs">The GameObject to copy from.</param>
		GameObject(const GameObject& rhs) = default;
		/// <summary>
		/// Move-constructor for a GameObject.
		/// </summary>
		/// <param name="rhs">The GameObject to move from.</param>
		GameObject(GameObject&& rhs) noexcept = default;
		/// <summary>
		/// Copy-assignment operator for a GameObject.
		/// </summary>
		/// <param name="rhs">The GameObject to copy from.</param>
		/// <returns>A reference to the newly copied GameObject.</returns>
		GameObject& operator=(const GameObject& rhs) = default;
		/// <summary>
		/// Move-assignment operator for a GameObject.
		/// </summary>
		/// <param name="rhs">The GameObject to move from.</param>
		/// <returns>A reference to the newly-moved GameObject.</returns>
		GameObject& operator=(GameObject&& rhs) noexcept = default;
		/// <summary>
		/// Virtual destructor for a GameObject.
		/// </summary>
		virtual ~GameObject() = default;

		/// <summary>
		/// The update function for GameObject. Updates self and then calls "update" on every child.
		/// </summary>
		/// <param name="time">A GameTime object.</param>
		virtual void Update(const GameTime& time);

		/// <summary>
		/// The signatures for the perscribed attributes of a GameObject.
		/// </summary>
		/// <returns>The vector of perscribed attribute signatures of a GameObject.</returns>
		static Vector<FieaGameEngine::Signature> Signatures();

		/// <summary>
		/// The action datum for the GameObject.
		/// </summary>
		/// <returns>The action datum for the GameObject.</returns>
		Datum& Actions();
		/// <summary>
		/// A const version of the action datum for the GameObject.
		/// </summary>
		/// <returns>A const version of the action datum for the GameObject.</returns>
		const Datum& Actions() const;

		/// <summary>
		/// Create and add a new action to the action datum.
		/// </summary>
		/// <param name="className">The class of the new action.</param>
		/// <param name="instanceName">The name of the new action.</param>
		/// <returns>A pointer to the newly created action.</returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		GameObject* CreateChild(const std::string& className, const std::string& instanceName);

		std::string name;

		glm::vec4 position;
		glm::vec4 rotation;
		glm::vec4 scale;
	};
	CONCRETE_FACTORY(GameObject, FieaGameEngine::Scope);
}

