#include "pch.h"
#include "TypeManager.h"

namespace FieaGameEngine {
	Signature::Signature(std::string name, Datum::DatumTypes type, std::size_t size, std::size_t offset) :
		name{ name }, type{ type }, size{ size }, offset{ offset }
	{
	}

	bool Signature::operator==(const Signature& rhs) {
		return name == rhs.name &&
			   type == rhs.type &&
			   size == rhs.size &&
			   offset == rhs.offset;
	}

	void TypeManager::CreateInstance() {
		assert(instance == nullptr);
		instance = new TypeManager();
	}

	void TypeManager::DeleteInstance() {
		delete(instance);
		instance = nullptr;
	}
}