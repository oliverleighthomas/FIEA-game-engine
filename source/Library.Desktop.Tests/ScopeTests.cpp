#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Bar.h"
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

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(FieaGameEngine::Datum* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(const FieaGameEngine::Datum* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(const FieaGameEngine::Scope& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(FieaGameEngine::Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(ScopeTests)
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

		TEST_METHOD(Constructor) {
			{
				Scope scope;

				Assert::IsNull(scope.GetParent());
				Assert::AreEqual(size_t(0), scope.Size());
			}
			{
				Scope scope(5);

				Assert::IsNull(scope.GetParent());
				Assert::AreEqual(size_t(0), scope.Size());
			}
		}

		TEST_METHOD(CopySemantics) {
			{
				Scope scope;
				scope.Append("ChildOne");
				scope["ChildOne"] = 5;
				scope.Append("ChildTwo");
				scope["ChildTwo"] = 10;

				Scope scopeCopy(scope);

				Assert::AreEqual(scope, scopeCopy);
			}
			{
				Scope scope;
				scope.Append("ChildOne");
				scope["ChildOne"] = 5;
				scope.Append("ChildTwo");
				scope["ChildTwo"] = 10;

				Scope scopeCopy;
				scopeCopy = scope;

				Assert::AreEqual(scope, scopeCopy);
			}
			{
				Scope scope;
				Scope* childOne = new Scope();
				Scope* childTwo = new Scope();

				scope.Adopt(*childOne, "ToCopy");
				scope.Adopt(*childTwo, "ToCopy");

				Scope scopeCopy(scope);

				Assert::AreEqual(scope, scopeCopy);
			}
			{
				Scope scope;
				Scope* childOne = new Scope();
				Scope* childTwo = new Scope();

				scope.Adopt(*childOne, "ToCopy");
				scope.Adopt(*childTwo, "ToCopy");

				Scope scopeCopy;
				scopeCopy = scope;

				Assert::AreEqual(scope, scopeCopy);
			}
		}

		TEST_METHOD(MoveSemantics) {
			Scope scope;
			Scope* childOne = new Scope();
			Scope* childTwo = new Scope();

			scope.Adopt(*childOne, "ToMove");
			scope.Adopt(*childTwo, "ToMove");
			{
				Scope scopeCopy(scope);
				Scope scopeMove(std::move(scopeCopy));

				Assert::AreEqual(scope, scopeMove);
			}
			{
				Scope scopeCopy(scope);
				Scope scopeMove;
				scopeMove = std::move(scopeCopy);

				Assert::AreEqual(scope, scopeMove);
			}
		}

		TEST_METHOD(Clone) {
			Scope scope;
			scope.Append("ChildOne");
			scope["ChildOne"] = 5;
			scope.Append("ChildTwo");
			scope["ChildTwo"] = 10;

			Scope* scopeCopy = scope.Clone();

			Assert::AreEqual(scope, *scopeCopy);

			delete(scopeCopy);
		}

		TEST_METHOD(EqualityOperators) {
			Scope scopeOne;
			scopeOne.Append("ChildOne");
			scopeOne["ChildOne"] = 5;
			scopeOne.Append("ChildTwo");
			scopeOne["ChildTwo"] = 10;

			Assert::IsTrue(scopeOne == scopeOne);
			Assert::IsFalse(scopeOne != scopeOne);

			Scope scopeTwo;

			Assert::IsFalse(scopeOne == scopeTwo);
			Assert::IsTrue(scopeOne != scopeTwo);
			
			scopeTwo = scopeOne;

			Assert::IsTrue(scopeOne == scopeTwo);
			Assert::IsFalse(scopeOne != scopeTwo);

			scopeTwo["ChileOne"] = 6;

			Assert::IsFalse(scopeOne == scopeTwo);
			Assert::IsTrue(scopeOne != scopeTwo);
		}

		TEST_METHOD(BracketOperators) {
			{
				Scope scope;
				Datum& d = scope.Append("ChildOne");
				scope["ChildOne"] = 5;

				Assert::AreEqual(d, scope["ChildOne"]);

				auto newDatum = scope["ChildTwo"];

				Assert::AreEqual(size_t(2), scope.Size());
				Assert::AreEqual(newDatum, scope["ChildTwo"]);
			}
			{
				Scope scope;
				Datum& d = scope.Append("ChildOne");
				scope["ChildOne"] = 5;

				const Scope cScope(scope);

				Assert::AreEqual(d, cScope["ChildOne"]);

				Assert::ExpectException<std::runtime_error>([&cScope] {
					auto fail = cScope["Fail"]; UNREFERENCED_LOCAL(fail);
					});
			}
			{
				Scope scope;
				Datum& d = scope.Append("ChildOne");
				scope["ChildOne"] = 5;

				Assert::AreEqual(d, scope[0]);
			}
			{
				Scope scope;
				Datum& d = scope.Append("ChildOne");
				scope["ChildOne"] = 5;

				const Scope cScope(scope);

				Assert::AreEqual(d, cScope[0]);
			}
		}

		TEST_METHOD(Append) {
			Scope scope;

			Assert::ExpectException<std::runtime_error>([&scope] {
				scope.Append("");
				});

			Datum d = scope.Append("Child");
			Assert::AreEqual(d, scope["Child"]);
			Assert::AreEqual(size_t(1), scope.Size());

			scope.Append("Child");
			Assert::AreEqual(size_t(1), scope.Size());
		}

		TEST_METHOD(AppendScope) {
			Scope scope;
			scope.Append("Child");
			scope["Child"] = 5;

			Assert::ExpectException<std::runtime_error>([&scope] {
				scope.AppendScope("Child");
				});

			scope.Clear();

			Scope& child = scope.AppendScope("Child");

			Assert::AreEqual(size_t(1), scope.Size());
			Assert::AreEqual(&scope, child.GetParent());
			Assert::IsNotNull(scope.Find("Child"));
		}

		TEST_METHOD(Find) {
			{
				Scope scope;
				Datum& d = scope.Append("Child");
				scope["Child"] = 5;

				Assert::AreEqual(d, *scope.Find("Child"));
				Assert::IsNull(scope.Find("NotInScope"));
			}
			{
				Scope scope;
				Datum& d = scope.Append("Child");
				scope["Child"] = 5;

				const Scope cScope(scope);

				Assert::AreEqual(d, *cScope.Find("Child"));
				Assert::IsNull(cScope.Find("NotInScope"));
			}
		}

		TEST_METHOD(Search) {
			Scope scope;
			Datum& a = scope.Append("A");
			Scope* child = new Scope();
			Datum& b = child->Append("B");
			scope.Adopt(*child, "Child");
			Scope* grandChild = new Scope();
			Datum& c = grandChild->Append("C");
			child->Adopt(*grandChild, "Child");

			Scope* containingScope;

			Assert::AreEqual(a, *scope.Search("A"));
			Assert::AreEqual(a, *child->Search("A"));
			Assert::AreEqual(a, *grandChild->Search("A", &containingScope));
			Assert::AreEqual(scope, *containingScope);

			Assert::IsNull(scope.Search("B", &containingScope));
			Assert::IsNull(containingScope);
			Assert::AreEqual(b, *child->Search("B"));
			Assert::AreEqual(b, *grandChild->Search("B", &containingScope));
			Assert::AreEqual(child, containingScope);

			Assert::IsNull(scope.Search("C"));
			Assert::IsNull(child->Search("C", &containingScope));
			Assert::IsNull(containingScope);
			Assert::AreEqual(c, *grandChild->Search("C", &containingScope));
			Assert::AreEqual(grandChild, containingScope);
		}

		TEST_METHOD(FindContainedScope) {
			Scope scope;
			Scope* childOne = new Scope();
			scope.Adopt(*childOne, "Children");
			Scope* childTwo = new Scope();
			scope.Adopt(*childTwo, "Children");
			Scope* childThree = new Scope();
			scope.Adopt(*childThree, "Children");

			Assert::AreEqual(scope["Children"], *scope.FindContainedScope(*childOne).first);
			Assert::AreEqual(size_t(0), scope.FindContainedScope(*childOne).second);

			Assert::AreEqual(scope["Children"], *scope.FindContainedScope(*childTwo).first);
			Assert::AreEqual(size_t(1), scope.FindContainedScope(*childTwo).second);

			Assert::AreEqual(scope["Children"], *scope.FindContainedScope(*childThree).first);
			Assert::AreEqual(size_t(2), scope.FindContainedScope(*childThree).second);

			Scope* notInScope = new Scope();
			Assert::IsNull(scope.FindContainedScope(*notInScope).first);
			Assert::AreEqual(scope.Size(), scope.FindContainedScope(*notInScope).second);
			delete(notInScope);
		}

		TEST_METHOD(Adopt) {
			Scope scope;
			Scope* child = new Scope();
			scope.Adopt(*child, "Children");

			Assert::AreEqual(size_t(1), scope.Size());
			Assert::AreEqual(scope, *child->GetParent());
			Assert::IsNotNull(scope.Find("Children"));

			Scope* grandChild = new Scope();
			child->Adopt(*grandChild, "Children");

			Assert::AreEqual(size_t(1), child->Size());
			Assert::AreEqual(child, grandChild->GetParent());
			Assert::IsNotNull(child->Find("Children"));

			Assert::ExpectException<std::runtime_error>([&scope] {
				scope.Adopt(scope, "Children");
				});

			Assert::ExpectException<std::runtime_error>([&grandChild, &scope] {
				grandChild->Adopt(scope, "Children");
				});

			scope.Adopt(*grandChild, "Children");
			Assert::AreEqual(size_t(1), scope.Size());
			Assert::AreEqual(scope, *grandChild->GetParent());
		}

		TEST_METHOD(Orphan) {
			Scope scope;
			Scope* child = new Scope();
			scope.Adopt(*child, "Children");
			Scope* grandChild = new Scope();
			child->Adopt(*grandChild, "Children");

			Assert::AreEqual(scope, *child->GetParent());
			Assert::AreEqual(child, grandChild->GetParent());

			grandChild->Orphan();

			Assert::AreEqual(scope, *child->GetParent());
			Assert::IsNull(grandChild->GetParent());

			delete(grandChild);

			child->Orphan();

			Assert::IsNull(child->GetParent());

			delete(child);
		}

		TEST_METHOD(AncestorAndDescendantOf) {
			Scope scope;
			Scope* child = new Scope();
			scope.Adopt(*child, "Children");
			Scope* grandChild = new Scope();
			child->Adopt(*grandChild, "Children");

			Assert::IsTrue(scope.IsAncestorOf(*child));
			Assert::IsTrue(scope.IsAncestorOf(*grandChild));
			Assert::IsTrue(child->IsAncestorOf(*grandChild));
			Assert::IsTrue(child->IsDescendantOf(scope));
			Assert::IsTrue(grandChild->IsDescendantOf(scope));
			Assert::IsTrue(grandChild->IsDescendantOf(*child));

			scope.Adopt(*grandChild, "Children");

			Assert::IsTrue(scope.IsAncestorOf(*child));
			Assert::IsTrue(scope.IsAncestorOf(*grandChild));
			Assert::IsFalse(child->IsAncestorOf(*grandChild));
			Assert::IsTrue(child->IsDescendantOf(scope));
			Assert::IsTrue(grandChild->IsDescendantOf(scope));
			Assert::IsFalse(grandChild->IsDescendantOf(*child));
		}

		TEST_METHOD(GetParent) {
			Scope scope;
			Scope* child = new Scope();
			scope.Adopt(*child, "Children");
			Scope* grandChild = new Scope();
			child->Adopt(*grandChild, "Children");

			Assert::IsNull(scope.GetParent());
			Assert::AreEqual(scope, *child->GetParent());
			Assert::AreEqual(child, grandChild->GetParent());
		}

		TEST_METHOD(Size) {
			Scope scope;

			Assert::AreEqual(size_t(0), scope.Size());

			scope.Append("One");

			Assert::AreEqual(size_t(1), scope.Size());

			scope.Append("Two");

			Assert::AreEqual(size_t(2), scope.Size());

			scope.Clear();

			Assert::AreEqual(size_t(0), scope.Size());
		}

		TEST_METHOD(RTTI) {
			Scope scope;

			Assert::IsTrue(scope.Is(scope.TypeIdClass()));
			Assert::IsNull(scope.As<Foo>());
			Assert::IsNotNull(scope.As<Scope>());
			Assert::IsFalse(scope.Is(Foo::TypeIdClass()));
		}

		TEST_METHOD(Equals) {
			Scope scopeOne, scopeTwo;
			Foo foo;

			Assert::IsTrue(scopeOne.Equals(&scopeTwo));
			Assert::IsFalse(scopeOne.Equals(&foo));
			Assert::IsFalse(scopeOne.Equals(nullptr));
		}

		TEST_METHOD(ToString) {
			Scope scope;

			Assert::AreEqual("Scope()"s, scope.ToString());

			scope.Append("Child");

			Assert::AreEqual("Scope(\"Child\"{})"s, scope.ToString());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}