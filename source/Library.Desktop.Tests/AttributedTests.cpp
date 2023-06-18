#include "pch.h"
#include "CppUnitTest.h"
#include "Attributed.h"
#include "AttributedFoo.h"
#include "AttributedFooChild.h"
#include "ToStringSpecialization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework {
	template<>
	inline std::wstring ToString<UnitTests::AttributedFoo>(const UnitTests::AttributedFoo& t) {
		std::string s = t.ToString();
		std::wstring w(s.begin(), s.end());
		return w;
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(AttributedTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			TypeManager::CreateInstance();
			TypeManager::Instance()->RegisterType<AttributedFoo>();
			TypeManager::Instance()->RegisterType<AttributedFooChild>();
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeManager::Instance()->Clear();
			TypeManager::DeleteInstance();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Constructor) {
			AttributedFoo foo;

			Assert::IsTrue(foo["this"] == &foo);
			Assert::AreEqual(0, foo.externalInt);
			Assert::AreEqual(0.f, foo.externalFloat);
			Assert::AreEqual(std::string(), foo.externalString);
			Assert::AreEqual(glm::to_string(glm::vec4()), glm::to_string(foo.externalVec));
			Assert::AreEqual(glm::to_string(glm::mat4()), glm::to_string(foo.externalMat));

			foo.externalInt = 3;
			Assert::AreEqual(3, foo.externalInt);
			Assert::AreEqual(3, foo["ExternalInteger"].GetInt());
			Assert::AreEqual(foo.externalInt, foo["ExternalInteger"].GetInt());

			for (int32_t i = 0; i < 5; ++i) {
				foo.externalIntArray[i] = i;
			}

			for (int32_t i = 0; i < 5; ++i) {
				Assert::AreEqual(i, foo.externalIntArray[i]);
				Assert::AreEqual(i, foo["ExternalIntegerArray"].GetInt(i));
			}
		}

		TEST_METHOD(CopySemantics) {
			{
				AttributedFoo foo;
				foo.externalFloat = 3.f;
				foo.externalString = "Hello";
				for (int32_t i = 0; i < 5; ++i) {
					foo.externalIntArray[i] = i;
				}

				AttributedFoo fooCopy(foo);

				Assert::AreEqual(foo, fooCopy);
			}
			{
				AttributedFoo foo;
				foo.externalFloat = 3.f;
				foo.externalString = "Hello";
				for (int32_t i = 0; i < 5; ++i) {
					foo.externalIntArray[i] = i;
				}

				AttributedFoo fooCopy;
				fooCopy = foo;

				Assert::AreEqual(foo, fooCopy);
			}
		}

		TEST_METHOD(MoveSemantics) {
			{
				AttributedFoo foo;
				foo.externalFloat = 3.f;
				foo.externalString = "Hello";
				for (int32_t i = 0; i < 5; ++i) {
					foo.externalIntArray[i] = i;
				}

				AttributedFoo fooCopy(foo);
				AttributedFoo fooMoved(std::move(foo));

				Assert::AreEqual(fooCopy, fooMoved);
			}
			{
				AttributedFoo foo;
				foo.externalFloat = 3.f;
				foo.externalString = "Hello";
				for (int32_t i = 0; i < 5; ++i) {
					foo.externalIntArray[i] = i;
				}

				AttributedFoo fooCopy(foo);
				AttributedFoo fooMoved;
				fooMoved = std::move(foo);

				Assert::AreEqual(fooCopy, fooMoved);
			}
		}

		TEST_METHOD(Clone) {
			AttributedFoo foo;
			foo.externalFloat = 3.f;
			foo.externalString = "Hello";
			for (int32_t i = 0; i < 5; ++i) {
				foo.externalIntArray[i] = i;
			}

			AttributedFoo* clone = foo.Clone();

			Assert::AreEqual(foo, *clone);

			delete(clone);
		}

		TEST_METHOD(Equality) {
			AttributedFoo one;
			one.externalFloat = 3.f;
			one.externalString = "Hello";
			for (int32_t i = 0; i < 5; ++i) {
				one.externalIntArray[i] = i;
			}

			AttributedFoo two;
			two.externalFloat = 5.f;
			two.externalString = "Goodbye";
			for (int32_t i = 0; i < 5; ++i) {
				two.externalIntArray[i] = i * 3;
			}

			AttributedFooChild three;

			Assert::IsTrue(one == one);
			Assert::IsFalse(one == two);
			Assert::IsFalse(one == three);

			Assert::IsFalse(one != one);
			Assert::IsTrue(one != two);
			Assert::IsTrue(one != three);
		}

		TEST_METHOD(IsPerscribedAuxiliaryAttribute) {
			AttributedFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"));
			
			Assert::IsFalse(foo.IsAttribute("NotAnAttribute"));
			Assert::IsFalse(foo.IsPrescribedAttribute("NotAnAttribute"));

			foo.AppendAuxiliaryAttribute("NewAttribute");

			Assert::IsTrue(foo.IsAuxiliaryAttribute("NewAttribute"));
		}

		TEST_METHOD(AppendAuxiliaryAttribute) {
			AttributedFoo foo;

			Assert::IsNull(foo.Find("NewAttribute"));

			Datum& datum = foo.AppendAuxiliaryAttribute("NewAttribute");

			Assert::IsNotNull(foo.Find("NewAttribute"));
			Assert::IsTrue(&datum == foo.Find("NewAttribute"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("NewAttribute"));

			Assert::ExpectException<std::runtime_error>([&foo] {
				foo.AppendAuxiliaryAttribute("ExternalInteger");
				});
		}

		TEST_METHOD(PerscribedAuxiliaryAttributes) {
			AttributedFoo foo;

			Assert::AreEqual(size_t(11), foo.Attributes().Size());
			Assert::AreEqual("this"s, foo.Attributes().Front()->first);

			Assert::AreEqual(size_t(11), foo.PrescribedAttributes().Size());
			Assert::AreEqual("this"s, foo.PrescribedAttributes().Front()->first);

			Assert::AreEqual(size_t(0), foo.AuxiliaryAttributes().Size());
			foo.AppendAuxiliaryAttribute("NewAttribute");
			Assert::AreEqual(size_t(1), foo.AuxiliaryAttributes().Size());
			Assert::AreEqual("NewAttribute"s, foo.AuxiliaryAttributes().Front()->first);
		}

		TEST_METHOD(Equals) {
			AttributedFoo one, two;
			Scope scope;

			Assert::IsTrue(one.Equals(&two));
			Assert::IsFalse(one.Equals(&scope));
			Assert::IsFalse(one.Equals(nullptr));
		}

		TEST_METHOD(ToString) {
			AttributedFoo foo;

			std::string s = "Attributed(\"this\"{},\"ExternalInteger\"{0},\"ExternalFloat\"{0.000000},\"ExternalString\"{},\"ExternalVector\"{vec4(0.000000, 0.000000, 0.000000, 0.000000)},\"ExternalMatrix\"{mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000))},\"ExternalIntegerArray\"{0,0,0,0,0},\"ExternalFloatArray\"{0.000000,0.000000,0.000000,0.000000,0.000000},\"ExternalStringArray\"{,,,,},\"ExternalVectorArray\"{vec4(0.000000, 0.000000, 0.000000, 0.000000),vec4(0.000000, 0.000000, 0.000000, 0.000000),vec4(0.000000, 0.000000, 0.000000, 0.000000),vec4(0.000000, 0.000000, 0.000000, 0.000000),vec4(0.000000, 0.000000, 0.000000, 0.000000)},\"ExternalMatrixArray\"{mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000)),mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000)),mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000)),mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000)),mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000))})"s;
			Assert::AreEqual(s, foo.ToString());

			foo.externalFloat = 3.f;
			foo.externalString = "Hello";
			for (int32_t i = 0; i < 5; ++i) {
				foo.externalIntArray[i] = i;
			}

			s = "Attributed(\"this\"{},\"ExternalInteger\"{0},\"ExternalFloat\"{3.000000},\"ExternalString\"{Hello},\"ExternalVector\"{vec4(0.000000, 0.000000, 0.000000, 0.000000)},\"ExternalMatrix\"{mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000))},\"ExternalIntegerArray\"{0,1,2,3,4},\"ExternalFloatArray\"{0.000000,0.000000,0.000000,0.000000,0.000000},\"ExternalStringArray\"{,,,,},\"ExternalVectorArray\"{vec4(0.000000, 0.000000, 0.000000, 0.000000),vec4(0.000000, 0.000000, 0.000000, 0.000000),vec4(0.000000, 0.000000, 0.000000, 0.000000),vec4(0.000000, 0.000000, 0.000000, 0.000000),vec4(0.000000, 0.000000, 0.000000, 0.000000)},\"ExternalMatrixArray\"{mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000)),mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000)),mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000)),mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000)),mat4x4((0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000))})"s;
			Assert::AreEqual(s, foo.ToString());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}