#pragma once
#include "Datum.h"
#include "HashMap.h"
#include "vector.h"
#include "RTTI.h"

namespace FieaGameEngine {

	struct Signature {
		explicit Signature(std::string name = "", Datum::DatumTypes type = Datum::DatumTypes::Unknown, std::size_t size = 0, std::size_t offset = 0);

		[[nodiscard]] bool operator==(const Signature& rhs);
		[[nodiscard]] bool operator!=(const Signature& rhs);

		std::string name;
		Datum::DatumTypes type;
		std::size_t size;
		std::size_t offset;
	};

	struct Type {
		RTTI::IdType parentTypeId;
		FieaGameEngine::Vector<Signature> signatures;
	};

	class TypeManager final {
	public:
		static TypeManager* Instance();

		static void CreateInstance();
		static void DeleteInstance();

		template <typename T>
		void RegisterType();
		void RemoveType(RTTI::IdType typeID);

		[[nodiscard]] Type* FindType(RTTI::IdType typeID);

		void Clear();

	private:
		inline static TypeManager* instance{ nullptr };
		FieaGameEngine::HashMap<RTTI::IdType, Type> types;
	};
}

#include "TypeManager.inl"