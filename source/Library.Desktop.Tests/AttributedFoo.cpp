#include "pch.h"
#include "AttributedFoo.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTests {
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	Vector<Signature> AttributedFoo::Signatures() {
		return Vector<Signature>{
			Signature("ExternalInteger", Datum::DatumTypes::Integer, 1, offsetof(AttributedFoo, externalInt)),
				Signature("ExternalFloat", Datum::DatumTypes::Float, 1, offsetof(AttributedFoo, externalFloat)),
				Signature("ExternalString", Datum::DatumTypes::String, 1, offsetof(AttributedFoo, externalString)),
				Signature("ExternalVector", Datum::DatumTypes::Vector, 1, offsetof(AttributedFoo, externalVec)),
				Signature("ExternalMatrix", Datum::DatumTypes::Matrix, 1, offsetof(AttributedFoo, externalMat)),
				Signature("ExternalIntegerArray", Datum::DatumTypes::Integer, arraySize, offsetof(AttributedFoo, externalIntArray)),
				Signature("ExternalFloatArray", Datum::DatumTypes::Float, arraySize, offsetof(AttributedFoo, externalFloatArray)),
				Signature("ExternalStringArray", Datum::DatumTypes::String, arraySize, offsetof(AttributedFoo, externalStringArray)),
				Signature("ExternalVectorArray", Datum::DatumTypes::Vector, arraySize, offsetof(AttributedFoo, externalVecArray)),
				Signature("ExternalMatrixArray", Datum::DatumTypes::Matrix, arraySize, offsetof(AttributedFoo, externalMatArray))
		};
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const {
		return new AttributedFoo(*this);
	}
}