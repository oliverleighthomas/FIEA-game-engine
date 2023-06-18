#include "pch.h"
#include "CppUnitTest.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "AttributedFooChild.h"
#include "ToStringSpecialization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework {
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
}

namespace LibraryDesktopTests
{
	TEST_CLASS(TypeManagerTests)
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

		TEST_METHOD(SignatureEquality) {
			Signature one = Signature("One", Datum::DatumTypes::Integer, size_t(0), size_t(0));
			Signature two = Signature("One", Datum::DatumTypes::Integer, size_t(0), size_t(0));
			Signature three = Signature("Three", Datum::DatumTypes::Float, size_t(1), size_t(1));

			Assert::IsTrue(one == two);
			Assert::IsFalse(one != two);

			Assert::IsFalse(one == three);
			Assert::IsTrue(one != three);
		}

		TEST_METHOD(CreateAndDestroy) {
			Assert::IsNull(TypeManager::Instance());

			TypeManager::CreateInstance();

			Assert::IsNotNull(TypeManager::Instance());

			TypeManager::DeleteInstance();

			Assert::IsNull(TypeManager::Instance());
		}

		TEST_METHOD(RegisterAndRemove) {
			TypeManager::CreateInstance();

			Assert::ExpectException<std::runtime_error>([] {
				TypeManager::Instance()->RegisterType<AttributedFooChild>();
				});

			TypeManager::Instance()->RegisterType<AttributedFoo>();

			Assert::IsNotNull(TypeManager::Instance()->FindType(AttributedFoo::TypeIdClass()));
			Assert::ExpectException<std::runtime_error>([] {
				TypeManager::Instance()->RegisterType<AttributedFoo>();
				});

			TypeManager::Instance()->RegisterType<AttributedFooChild>();

			Assert::IsNotNull(TypeManager::Instance()->FindType(AttributedFooChild::TypeIdClass()));
			Assert::ExpectException<std::runtime_error>([] {
				TypeManager::Instance()->RegisterType<AttributedFooChild>();
				});

			TypeManager::Instance()->RemoveType(AttributedFoo::TypeIdClass());

			Assert::IsNull(TypeManager::Instance()->FindType(AttributedFoo::TypeIdClass()));

			TypeManager::Instance()->RemoveType(AttributedFooChild::TypeIdClass());

			Assert::IsNull(TypeManager::Instance()->FindType(AttributedFooChild::TypeIdClass()));

			TypeManager::DeleteInstance();
		}

		TEST_METHOD(Clear) {
			TypeManager::CreateInstance();

			TypeManager::Instance()->RegisterType<AttributedFoo>();
			TypeManager::Instance()->RegisterType<AttributedFooChild>();

			TypeManager::Instance()->Clear();

			Assert::IsNull(TypeManager::Instance()->FindType(AttributedFoo::TypeIdClass()));
			Assert::IsNull(TypeManager::Instance()->FindType(AttributedFooChild::TypeIdClass()));

			TypeManager::DeleteInstance();
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}