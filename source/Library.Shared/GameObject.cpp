#include "pch.h"
#include "GameObject.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(GameObject);

	GameObject::GameObject() : GameObject("") 
	{
	}

	GameObject::GameObject(const std::string& name) :
		Attributed(GameObject::TypeIdClass()), name{ name }
	{
	}

	void GameObject::Update(const GameTime& time) {
		Datum* children = Find("Children");
		for (unsigned int i = 0; i < children->Size(); ++i) {
			auto child = children->GetScope(i)->As<GameObject>();

			assert(child != nullptr);

			child->Update(time);
		}

		Datum* actions = Find("Actions");
		for (unsigned int i = 0; i < actions->Size(); ++i) {
			auto action = actions->GetScope(i)->As<Action>();

			assert(action != nullptr);

			action->Update(time);
		}
	}

	Vector<FieaGameEngine::Signature> GameObject::Signatures() {
		return Vector<Signature> {
			{ "Name", Datum::DatumTypes::String, 1, offsetof(GameObject, name) },
			{ "Position", Datum::DatumTypes::Vector, 1, offsetof(GameObject, position) },
			{ "Rotation", Datum::DatumTypes::Vector, 1, offsetof(GameObject, rotation) },
			{ "Scale", Datum::DatumTypes::Vector, 1, offsetof(GameObject, scale) },
			{ "Children", Datum::DatumTypes::Table, 0, 0 },
			{ "Actions", Datum::DatumTypes::Table, 0, 0 }
		};
	}

	Datum& GameObject::Actions() {
		return *Find("Actions");
	}

	const Datum& GameObject::Actions() const {
		return *Find("Actions");
	}

	Action* GameObject::CreateAction(const std::string& className, const std::string& instanceName) {
		auto action = Factory<Scope>::Create(className)->As<Action>();
		assert(action != nullptr);
		action->SetName(instanceName);
		Adopt(*action, "Actions");
		return action;
	}

	GameObject* GameObject::CreateChild(const std::string& className, const std::string& instanceName) {
		auto child = Factory<Scope>::Create(className)->As<GameObject>();
		assert(child != nullptr);
		child->name = instanceName;
		Adopt(*child, "Children");
		return child;
	}

}