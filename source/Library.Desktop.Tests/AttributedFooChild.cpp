#include "pch.h"
#include "AttributedFooChild.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTests {
	RTTI_DEFINITIONS(AttributedFooChild);

	AttributedFooChild::AttributedFooChild() :
		AttributedFoo(AttributedFooChild::TypeIdClass())
	{
	}

	Vector<Signature> AttributedFooChild::Signatures() {
		return Vector<Signature>{
				Signature("TestAttribute", Datum::DatumTypes::String, 1, offsetof(AttributedFooChild, testAttribute))
		};
	}

	gsl::owner<AttributedFooChild*> AttributedFooChild::Clone() const {
		return new AttributedFooChild(*this);
	}

	bool AttributedFooChild::Equals(const RTTI* rhs) const {
		if (rhs == nullptr) return false;

		const AttributedFooChild* toCompare = rhs->As<AttributedFooChild>();
		return (toCompare != nullptr ? *this == *toCompare : false);
	}

	std::string AttributedFooChild::ToString() const {
		return "AttributedFooChild("s + AttributedFoo::ToString() + ")";
	}
}