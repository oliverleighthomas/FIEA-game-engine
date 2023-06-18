#include "pch.h"
#include "CppUnitTest.h"
#include "TestRTTI.h"
#include "Foo.h"
#include "Bar.h"
#include "Datum.h"
#include "Scope.h"
#include "ToStringSpecialization.h"
#pragma warning(push)
#pragma warning(disable:4201)
#include "glm/gtx/string_cast.hpp"
#pragma warning(pop)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework {
	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(const FieaGameEngine::Datum& t) {
		RETURN_WIDE_STRING(&t);
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Datum::DatumTypes>(const FieaGameEngine::Datum::DatumTypes& t) {
		switch (t) {
		case(FieaGameEngine::Datum::DatumTypes::Integer):
			return L"Integer"s;
		case(FieaGameEngine::Datum::DatumTypes::Float):
			return L"Float"s;
		case(FieaGameEngine::Datum::DatumTypes::String):
			return L"String"s;
		case(FieaGameEngine::Datum::DatumTypes::Vector):
			return L"Vector"s;
		case(FieaGameEngine::Datum::DatumTypes::Matrix):
			return L"Matrix"s;
		case(FieaGameEngine::Datum::DatumTypes::Pointer):
			return L"Pointer"s;
		case(FieaGameEngine::Datum::DatumTypes::Table):
			return L"Table"s;
		default:
			return L"Unknown"s;
		}
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t) {
		std::string s = glm::to_string(t);
		return wstring(s.begin(), s.end());
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t) {
		std::string s = glm::to_string(t);
		return wstring(s.begin(), s.end());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(const FieaGameEngine::Scope& t) {
		std::string s = t.ToString();
		return wstring(s.begin(), s.end());
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(DatumTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
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

		TEST_METHOD(RTTITest) {
			TestRTTI one, two;

			Assert::IsFalse(one.Equals(&two));
			Assert::IsTrue(one.Equals(&one));
			Assert::AreEqual("RTTI"s, one.ToString());
		}

		TEST_METHOD(Constructor) {
			{
				Datum datum;
				Assert::AreEqual(Datum::DatumTypes::Unknown, datum.Type());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());

				Assert::ExpectException<std::runtime_error>([&datum] {
					datum.Reserve(5);
					});
			}
			{
				Datum datum(Datum::DatumTypes::Integer);
				Assert::AreEqual(Datum::DatumTypes::Integer, datum.Type());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
			}
			{
				Datum datum(Datum::DatumTypes::String, 5);
				Assert::AreEqual(Datum::DatumTypes::String, datum.Type());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
			}
		}

		TEST_METHOD(CopySemantics) {
			{
				{
					Datum datum = { 1, 2, 3 };

					Datum copy(datum);

					Assert::AreEqual(datum, copy);
				}
				{
					Datum datum = { 1, 2, 3 };

					Datum copy;
					copy = datum;

					Assert::AreEqual(datum, copy);
				}
			}
			{
				{
					Datum datum = { "Test", "One", "Two" };

					Datum copy(datum);

					Assert::AreEqual(datum, copy);
				}
				{
					Datum datum = { "Test", "One", "Two" };

					Datum copy;
					copy = datum;

					Assert::AreEqual(datum, copy);
				}
			}
			{
				{
					Datum datum;
					int32_t arr[3] = { 1, 2, 3 };
					datum.SetStorage(arr, 3);

					Datum copy(datum);

					Assert::AreEqual(datum, copy);
				}
				{
					Datum datum;
					int32_t arr[3] = { 1, 2, 3 };
					datum.SetStorage(arr, 3);

					Datum copy;
					copy = datum;

					Assert::AreEqual(datum, copy);
				}
			}
			
		}

		TEST_METHOD(MoveSemantics) {
			{
				{
					Datum datum = { 1, 2, 3 };

					Datum copy(datum);
					Datum move(std::move(copy));

					Assert::AreEqual(datum, move);
				}
				{
					Datum datum = { 1, 2, 3 };

					Datum copy(datum);
					Datum move;
					move = std::move(copy);

					Assert::AreEqual(datum, move);
				}
			}
			{
				{
					Datum datum = { "Test", "One", "Two" };

					Datum copy(datum);
					Datum move(std::move(copy));

					Assert::AreEqual(datum, move);
				}
				{
					Datum datum = { "Test", "One", "Two" };

					Datum copy(datum);
					Datum move;
					move = std::move(copy);

					Assert::AreEqual(datum, move);
				}
			}
			{
				{
					Datum datum;
					int32_t arr[3] = { 1, 2, 3 };
					datum.SetStorage(arr, 3);

					Datum copy(datum);
					Datum move(std::move(copy));

					Assert::AreEqual(datum, move);
				}
				{
					Datum datum;
					int32_t arr[3] = { 1, 2, 3 };
					datum.SetStorage(arr, 3);

					Datum copy(datum);
					Datum move;
					move = std::move(copy);

					Assert::AreEqual(datum, move);
				}
			}

		}

		TEST_METHOD(InitializerLists) {
			{
				{
					Datum datum{ 1, 2, 3 };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Integer, datum.Type());
					Assert::AreEqual(1, datum.FrontInt());
					Assert::AreEqual(3, datum.BackInt());
				}
				{
					Datum datum;
					datum = { 1, 2, 3 };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Integer, datum.Type());
					Assert::AreEqual(1, datum.FrontInt());
					Assert::AreEqual(3, datum.BackInt());
				}
			}
			{
				{
					Datum datum{ 1.f, 2.f, 3.f };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Float, datum.Type());
					Assert::AreEqual(1.f, datum.FrontFloat());
					Assert::AreEqual(3.f, datum.BackFloat());
				}
				{
					Datum datum;
					datum = { 1.f, 2.f, 3.f };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Float, datum.Type());
					Assert::AreEqual(1.f, datum.FrontFloat());
					Assert::AreEqual(3.f, datum.BackFloat());
				}
			}
			{
				{
					Datum datum{ "A", "B", "C"};

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::String, datum.Type());
					Assert::AreEqual("A"s, datum.FrontString());
					Assert::AreEqual("C"s, datum.BackString());
				}
				{
					Datum datum;
					datum = { "A", "B", "C" };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::String, datum.Type());
					Assert::AreEqual("A"s, datum.FrontString());
					Assert::AreEqual("C"s, datum.BackString());
				}
			}
			{
				{
					Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Vector, datum.Type());
					Assert::AreEqual(glm::vec4(0), datum.FrontVector());
					Assert::AreEqual(glm::vec4(2), datum.BackVector());
				}
				{
					Datum datum;
					datum = { glm::vec4(0), glm::vec4(1), glm::vec4(2) };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Vector, datum.Type());
					Assert::AreEqual(glm::vec4(0), datum.FrontVector());
					Assert::AreEqual(glm::vec4(2), datum.BackVector());
				}
			}
			{
				{
					Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Matrix, datum.Type());
					Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
					Assert::AreEqual(glm::mat4(2), datum.BackMatrix());
				}
				{
					Datum datum;
					datum = { glm::mat4(0), glm::mat4(1), glm::mat4(2) };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Matrix, datum.Type());
					Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
					Assert::AreEqual(glm::mat4(2), datum.BackMatrix());
				}
			}
			{
				{
					Foo a{ 0 };
					Foo b{ 1 };
					Foo c{ 2 };
					Datum datum{ &a, &b, &c };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Pointer, datum.Type());
					Assert::AreEqual(a, *datum.FrontPointer()->As<Foo>());
					Assert::AreEqual(c, *datum.BackPointer()->As<Foo>());
				}
				{
					Foo a{ 0 };
					Foo b{ 1 };
					Foo c{ 2 };
					Datum datum;
					datum = { &a, & b, & c };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Pointer, datum.Type());
					Assert::AreEqual(a, *datum.FrontPointer()->As<Foo>());
					Assert::AreEqual(c, *datum.BackPointer()->As<Foo>());
				}
			}
			{
				{
					Scope a{ 5 };
					Scope b{ 10 };
					Scope c{ 20 };
					Datum datum{ &a, &b, &c };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Table, datum.Type());
					Assert::AreEqual(a, *datum.FrontScope());
					Assert::AreEqual(b, *datum.BackScope());
				}
				{
					Scope a{ 5 };
					Scope b{ 10 };
					Scope c{ 20 };
					Datum datum;
					datum = { &a, & b, & c };

					Assert::AreEqual(size_t(3), datum.Size());
					Assert::AreEqual(Datum::DatumTypes::Table, datum.Type());
					Assert::AreEqual(a, *datum.FrontScope());
					Assert::AreEqual(b, *datum.BackScope());
				}
			}
		}

		TEST_METHOD(ScalarInitializer) {
			{
				Datum datum;
				datum = 1;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Integer, datum.Type());
				Assert::AreEqual(1, datum.FrontInt());
				Assert::AreEqual(datum.FrontInt(), datum.BackInt());
			}
			{
				Datum datum;
				datum = 1.f;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Float, datum.Type());
				Assert::AreEqual(1.f, datum.FrontFloat());
				Assert::AreEqual(datum.FrontFloat(), datum.BackFloat());
			}
			{
				Datum datum;
				datum = "A";
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::String, datum.Type());
				Assert::AreEqual("A"s, datum.FrontString());
				Assert::AreEqual(datum.FrontString(), datum.BackString());
			}
			{
				Datum datum;
				datum = glm::vec4(0);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Vector, datum.Type());
				Assert::AreEqual(glm::vec4(0), datum.FrontVector());
				Assert::AreEqual(datum.FrontVector(), datum.BackVector());
			}
			{
				Datum datum;
				datum = glm::mat4(0);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
				Assert::AreEqual(datum.FrontMatrix(), datum.BackMatrix());
			}
			{
				Datum datum;
				Foo a{ 0 };
				datum = &a;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(a, *datum.FrontPointer()->As<Foo>());
				Assert::AreEqual(*datum.FrontPointer()->As<Foo>(), *datum.BackPointer()->As<Foo>());
			}
			{
				Datum datum;
				Scope a;
				datum = &a;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Table, datum.Type());
				Assert::AreEqual(a, *datum.FrontScope());
				Assert::AreEqual(*datum.FrontScope(), *datum.BackScope());
			}
		}

		TEST_METHOD(Equality) {
			{
				Datum datum;
				Datum other;

				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);
			}
			{
				Datum datum;
				Datum other;
				int32_t arr[] = { 1, 2, 3 };

				datum.SetStorage(arr, 3);
				other.SetStorage(arr, 3);

				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				int32_t arr2[] = { 4, 5, 6 };
				other.SetStorage(arr2, 3);

				Assert::IsTrue(datum != other);
				Assert::IsFalse(datum == other);
			}
			{
				{
					Datum datum;
					datum = 1;

					Assert::IsTrue(datum == 1);
					Assert::IsFalse(datum != 1);

					datum.Resize(5);

					Assert::IsTrue(datum != 1);
					Assert::IsFalse(datum == 1);
				}
				{
					Datum datum{ 1 };
					Datum other{ 1 };

					Assert::IsTrue(datum == other);
					Assert::IsFalse(datum != other);

					datum.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Resize(1);
					other.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);
				}
			}
			{
				{
					Datum datum;
					datum = 1.f;

					Assert::IsTrue(datum == 1.f);
					Assert::IsFalse(datum != 1.f);

					datum.Resize(5);

					Assert::IsTrue(datum != 1.f);
					Assert::IsFalse(datum == 1.f);
				}
				{
					Datum datum{ 1.f };
					Datum other{ 1.f };

					Assert::IsTrue(datum == other);
					Assert::IsFalse(datum != other);

					datum.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Resize(1);
					other.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);
				}
				
			}
			{
				{
					Datum datum;
					datum = "A";

					Assert::IsTrue(datum == "A");
					Assert::IsFalse(datum != "A");

					datum.Resize(5);

					Assert::IsTrue(datum != "A");
					Assert::IsFalse(datum == "A");
				}
				{
					Datum datum{ "A" };
					Datum other{ "A" };

					Assert::IsTrue(datum == other);
					Assert::IsFalse(datum != other);

					datum.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Resize(1);
					other.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);
				}
			}
			{
				{
					Datum datum;
					datum = glm::vec4(0);

					Assert::IsTrue(datum == glm::vec4(0));
					Assert::IsFalse(datum != glm::vec4(0));

					datum.Resize(5);

					Assert::IsTrue(datum != glm::vec4(0));
					Assert::IsFalse(datum == glm::vec4(0));
				}
				{
					Datum datum{ glm::vec4(0) };
					Datum other{ glm::vec4(0) };

					Assert::IsTrue(datum == other);
					Assert::IsFalse(datum != other);

					datum.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Resize(1);
					other.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);
				}
			}
			{
				{
					Datum datum;
					datum = glm::mat4(0);

					Assert::IsTrue(datum == glm::mat4(0));
					Assert::IsFalse(datum != glm::mat4(0));

					datum.Resize(5);

					Assert::IsTrue(datum != glm::mat4(0));
					Assert::IsFalse(datum == glm::mat4(0));
				}
				{
					Datum datum{ glm::mat4(0) };
					Datum other{ glm::mat4(0) };

					Assert::IsTrue(datum == other);
					Assert::IsFalse(datum != other);

					datum.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Resize(1);
					other.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);
				}
			}
			{
				{
					Datum datum(Datum::DatumTypes::Pointer);
					Foo a{ 10 };

					Assert::IsTrue(datum != &a);
					Assert::IsFalse(datum == &a);
				}
				{
					Datum datum;
					Foo a{ 10 };
					datum = &a;

					Assert::IsTrue(datum == &a);
					Assert::IsFalse(datum != &a);

					datum.Resize(5);

					Assert::IsTrue(datum != &a);
					Assert::IsFalse(datum == &a);
				}
				{
					Foo a{ 10 };
					Datum datum{ &a };
					Datum other{ &a };

					Assert::IsTrue(datum == other);
					Assert::IsFalse(datum != other);

					datum.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Resize(1);
					other.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Clear();

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					Foo b{ 20 };
					datum = &b;
					other.Resize(1);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);
				}
				
			}
			{
				{
					Datum datum(Datum::DatumTypes::Table);
					Scope a{ 10 };

					Assert::IsTrue(datum != &a);
					Assert::IsFalse(datum == &a);
				}
				{
					Datum datum;
					Scope a;
					datum = &a;

					Assert::IsTrue(datum == &a);
					Assert::IsFalse(datum != &a);

					datum.Resize(5);

					Assert::IsTrue(datum != &a);
					Assert::IsFalse(datum == &a);
				}
				{
					Scope a{ 10 };
					Datum datum{ &a };
					Datum other{ &a };

					Assert::IsTrue(datum == other);
					Assert::IsFalse(datum != other);

					datum.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Resize(1);
					other.Resize(5);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);

					datum.Clear();

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other); 
					
					Scope b{ 20 }; 
					Scope* child = new Scope();
					b.Adopt(*child, "TEST");

					datum = &b;
					other.Resize(1);

					Assert::IsTrue(datum != other);
					Assert::IsFalse(datum == other);
				}
			}
		}

		TEST_METHOD(OperatorBracketBracket) {
			Scope a;
			Scope b{ 10 };
			Scope c{ 20 };

			Datum datum = { &a, &b, &c };

			Assert::AreEqual(a, datum[0]);
			Assert::AreEqual(b, datum[1]);
			Assert::AreEqual(c, datum[2]);
		}

		TEST_METHOD(PushBack) {
			{
				Datum datum;
				datum.PushBack(1);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Integer, datum.Type());
				Assert::AreEqual(1, datum.FrontInt());
				Assert::AreEqual(1, datum.BackInt());

				datum.PushBack(2);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(1, datum.FrontInt());
				Assert::AreEqual(2, datum.BackInt());
			}
			{
				Datum datum;
				datum.PushBack(1.f);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Float, datum.Type());
				Assert::AreEqual(1.f, datum.FrontFloat());
				Assert::AreEqual(1.f, datum.BackFloat());

				datum.PushBack(2.f);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(1.f, datum.FrontFloat());
				Assert::AreEqual(2.f, datum.BackFloat());
			}
			{
				Datum datum;
				datum.PushBack("A");

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::String, datum.Type());
				Assert::AreEqual("A"s, datum.FrontString());
				Assert::AreEqual("A"s, datum.BackString());

				datum.PushBack("B");
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual("A"s, datum.FrontString());
				Assert::AreEqual("B"s, datum.BackString());
			}
			{
				Datum datum;
				datum.PushBack(glm::vec4(0));

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Vector, datum.Type());
				Assert::AreEqual(glm::vec4(0), datum.FrontVector());
				Assert::AreEqual(glm::vec4(0), datum.BackVector());

				datum.PushBack(glm::vec4(1));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(glm::vec4(0), datum.FrontVector());
				Assert::AreEqual(glm::vec4(1), datum.BackVector());
			}
			{
				Datum datum;
				datum.PushBack(glm::mat4(0));

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
				Assert::AreEqual(glm::mat4(0), datum.BackMatrix());

				datum.PushBack(glm::mat4(1));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
				Assert::AreEqual(glm::mat4(1), datum.BackMatrix());
			}
			{
				Datum datum;
				Foo a{ 10 };
				datum.PushBack(&a);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(a, *datum.FrontPointer()->As<Foo>());
				Assert::AreEqual(a, *datum.BackPointer()->As<Foo>());

				Foo b{ 20 };
				datum.PushBack(&b);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(a, *datum.FrontPointer()->As<Foo>());
				Assert::AreEqual(b, *datum.BackPointer()->As<Foo>());
			}
			{
				Datum datum;
				Scope a{ 10 };
				datum.PushBack(&a);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Table, datum.Type());
				Assert::AreEqual(a, *datum.FrontScope());
				Assert::AreEqual(a, *datum.BackScope());

				Scope b{ 20 };
				datum.PushBack(&b);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(a, *datum.FrontScope());
				Assert::AreEqual(b, *datum.BackScope());
			}
		}

		TEST_METHOD(Set) {
			{
				{
					Datum datum{ 1, 2, 3 };

					Assert::AreEqual(1, datum.FrontInt());
					Assert::AreEqual(3, datum.BackInt());

					datum.Set(0);

					Assert::AreEqual(0, datum.FrontInt());

					datum.Set(5, 2);

					Assert::AreEqual(5, datum.BackInt());
				}
				{
					Datum datum;
					int arr[] = {1, 2, 3};
					datum.SetStorage(arr, 3);

					Assert::AreEqual(1, datum.FrontInt());
					Assert::AreEqual(3, datum.BackInt());

					datum.Set(0);

					Assert::AreEqual(0, datum.FrontInt());

					datum.Set(5, 2);

					Assert::AreEqual(5, datum.BackInt());
				}
			}
			{
				Datum datum{ 1.f, 2.f, 3.f };

				Assert::AreEqual(1.f, datum.FrontFloat());
				Assert::AreEqual(3.f, datum.BackFloat());

				datum.Set(0.f);

				Assert::AreEqual(0.f, datum.FrontFloat());

				datum.Set(5.f, 2);

				Assert::AreEqual(5.f, datum.BackFloat());
			}
			{
				Datum datum{ "A", "B", "C"};

				Assert::AreEqual("A"s, datum.FrontString());
				Assert::AreEqual("C"s, datum.BackString());

				datum.Set("D");

				Assert::AreEqual("D"s, datum.FrontString());

				datum.Set("E", 2);

				Assert::AreEqual("E"s, datum.BackString());
			}
			{
				Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };

				Assert::AreEqual(glm::vec4(0), datum.FrontVector());
				Assert::AreEqual(glm::vec4(2), datum.BackVector());

				datum.Set(glm::vec4(3));

				Assert::AreEqual(glm::vec4(3), datum.FrontVector());

				datum.Set(glm::vec4(4), 2);

				Assert::AreEqual(glm::vec4(4), datum.BackVector());
			}
			{
				Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };

				Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
				Assert::AreEqual(glm::mat4(2), datum.BackMatrix());

				datum.Set(glm::mat4(3));

				Assert::AreEqual(glm::mat4(3), datum.FrontMatrix());

				datum.Set(glm::mat4(4), 2);

				Assert::AreEqual(glm::mat4(4), datum.BackMatrix());
			}
			{
				Foo a{ 10 };
				Foo b{ 20 };
				Foo c{ 30 };
				Datum datum{ &a, &b, &c };

				Assert::AreEqual(a, *datum.FrontPointer()->As<Foo>());
				Assert::AreEqual(c, *datum.BackPointer()->As<Foo>());

				Foo d{ 40 };
				datum.Set(&d);

				Assert::AreEqual(d, *datum.FrontPointer()->As<Foo>());

				Foo e{ 50 };
				datum.Set(&e, 2);

				Assert::AreEqual(e, *datum.BackPointer()->As<Foo>());
			}
			{
				Scope a{ 10 };
				Scope b{ 20 };
				Scope c{ 30 };
				Datum datum{ &a, &b, &c };

				Assert::AreEqual(a, *datum.FrontScope());
				Assert::AreEqual(c, *datum.BackScope());

				Scope d{ 40 };
				datum.Set(&d);

				Assert::AreEqual(d, *datum.FrontScope());

				Scope e{ 50 };
				datum.Set(&e, 2);

				Assert::AreEqual(e, *datum.BackScope());
			}
		}

		TEST_METHOD(SetStorage) {
			{
				Datum datum{ 1, 2, 3 };

				Assert::ExpectException<std::runtime_error>([&datum] {
					int arr[] = { 1, 2, 3 };
					datum.SetStorage(arr, 3);
					});
			}
			{
				Datum datum;
				int32_t arr[] = { 1, 2, 3 };
				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Integer, datum.Type());
				Assert::AreEqual(1, datum.FrontInt());
				Assert::AreEqual(3, datum.BackInt());
			}
			{
				Datum datum;
				float arr[] = { 1.f, 2.f, 3.f };
				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Float, datum.Type());
				Assert::AreEqual(1.f, datum.FrontFloat());
				Assert::AreEqual(3.f, datum.BackFloat());
			}
			{
				Datum datum;
				std::string arr[] = { "A", "B", "C" };
				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::String, datum.Type());
				Assert::AreEqual("A"s, datum.FrontString());
				Assert::AreEqual("C"s, datum.BackString());
			}
			{
				Datum datum;
				glm::vec4 arr[] = { glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Vector, datum.Type());
				Assert::AreEqual(glm::vec4(0), datum.FrontVector());
				Assert::AreEqual(glm::vec4(2), datum.BackVector());
			}
			{
				Datum datum;
				glm::mat4 arr[] = { glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
				Assert::AreEqual(glm::mat4(2), datum.BackMatrix());
			}
			{
				Datum datum;
				Foo a{ 0 };
				Foo b{ 1 };
				Foo c{ 2 };
				RTTI* arr[] = { &a, &b, &c };
				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(Foo(0), *datum.FrontPointer()->As<Foo>());
				Assert::AreEqual(Foo(2), *datum.BackPointer()->As<Foo>());
			}
			{
				Datum datum;
				Scope a{ 5 };
				Scope b{ 10 };
				Scope c{ 20 };
				Scope* arr[] = { &a, &b, &c };
				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Table, datum.Type());
				Assert::AreEqual(Scope(5), *datum.FrontScope());
				Assert::AreEqual(Scope(20), *datum.BackScope());
			}
		}

		TEST_METHOD(Get) {
			{
				{
					Datum datum{ 1, 2, 3 };
					Assert::AreEqual(1, datum.GetInt());
					Assert::AreEqual(2, datum.GetInt(1));
					Assert::AreEqual(3, datum.GetInt(2));

					Assert::ExpectException<std::runtime_error>([&datum] {
						auto& fail = datum.GetFloat(); UNREFERENCED_LOCAL(fail);
						});

					Assert::ExpectException<std::runtime_error>([&datum] {
						auto& fail = datum.GetInt(5); UNREFERENCED_LOCAL(fail);
						});
				}
				{
					const Datum datum{ 1, 2, 3 };
					Assert::AreEqual(1, datum.GetInt());
					Assert::AreEqual(2, datum.GetInt(1));
					Assert::AreEqual(3, datum.GetInt(2));
				}
			}
			{
				{
					Datum datum{ 1.f, 2.f, 3.f };
					Assert::AreEqual(1.f, datum.GetFloat());
					Assert::AreEqual(2.f, datum.GetFloat(1));
					Assert::AreEqual(3.f, datum.GetFloat(2));
				}
				{
					const Datum datum{ 1.f, 2.f, 3.f };
					Assert::AreEqual(1.f, datum.GetFloat());
					Assert::AreEqual(2.f, datum.GetFloat(1));
					Assert::AreEqual(3.f, datum.GetFloat(2));
				}
			}
			{
				{
					Datum datum{ "A", "B", "C" };
					Assert::AreEqual("A"s, datum.GetString());
					Assert::AreEqual("B"s, datum.GetString(1));
					Assert::AreEqual("C"s, datum.GetString(2));
				}
				{
					const Datum datum{ "A", "B", "C" };
					Assert::AreEqual("A"s, datum.GetString());
					Assert::AreEqual("B"s, datum.GetString(1));
					Assert::AreEqual("C"s, datum.GetString(2));
				}
			}
			{
				{
					Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
					Assert::AreEqual(glm::vec4(0), datum.GetVector());
					Assert::AreEqual(glm::vec4(1), datum.GetVector(1));
					Assert::AreEqual(glm::vec4(2), datum.GetVector(2));
				}
				{
					const Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
					Assert::AreEqual(glm::vec4(0), datum.GetVector());
					Assert::AreEqual(glm::vec4(1), datum.GetVector(1));
					Assert::AreEqual(glm::vec4(2), datum.GetVector(2));
				}
			}
			{
				{
					Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
					Assert::AreEqual(glm::mat4(0), datum.GetMatrix());
					Assert::AreEqual(glm::mat4(1), datum.GetMatrix(1));
					Assert::AreEqual(glm::mat4(2), datum.GetMatrix(2));
				}
				{
					const Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
					Assert::AreEqual(glm::mat4(0), datum.GetMatrix());
					Assert::AreEqual(glm::mat4(1), datum.GetMatrix(1));
					Assert::AreEqual(glm::mat4(2), datum.GetMatrix(2));
				}
			}
			{
				{
					Foo a{ 0 };
					Foo b{ 1 };
					Foo c{ 2 };
					Datum datum{ &a, &b, &c };
					Assert::AreEqual(a, *datum.GetPointer()->As<Foo>());
					Assert::AreEqual(b, *datum.GetPointer(1)->As<Foo>());
					Assert::AreEqual(c, *datum.GetPointer(2)->As<Foo>());
				}
				{
					Foo a{ 0 };
					Foo b{ 1 };
					Foo c{ 2 };
					const Datum datum{ &a, &b, &c };
					Assert::AreEqual(a, *datum.GetPointer()->As<Foo>());
					Assert::AreEqual(b, *datum.GetPointer(1)->As<Foo>());
					Assert::AreEqual(c, *datum.GetPointer(2)->As<Foo>());
				}
			}
			{
				{
					Scope a{ 5 };
					Scope b{ 10 };
					Scope c{ 20 };
					Datum datum{ &a, &b, &c };
					Assert::AreEqual(a, *datum.GetScope());
					Assert::AreEqual(b, *datum.GetScope(1));
					Assert::AreEqual(c, *datum.GetScope(2));
				}
				{
					Scope a{ 5 };
					Scope b{ 10 };
					Scope c{ 20 };
					const Datum datum{ &a, &b, &c };
					Assert::AreEqual(a, *datum.GetScope());
					Assert::AreEqual(b, *datum.GetScope(1));
					Assert::AreEqual(c, *datum.GetScope(2));
				}
			}
		}

		TEST_METHOD(FrontAndBack) {
			{
				{
					Datum datum{ 1, 2, 3 };
					Assert::AreEqual(1, datum.FrontInt());
					Assert::AreEqual(3, datum.BackInt());
				}
				{
					const Datum datum{ 1, 2, 3 };
					Assert::AreEqual(1, datum.FrontInt());
					Assert::AreEqual(3, datum.BackInt());
				}
			}
			{
				{
					Datum datum{ 1.f, 2.f, 3.f };
					Assert::AreEqual(1.f, datum.FrontFloat());
					Assert::AreEqual(3.f, datum.BackFloat());
				}
				{
					const Datum datum{ 1.f, 2.f, 3.f };
					Assert::AreEqual(1.f, datum.FrontFloat());
					Assert::AreEqual(3.f, datum.BackFloat());
				}
			}
			{
				{
					Datum datum{ "A", "B", "C" };
					Assert::AreEqual("A"s, datum.FrontString());
					Assert::AreEqual("C"s, datum.BackString());
				}
				{
					const Datum datum{ "A", "B", "C" };
					Assert::AreEqual("A"s, datum.FrontString());
					Assert::AreEqual("C"s, datum.BackString());
				}
			}
			{
				{
					Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
					Assert::AreEqual(glm::vec4(0), datum.FrontVector());
					Assert::AreEqual(glm::vec4(2), datum.BackVector());
				}
				{
					const Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
					Assert::AreEqual(glm::vec4(0), datum.FrontVector());
					Assert::AreEqual(glm::vec4(2), datum.BackVector());
				}
			}
			{
				{
					Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
					Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
					Assert::AreEqual(glm::mat4(2), datum.BackMatrix());
				}
				{
					const Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
					Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
					Assert::AreEqual(glm::mat4(2), datum.BackMatrix());
				}
			}
			{
				{
					Foo a{ 0 };
					Foo b{ 1 };
					Foo c{ 2 };
					Datum datum{ &a, &b, &c };
					Assert::AreEqual(a, *datum.FrontPointer()->As<Foo>());
					Assert::AreEqual(c, *datum.BackPointer()->As<Foo>());
				}
				{
					Foo a{ 0 };
					Foo b{ 1 };
					Foo c{ 2 };
					const Datum datum{ &a, &b, &c };
					Assert::AreEqual(a, *datum.FrontPointer()->As<Foo>());
					Assert::AreEqual(c, *datum.BackPointer()->As<Foo>());
				}
			}
			{
				{
					Scope a{ 5 };
					Scope b{ 10 };
					Scope c{ 20 };
					Datum datum{ &a, &b, &c };
					Assert::AreEqual(a, *datum.FrontScope());
					Assert::AreEqual(c, *datum.BackScope());
				}
				{
					Scope a{ 5 };
					Scope b{ 10 };
					Scope c{ 20 };
					const Datum datum{ &a, &b, &c };
					Assert::AreEqual(a, *datum.FrontScope());
					Assert::AreEqual(c, *datum.BackScope());
				}
			}
			
		}

		TEST_METHOD(GetIndex) {
			{
				Datum datum{ 1, 2, 3 };

				Assert::AreEqual(size_t(1), datum.GetIndex(2));

				Assert::AreEqual(datum.Size(), datum.GetIndex(5));

				Assert::ExpectException<std::runtime_error>([&datum] {
					size_t fail = datum.GetIndex("A"); UNREFERENCED_LOCAL(fail);
					});
			}
			{
				Datum datum{ 1.f, 2.f, 3.f };

				Assert::AreEqual(size_t(1), datum.GetIndex(2.f));
			}
			{
				Datum datum{ "A", "B", "C" };

				Assert::AreEqual(size_t(1), datum.GetIndex("B"));
			}
			{
				Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };

				Assert::AreEqual(size_t(1), datum.GetIndex(glm::vec4(1)));
			}
			{
				Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };

				Assert::AreEqual(size_t(1), datum.GetIndex(glm::mat4(1)));
			}
			{
				Foo a{ 0 };
				Foo b{ 1 };
				Foo c{ 2 };
				Datum datum{ &a, &b, &c };

				Assert::AreEqual(size_t(1), datum.GetIndex(&b));
			}
			{
				Scope a{ 5 };
				Scope b{ 10 };
				Scope c{ 20 };
				Datum datum{ &a, &b, &c };

				Assert::AreEqual(size_t(1), datum.GetIndex(&b));
			}
		}

		TEST_METHOD(PopBack) {
			{
				Datum datum;

				Assert::ExpectException<std::runtime_error>([&datum] {
					datum.PopBack();
					});
			}
			{
				Datum datum;
				int32_t arr[] = { 1, 2, 3 };
				datum.SetStorage(arr, 3);

				Assert::ExpectException<std::runtime_error>([&datum] {
					datum.PopBack();
					});
			}
			{
				Datum datum{ 1, 2, 3 };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(3, datum.BackInt());

				datum.PopBack();
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(2, datum.BackInt());

				datum.PopBack();
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(1, datum.BackInt());

				datum.PopBack();
				Assert::AreEqual(size_t(0), datum.Size());
			}

			{
				Datum datum{ "A", "B", "C" };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual("C"s, datum.BackString());

				datum.PopBack();
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual("B"s, datum.BackString());

				datum.PopBack();
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual("A"s, datum.BackString());

				datum.PopBack();
				Assert::AreEqual(size_t(0), datum.Size());
			}
		}

		TEST_METHOD(Remove) {
			{
				Datum datum;
				int32_t arr[] = { 1, 2, 3 };
				datum.SetStorage(arr, 3);

				Assert::ExpectException<std::runtime_error>([&datum] {
					datum.Remove(1);
					});
			}
			{
				Datum datum{ 1, 2, 3 };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreNotEqual(datum.Size(), datum.GetIndex(2));

				datum.Remove(2);

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(datum.Size(), datum.GetIndex(2));

				Assert::ExpectException<std::runtime_error>([&datum] {
					datum.Remove(5);
					});
			}
			{
				Datum datum{ 1.f, 2.f, 3.f };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreNotEqual(datum.Size(), datum.GetIndex(2.f));

				datum.Remove(2.f);

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(datum.Size(), datum.GetIndex(2.f));
			}
			{
				Datum datum{ "A", "B", "C" };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreNotEqual(datum.Size(), datum.GetIndex("B"));

				datum.Remove("B");

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(datum.Size(), datum.GetIndex("B"));
			}
			{
				Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreNotEqual(datum.Size(), datum.GetIndex(glm::vec4(1)));

				datum.Remove(glm::vec4(1));

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(datum.Size(), datum.GetIndex(glm::vec4(1)));
			}
			{
				Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreNotEqual(datum.Size(), datum.GetIndex(glm::mat4(1)));

				datum.Remove(glm::mat4(1));

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(datum.Size(), datum.GetIndex(glm::mat4(1)));
			}
			{
				Foo a{ 0 };
				Foo b{ 1 };
				Foo c{ 2 };
				Datum datum{ &a, &b, &c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreNotEqual(datum.Size(), datum.GetIndex(&b));

				datum.Remove(&b);

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(datum.Size(), datum.GetIndex(&b));
			}
			{
				Scope a{ 5 };
				Scope b{ 10 };
				Scope c{ 20 };
				Datum datum{ &a, &b, &c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreNotEqual(datum.Size(), datum.GetIndex(&b));

				datum.Remove(&b);

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(datum.Size(), datum.GetIndex(&b));
			}
		}

		TEST_METHOD(SetType) {
			Datum datum;

			Assert::AreEqual(Datum::DatumTypes::Unknown, datum.Type());

			datum.SetType(Datum::DatumTypes::Integer);
			Assert::AreEqual(Datum::DatumTypes::Integer, datum.Type());

			Assert::ExpectException<std::runtime_error>([&datum] {
				datum.SetType(Datum::DatumTypes::Unknown);
				});

			Assert::ExpectException<std::runtime_error>([&datum] {
				datum.SetType(Datum::DatumTypes::Float);
				});
		}

		TEST_METHOD(ToString) {
			{
				Datum d{ 1 };

				Assert::AreEqual(to_string(1), d.ToString());

				Assert::ExpectException<std::runtime_error>([&d] {
					auto fail = d.ToString(5); UNREFERENCED_LOCAL(fail);
					});
			}
			{
				Datum d{ 1.f };

				Assert::AreEqual(to_string(1.f), d.ToString());
			}
			{
				Datum d{ "A" };

				Assert::AreEqual("A"s, d.ToString());
			}
			{
				Datum d{ glm::vec4(0) };

				Assert::AreEqual(glm::to_string(glm::vec4(0)), d.ToString());
			}
			{
				Datum d{ glm::mat4(0) };

				Assert::AreEqual(glm::to_string(glm::mat4(0)), d.ToString());
			}
			{
				Foo a{ 0 };
				Datum d{ &a };

				Assert::AreEqual(a.ToString(), d.ToString());
			}
			{
				Scope a{ 5 };
				Datum d{ &a };

				Assert::AreEqual(a.ToString(), d.ToString());
			}
		}

		TEST_METHOD(FromString) {
			{
				Datum datum{ 0 };
				std::string s = to_string(1);

				Assert::ExpectException<std::runtime_error>([&datum, &s] {
					datum.SetFromString(s, 5);
					});

				datum.SetFromString(s);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Integer, datum.Type());
				Assert::AreEqual(1, datum.FrontInt());
			}
			{
				Datum datum{ 0.f };
				std::string s = to_string(1.f);

				datum.SetFromString(s);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Float, datum.Type());
				Assert::AreEqual(1.f, datum.FrontFloat());
			}
			{
				Datum datum{ "" };
				std::string s = "A";

				datum.SetFromString(s);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::String, datum.Type());
				Assert::AreEqual("A"s, datum.FrontString());
			}
			{
				Datum datum{ glm::vec4(5) };
				std::string s = glm::to_string(glm::vec4(0));

				datum.SetFromString(s);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Vector, datum.Type());
				Assert::AreEqual(glm::vec4(0), datum.FrontVector());
			}
			{
				Datum datum{ glm::mat4(5) };
				std::string s = glm::to_string(glm::mat4(0));

				datum.SetFromString(s);

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(Datum::DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(glm::mat4(0), datum.FrontMatrix());
			}
			{
				Foo a{ 0 };
				Foo b{ 1 };
				Datum datum{ &a };
				std::string s = b.ToString();

				Assert::ExpectException<std::runtime_error>([&datum, &s] {
					datum.SetFromString(s);
					});
			}
			{
				Scope a{ 5 };
				Scope b{ 10 };
				Datum datum{ &a };
				std::string s = b.ToString();

				Assert::ExpectException<std::runtime_error>([&datum, &s] {
					datum.SetFromString(s);
					});
			}
		}

		TEST_METHOD(Resize) {
			{
				Datum datum{ 1, 2, 3 };
				Assert::AreEqual(size_t(3), datum.Size());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());

				datum.Resize(0);
				Assert::AreEqual(size_t(0), datum.Size());
			}
			{
				Datum datum{ 1.f, 2.f, 3.f };
				Assert::AreEqual(size_t(3), datum.Size());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());

				datum.Resize(0);
				Assert::AreEqual(size_t(0), datum.Size());
			}
			{
				Datum datum{ "A", "B", "C" };
				Assert::AreEqual(size_t(3), datum.Size());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());

				datum.Resize(0);
				Assert::AreEqual(size_t(0), datum.Size());
			}
			{
				Datum datum{ glm::vec4(0), glm::vec4(1), glm::vec4(2) };
				Assert::AreEqual(size_t(3), datum.Size());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());

				datum.Resize(0);
				Assert::AreEqual(size_t(0), datum.Size());
			}
			{
				Datum datum{ glm::mat4(0), glm::mat4(1), glm::mat4(2) };
				Assert::AreEqual(size_t(3), datum.Size());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());

				datum.Resize(0);
				Assert::AreEqual(size_t(0), datum.Size());
			}
			{
				Foo a{ 0 };
				Foo b{ 1 };
				Foo c{ 2 };
				Datum datum{ &a, &b, &c };
				Assert::AreEqual(size_t(3), datum.Size());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());

				datum.Resize(0);
				Assert::AreEqual(size_t(0), datum.Size());
			}
			{
				Scope a{ 5 };
				Scope b{ 10 };
				Scope c{ 20 };
				Datum datum{ &a, &b, &c };
				Assert::AreEqual(size_t(3), datum.Size());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());

				datum.Resize(0);
				Assert::AreEqual(size_t(0), datum.Size());
			}
		}

		TEST_METHOD(Clear) {
			{
				Datum datum{ 1, 2, 3 };

				Assert::AreEqual(size_t(3), datum.Size());

				size_t initialCapacity = datum.Capacity();

				datum.Clear();

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(initialCapacity, datum.Capacity());
			}
			{
				Datum datum{ "A", "B", "C" };

				Assert::AreEqual(size_t(3), datum.Size());

				size_t initialCapacity = datum.Capacity();

				datum.Clear();

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(initialCapacity, datum.Capacity());
			}
			{
				Datum datum;
				int32_t arr[] = { 1, 2, 3 };
				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());

				size_t initialCapacity = datum.Capacity();

				datum.Clear();

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(initialCapacity, datum.Capacity());
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}