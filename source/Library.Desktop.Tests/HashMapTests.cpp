#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Bar.h"
#include "HashMap.h"
#include "ToStringSpecialization.h"
#include "DefaultHash.h"
#include "DefaultEquality.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework {
	template<>
	inline std::wstring ToString<std::pair<const int32_t, UnitTests::Foo>>(const std::pair <const int32_t, UnitTests::Foo>& t)
	{
		std::wstring value;
		try {
			value = ToString(t.first);
			value += ToString(" ");
			value += ToString(t.second);
		}
		catch (const std::exception) {
			value = L"end()"s;
		}
		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::HashMap<int32_t, UnitTests::Foo>::Iterator>(const FieaGameEngine::HashMap<int32_t, UnitTests::Foo>::Iterator& t)
	{
		std::wstring value;
		try {
			value = ToString(*t);
		}
		catch (const std::exception) {
			value = L"end()"s;
		}
		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::HashMap<int32_t, UnitTests::Foo>::ConstIterator>(const FieaGameEngine::HashMap<int32_t, UnitTests::Foo>::ConstIterator& t)
	{
		std::wstring value;
		try {
			value = ToString(*t);
		}
		catch (const std::exception) {
			value = L"end()"s;
		}
		return value;
	}
}

namespace FieaGameEngine
{
	template<>
	struct DefaultHash<Foo>
	{
		size_t operator()(const Foo& key) noexcept {
			DefaultHash<int32_t> defaultHash;
			return defaultHash(key.Data());
		}
	};

	template<>
	struct DefaultHash<Bar>
	{
		size_t operator()(const Bar& key) noexcept {
			DefaultHash<int32_t> defaultHash;
			return defaultHash(key.Data());
		}
	};
}

namespace LibraryDesktopTests
{
	TEST_CLASS(HashTests)
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

		TEST_METHOD(Hash) {
			int32_t a = 5;
			int32_t b = 5;
			int32_t c = 10;

			DefaultHash<int32_t> defaultHash;

			Assert::IsTrue(defaultHash(a) == defaultHash(b));
			Assert::IsFalse(defaultHash(a) == defaultHash(c));

			const Foo d{ 10 };
			const Foo e{ 10 };
			const Foo f{ 20 };

			DefaultHash<Foo> fooHash;

			Assert::IsTrue(fooHash(d) == fooHash(e));
			Assert::IsFalse(fooHash(d) == fooHash(f));

			std::string g = "Hello";
			std::string h = "Hello";
			std::string i = "Goodbye";

			DefaultHash<std::string> stringHash;

			Assert::IsTrue(stringHash(g) == stringHash(h));
			Assert::IsFalse(stringHash(g) == stringHash(i));
		}

		TEST_METHOD(Constructor) {
			HashMap<int32_t, Foo> map;
			Assert::AreEqual(size_t(0), map.Size());
			Assert::AreEqual(map.begin(), map.end());

			Assert::ExpectException<std::runtime_error>([&map]() {
				auto& fail = *map.begin(); UNREFERENCED_LOCAL(fail);
				});
		}

		TEST_METHOD(CopySemantics) {
			HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
			{
				HashMap<int32_t, Foo> copy(map);
				Assert::IsTrue(std::equal(copy.begin(), copy.end(), map.begin()));
			}
			{
				HashMap<int32_t, Foo> copy;
				copy = map;
				Assert::IsTrue(std::equal(copy.begin(), copy.end(), map.begin()));
			}
		}

		TEST_METHOD(MoveSemantics) {
			HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
			{
				HashMap<int32_t, Foo> temp(map);
				HashMap<int32_t, Foo> copy(std::move(temp));
				Assert::IsTrue(std::equal(copy.begin(), copy.end(), map.begin()));
			}
			{
				HashMap<int32_t, Foo> temp(map);
				HashMap<int32_t, Foo> copy;
				copy = std::move(temp);
				Assert::IsTrue(std::equal(copy.begin(), copy.end(), map.begin()));
			}
		}

		TEST_METHOD(InitializerLists) {
			{
				HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				Assert::AreEqual(size_t(3), map.Size());
				Assert::AreEqual(Foo(0), (*map.begin()).second);
				Assert::AreEqual(Foo(1), map[1]);
			}
			{
				HashMap<int32_t, Foo> map;
				map = { std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				Assert::AreEqual(size_t(3), map.Size());
				Assert::AreEqual(Foo(0), (*map.begin()).second);
				Assert::AreEqual(Foo(1), map[1]);
			}
		}

		TEST_METHOD(IteratorOperators) {
			{
				HashMap<int32_t, Foo>::Iterator iter;
				Assert::ExpectException<std::runtime_error>([&iter]() {
					++iter;
					});
				Assert::ExpectException<std::runtime_error>([&iter]() {
					iter++;
					});
				Assert::ExpectException<std::runtime_error>([&iter]() {
					auto fail = *iter; UNREFERENCED_LOCAL(fail);
					});
			}
			{
				HashMap<int32_t, Foo>::ConstIterator iter;
				Assert::ExpectException<std::runtime_error>([&iter]() {
					++iter;
					});
				Assert::ExpectException<std::runtime_error>([&iter]() {
					iter++;
					});
				Assert::ExpectException<std::runtime_error>([&iter]() {
					auto fail = *iter; UNREFERENCED_LOCAL(fail);
					});
			}
		}

		TEST_METHOD(OperatorBracketBracket) {
			{
				HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				Assert::AreEqual(Foo(0), map[0]);
				Assert::AreEqual(Foo(1), map[1]);
				Assert::AreEqual(Foo(2), map[2]);
				Assert::AreEqual(Foo(), map[3]);
			}
			{
				const HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				Assert::AreEqual(Foo(0), map[0]);
				Assert::AreEqual(Foo(1), map[1]);
				Assert::AreEqual(Foo(2), map[2]);
				Assert::ExpectException<std::runtime_error>([&map]() {
					auto& fail = map[3]; UNREFERENCED_LOCAL(fail);
					});
			}
		}

		TEST_METHOD(Insert) {
			HashMap<int32_t, Foo> map;
			Assert::AreEqual(size_t(0), map.Size());

			auto p1 = std::make_pair(10, Foo(10));
			auto result = map.Insert(p1);
			Assert::AreEqual(size_t(1), map.Size());
			Assert::IsTrue(map.ContainsKey(10));
			Assert::IsTrue(result.second);

			result = map.Insert(p1);
			Assert::AreEqual(size_t(1), map.Size());
			Assert::IsTrue(map.ContainsKey(10));
			Assert::AreEqual(result.first, map.begin());
			Assert::IsFalse(result.second);

			result = map.Insert(std::move(std::make_pair(20, Foo(20))));
			Assert::AreEqual(size_t(2), map.Size());
			Assert::IsTrue(map.ContainsKey(20));
			Assert::IsTrue(result.second);

			result = map.Insert(std::move(std::make_pair(20, Foo(20))));
			Assert::AreEqual(size_t(2), map.Size());
			Assert::IsTrue(map.ContainsKey(20));
			Assert::IsFalse(result.second);
		}

		TEST_METHOD(Remove) {
			HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };

			Assert::AreEqual(size_t(3), map.Size());
			Assert::IsTrue(map.ContainsKey(0));

			auto result = map.Remove(0);
			Assert::AreEqual(size_t(2), map.Size());
			Assert::IsFalse(map.ContainsKey(0));
			Assert::IsTrue(result);

			result = map.RemoveAt(map.begin());
			Assert::AreEqual(size_t(1), map.Size());
			Assert::IsFalse(map.ContainsKey(1));
			Assert::IsTrue(result);

			result = map.Remove(5);
			Assert::AreEqual(size_t(1), map.Size());
			Assert::IsFalse(result);

			map.Remove(2);
			Assert::AreEqual(size_t(0), map.Size());
			Assert::IsFalse(map.ContainsKey(2));

			Assert::ExpectException<std::runtime_error>([&map]() {
				map.RemoveAt(HashMap<int32_t, Foo>::Iterator());
				});
		}

		TEST_METHOD(Find) {
			{
				HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				auto iter = map.begin();

				Assert::AreEqual(iter, map.Find(0));
				Assert::AreEqual(++iter, map.Find(1));
				Assert::AreEqual(++iter, map.Find(2));
				Assert::AreEqual(map.end(), map.Find(3));
			}
			{
				const HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				auto iter = map.begin();

				Assert::AreEqual(iter, map.Find(0));
				Assert::AreEqual(++iter, map.Find(1));
				Assert::AreEqual(++iter, map.Find(2));
				Assert::AreEqual(map.end(), map.Find(3));
			}
		}

		TEST_METHOD(ContainsKey) {
			HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
			Assert::IsTrue(map.ContainsKey(0));
			Assert::IsTrue(map.ContainsKey(1));
			Assert::IsTrue(map.ContainsKey(2));
			Assert::IsFalse(map.ContainsKey(3));
		}

		TEST_METHOD(At) {
			{
				HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };

				Assert::AreEqual(Foo(0), map.At(0));
				Assert::AreEqual(Foo(1), map.At(1));
				Assert::AreEqual(Foo(2), map.At(2));
				Assert::ExpectException<std::runtime_error>([&map]() {
					auto& fail = map.At(3); UNREFERENCED_LOCAL(fail);
					});
			}
			{
				const HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				
				Assert::AreEqual(Foo(0), map.At(0));
				Assert::AreEqual(Foo(1), map.At(1));
				Assert::AreEqual(Foo(2), map.At(2));
				Assert::ExpectException<std::runtime_error>([&map]() {
					auto& fail = map.At(3); UNREFERENCED_LOCAL(fail);
					});
			}
		}

		TEST_METHOD(BeginAndEnd) {
			{
				HashMap<int32_t, Foo> map;
				Assert::AreEqual(map.begin(), map.end());
				Assert::AreEqual(map.cbegin(), map.cend());
				
				map = { std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				Assert::AreEqual(Foo(0), (*map.begin()).second);
				Assert::AreEqual(Foo(0), (*map.cbegin()).second);
				Assert::ExpectException<std::runtime_error>([&map]() {
					auto& fail = *map.end(); UNREFERENCED_LOCAL(fail);
					});
				Assert::ExpectException<std::runtime_error>([&map]() {
					auto& fail = *map.cend(); UNREFERENCED_LOCAL(fail);
					});
			}
			{
				const HashMap<int32_t, Foo> map;
				Assert::AreEqual(map.begin(), map.end());
			}
			{
				const HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };
				Assert::AreEqual(Foo(0), (*map.begin()).second);
				Assert::ExpectException<std::runtime_error>([&map]() {
					auto& fail = *map.end(); UNREFERENCED_LOCAL(fail);
					});
			}
		}

		TEST_METHOD(Iterating) {
			{
				HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };

				int count = 0;
				for (auto iter = map.begin(); iter != map.end(); ++iter, ++count) {
					;
				}

				Assert::AreEqual(3, count);

				count = 0;
				for (auto iter = map.begin(); iter != map.end(); iter++, ++count) {
					;
				}

				Assert::AreEqual(3, count);
			}
			{
				const HashMap<int32_t, Foo> map{ std::make_pair(0, Foo(0)), std::make_pair(1, Foo(1)), std::make_pair(2, Foo(2)) };

				int count = 0;
				for (auto iter = map.begin(); iter != map.end(); ++iter, ++count) {
					;
				}

				Assert::AreEqual(3, count);

				count = 0;
				for (auto iter = map.begin(); iter != map.end(); iter++, ++count) {
					;
				}

				Assert::AreEqual(3, count);
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}