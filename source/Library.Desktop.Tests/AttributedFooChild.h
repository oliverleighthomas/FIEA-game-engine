#pragma once
#include "AttributedFoo.h"

namespace UnitTests {
	class AttributedFooChild final : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedFooChild, AttributedFoo);

	public:
		static const std::size_t arraySize = 5;

		AttributedFooChild();
		AttributedFooChild(const AttributedFooChild&) = default;
		AttributedFooChild(AttributedFooChild&&) = default;
		AttributedFooChild& operator=(const AttributedFooChild&) = default;
		AttributedFooChild& operator=(AttributedFooChild&&) = default;
		~AttributedFooChild() = default;

		gsl::owner<AttributedFooChild*> Clone() const override;
		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		std::string testAttribute = "";

	protected:
		AttributedFooChild(RTTI::IdType typeID) : AttributedFoo(typeID) { };
	};
}


