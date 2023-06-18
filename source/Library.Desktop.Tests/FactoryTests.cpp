#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "ToStringSpecialization.h"
#include "Factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(FactoryTests)
	{
		CONCRETE_FACTORY(AttributedFoo, Scope);
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			TypeManager::CreateInstance();
			TypeManager::Instance()->RegisterSignature(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());

			Factory<Scope>::Add(std::make_unique<AttributedFooFactory>());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Factory<Scope>::Remove(std::make_unique<AttributedFooFactory>());

			TypeManager::Instance()->RemoveSignature(AttributedFoo::TypeIdClass());
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
			AttributedFooFactory fooFactory;

			Assert::AreEqual(fooFactory.ClassName(), "AttributedFoo"s);
			Assert::IsNotNull(fooFactory.Find("AttributedFoo"s));
		}

		TEST_METHOD(Create) {
			{
				AttributedFooFactory fooFactory;

				auto createdFactory = fooFactory.Create();
				Assert::IsNotNull(createdFactory);

				auto foo = createdFactory->As<AttributedFoo>();
				Assert::IsNotNull(foo);
				Assert::IsTrue((*foo)["this"] == foo);
				Assert::AreEqual(0, foo->externalInt);
				Assert::AreEqual(0.f, foo->externalFloat);
				Assert::AreEqual(std::string(), foo->externalString);
				Assert::AreEqual(glm::to_string(glm::vec4()), glm::to_string(foo->externalVec));
				Assert::AreEqual(glm::to_string(glm::mat4()), glm::to_string(foo->externalMat));

				delete createdFactory;
			}
			{
				auto createdFactory = Factory<Scope>::Create("AttributedFoo"s);

				auto foo = createdFactory->As<AttributedFoo>();
				Assert::IsNotNull(foo);
				Assert::IsTrue((*foo)["this"] == foo);
				Assert::AreEqual(0, foo->externalInt);
				Assert::AreEqual(0.f, foo->externalFloat);
				Assert::AreEqual(std::string(), foo->externalString);
				Assert::AreEqual(glm::to_string(glm::vec4()), glm::to_string(foo->externalVec));
				Assert::AreEqual(glm::to_string(glm::mat4()), glm::to_string(foo->externalMat));

				delete createdFactory;
			}
			{
				auto fail = Factory<Scope>::Create("Fail");
				Assert::IsNull(fail);
			}
		}

		TEST_METHOD(Find) {
			AttributedFooFactory fooFactory;

			Assert::IsNotNull(fooFactory.Find("AttributedFoo"s));
			Assert::IsNull(fooFactory.Find("NotInFactory"));
		}

		TEST_METHOD(Clear) {
			AttributedFooFactory fooFactory;

			Assert::IsNotNull(fooFactory.Find("AttributedFoo"s));
			fooFactory.Clear();
			Assert::IsNull(fooFactory.Find("AttributedFoo"s));
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}