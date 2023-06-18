#pragma once
#include "RTTI.h"
#include "Attributed.h"
#include "TypeManager.h"

namespace UnitTests {
	class AttributedFoo : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed);

	public:
		static const std::size_t arraySize = 5;

		AttributedFoo();
		AttributedFoo(const AttributedFoo&) = default;
		AttributedFoo(AttributedFoo&&) = default;
		AttributedFoo& operator=(const AttributedFoo&) = default;
		AttributedFoo& operator=(AttributedFoo&&) = default;
		virtual ~AttributedFoo() = default;

		gsl::owner<AttributedFoo*> Clone() const override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		int externalInt = 0;
		float externalFloat = 0;
		std::string externalString;
		glm::vec4 externalVec;
		glm::mat4 externalMat;

		int externalIntArray[arraySize];
		float externalFloatArray[arraySize];
		std::string externalStringArray[arraySize];
		glm::vec4 externalVecArray[arraySize];
		glm::mat4 externalMatArray[arraySize];

	protected:
		AttributedFoo(RTTI::IdType typeID) : Attributed(typeID) { };
	};


}
