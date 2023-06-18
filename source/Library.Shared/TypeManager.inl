#pragma once
#include "TypeManager.h"

namespace FieaGameEngine {
	inline bool Signature::operator!=(const Signature& rhs) {
		return !(*this == rhs);
	}

	inline TypeManager* TypeManager::Instance() {
		return instance;
	}

	inline Type* TypeManager::FindType(RTTI::IdType typeID) {
		auto found = types.Find(typeID);
		return found == types.end() ? nullptr : &found->second;
	}

	template <typename T>
	inline void TypeManager::RegisterType() {
#ifdef USE_EXCEPTIONS
		if (FindType(T::TypeIdClass())) throw std::runtime_error("Type already created.");
		if (T::Parent::TypeIdClass() != Attributed::TypeIdClass() && !FindType(T::Parent::TypeIdClass())) throw std::runtime_error("Parent not registered.");
#endif

		Type type = { T::Parent::TypeIdClass(), T::Signatures() };

		types.Insert(T::TypeIdClass(), type);
	}

	inline void TypeManager::RemoveType(RTTI::IdType typeID) {
		types.Remove(typeID);
	}

	inline void TypeManager::Clear() {
		types.Clear();
	}
}