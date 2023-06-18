#include "pch.h"
//#include "CppUnitTest.h"
//#include "Foo.h"
//#include "Bar.h"
//#include "HashMap.h"
//#include "ToStringSpecialization.h"
//#include "DefaultHash.h"
//#include "DefaultEquality.h"
//
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//using namespace UnitTests;
//using namespace std;
//using namespace std::string_literals;
//using namespace FieaGameEngine;
//
//namespace Microsoft::VisualStudio::CppUnitTestFramework {
//	template<>
//	inline std::wstring ToString<std::pair<const int32_t, UnitTests::Foo>>(const std::pair <const int32_t, UnitTests::Foo>& t)
//	{
//		std::wstring value;
//		try {
//			value = ToString(t.first);
//			value += ToString(" ");
//			value += ToString(t.second);
//		}
//		catch (const std::exception) {
//			value = L"end()"s;
//		}
//		return value;
//	}
//
//	template<>
//	inline std::wstring ToString<FieaGameEngine::HashMap<int32_t, UnitTests::Foo>::Iterator>(const FieaGameEngine::HashMap<int32_t, UnitTests::Foo>::Iterator& t)
//	{
//		std::wstring value;
//		try {
//			value = ToString(*t);
//		}
//		catch (const std::exception) {
//			value = L"end()"s;
//		}
//		return value;
//	}
//
//	template<>
//	inline std::wstring ToString<FieaGameEngine::HashMap<int32_t, UnitTests::Foo>::ConstIterator>(const FieaGameEngine::HashMap<int32_t, UnitTests::Foo>::ConstIterator& t)
//	{
//		std::wstring value;
//		try {
//			value = ToString(*t);
//		}
//		catch (const std::exception) {
//			value = L"end()"s;
//		}
//		return value;
//	}
//}
//
//namespace FieaGameEngine
//{
//	template<>
//	struct DefaultHash<Foo>
//	{
//		size_t operator()(const Foo& key) noexcept {
//			DefaultHash<int32_t> defaultHash;
//			return defaultHash(key.Data());
//		}
//	};
//
//	template<>
//	struct DefaultHash<Bar>
//	{
//		size_t operator()(const Bar& key) noexcept {
//			DefaultHash<int32_t> defaultHash;
//			return defaultHash(key.Data());
//		}
//	};
//}
//
//namespace LibraryDesktopTests
//{
//	TEST_CLASS(HashTests)
//	{
//	public:
//		TEST_METHOD_INITIALIZE(Initialize)
//		{
//#if defined(DEBUG) || defined(_DEBUG)
//			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
//			_CrtMemCheckpoint(&_startMemState);
//#endif
//		}
//
//		TEST_METHOD_CLEANUP(Cleanup)
//		{
//#if defined(DEBUG) || defined(_DEBUG)
//			_CrtMemState endMemState, diffMemState;
//			_CrtMemCheckpoint(&endMemState);
//			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
//			{
//				_CrtMemDumpStatistics(&diffMemState);
//				Assert::Fail(L"Memory Leaks!");
//			}
//#endif
//		}
//
//		TEST_METHOD(Hash) {
//			int32_t a = 5;
//			int32_t b = 5;
//			int32_t c = 10;
//			
//			DefaultHash<int32_t> defaultHash;
//
//			Assert::IsTrue(defaultHash(a) == defaultHash(b));
//			Assert::IsFalse(defaultHash(a) == defaultHash(c));
//
//			const Foo d{ 10 };
//			const Foo e{ 10 };
//			const Foo f{ 20 };
//
//			DefaultHash<Foo> fooHash;
//
//			Assert::IsTrue(fooHash(d) == fooHash(e));
//			Assert::IsFalse(fooHash(d) == fooHash(f));
//
//			std::string g = "Hello";
//			std::string h = "Hello";
//			std::string i = "Goodbye";
//
//			DefaultHash<std::string> stringHash;
//
//			Assert::IsTrue(stringHash(g) == stringHash(h));
//			Assert::IsFalse(stringHash(g) == stringHash(i));
//		}
//
//		TEST_METHOD(Constructor) {
//
//			HashMap<int32_t, Foo> map;
//			
//			Assert::AreEqual(size_t(0), map.Size());
//		}
//
//		TEST_METHOD(Find) {
//			const Foo a{ 10 };
//			const Foo b{ 20 };
//			const Foo c{ 30 };
//
//			HashMap<int32_t, Foo> map{ std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c) };
//
//			Assert::AreEqual(a, (*map.Find(10)).second);
//			Assert::AreEqual(b, (*map.Find(20)).second);
//			Assert::AreEqual(c, (*map.Find(30)).second);
//
//			Assert::AreEqual(map.end(), map.Find(40));
//
//			const HashMap<int32_t, Foo> cMap{ std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c) };
//
//			Assert::AreEqual(a, (*cMap.Find(10)).second);
//			Assert::AreEqual(b, (*cMap.Find(20)).second);
//			Assert::AreEqual(c, (*cMap.Find(30)).second);
//
//			Assert::AreEqual(cMap.end(), cMap.Find(40));
//		}
//
//		TEST_METHOD(Insert) {
//			const Foo a{ 10 };
//			const Foo b{ 20 };
//			const Foo c{ 30 };
//
//			HashMap<int32_t, Foo> map;
//
//			Assert::AreEqual(size_t(0), map.Size());
//
//			map.Insert(std::pair<int32_t, Foo>(10, a));
//
//			Assert::AreEqual(size_t(1), map.Size());
//			Assert::IsTrue(map.ContainsKey(10));
//
//			map.Insert(std::pair<int32_t, Foo>(20, b));
//
//			Assert::AreEqual(size_t(2), map.Size());
//			Assert::IsTrue(map.ContainsKey(20));
//
//			map.Insert(std::pair<int32_t, Foo>(30, c));
//
//			Assert::AreEqual(size_t(3), map.Size());
//			Assert::IsTrue(map.ContainsKey(30));
//		}
//
//		TEST_METHOD(BracketOperator) {
//			const Foo a{ 10 };
//			const Foo b{ 20 };
//
//			HashMap<int32_t, Foo> map{ std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b) };
//
//			Assert::AreEqual(a, map[10]);
//			Assert::AreEqual(b, map[20]);
//			Assert::AreEqual(size_t(2), map.Size());
//			Assert::IsFalse(map.ContainsKey(30));
//
//			Assert::AreEqual(Foo(), map[30]);
//			Assert::AreEqual(size_t(3), map.Size());
//			Assert::IsTrue(map.ContainsKey(30));
//		}
//
//		TEST_METHOD(Remove) {
//			const Foo a{ 10 };
//			const Foo b{ 20 };
//			const Foo c{ 30 };
//
//			HashMap<int32_t, Foo> map{ std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c) };
//			Assert::AreEqual(size_t(3), map.Size());
//
//			map.Remove(10);
//			Assert::AreEqual(size_t(2), map.Size());
//			Assert::IsFalse(map.ContainsKey(10));
//
//			map.Remove(20);
//			Assert::AreEqual(size_t(1), map.Size());
//			Assert::IsFalse(map.ContainsKey(20));
//
//			map.Remove(30);
//			Assert::AreEqual(size_t(0), map.Size());
//			Assert::IsFalse(map.ContainsKey(30));
//		}
//
//		TEST_METHOD(Clear) {
//			const Foo a{ 10 };
//			const Foo b{ 20 };
//			const Foo c{ 30 };
//
//			HashMap<int32_t, Foo> map{ std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c) };
//			Assert::AreEqual(size_t(3), map.Size());
//
//			map.Clear();
//
//			Assert::AreEqual(size_t(0), map.Size());
//		}
//
//		TEST_METHOD(At) {
//			const Foo a{ 10 };
//			const Foo b{ 20 };
//			const Foo c{ 30 };
//
//			HashMap<int32_t, Foo> map{ std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c) };
//
//			Assert::AreEqual(a, map.At(10));
//			Assert::AreEqual(b, map.At(20));
//			Assert::AreEqual(c, map.At(30));
//
//			Assert::ExpectException<std::runtime_error>([&map]() {
//				auto& data = map.At(40); UNREFERENCED_LOCAL(data);
//				});
//
//			const HashMap<int32_t, Foo> cMap{std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c)};
//
//			Assert::AreEqual(a, cMap.At(10));
//			Assert::AreEqual(b, cMap.At(20));
//			Assert::AreEqual(c, cMap.At(30));
//
//			Assert::ExpectException<std::runtime_error>([&cMap]() {
//				auto& data = cMap.At(40); UNREFERENCED_LOCAL(data);
//				});
//		}
//
//		TEST_METHOD(BeginAndEnd) {
//			const Foo a{ 10 };
//			const Foo b{ 20 };
//			const Foo c{ 30 };
//
//			HashMap<int32_t, Foo> map;
//			const HashMap<int32_t, Foo> cMap;
//
//			Assert::AreEqual(map.begin(), map.end());
//			Assert::AreEqual(cMap.begin(), cMap.end());
//			Assert::AreEqual(map.cbegin(), map.cend());
//			
//			HashMap<int32_t, Foo> mapFilled { std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c) };
//			const HashMap<int32_t, Foo> cMapFilled { std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c) };
//
//			Assert::AreNotEqual(mapFilled.begin(), mapFilled.end());
//			Assert::AreNotEqual(cMapFilled.begin(), cMapFilled.end());
//			Assert::AreNotEqual(mapFilled.cbegin(), mapFilled.cend());
//		}
//
//		TEST_METHOD(Iterating) {
//			HashMap<int32_t, Foo>::Iterator emptyIter;
//			HashMap<int32_t, Foo>::ConstIterator emptyCIter;
//
//			Assert::ExpectException<std::runtime_error>([&emptyIter]() {
//				++emptyIter;
//				});
//
//			Assert::ExpectException<std::runtime_error>([&emptyCIter]() {
//				++emptyCIter;
//				});
//
//			Assert::ExpectException<std::runtime_error>([&emptyIter]() {
//				*emptyIter;
//				});
//
//			Assert::ExpectException<std::runtime_error>([&emptyCIter]() {
//				*emptyCIter;
//				});
//
//
//			const Foo a{ 10 };
//			const Foo b{ 20 };
//			const Foo c{ 30 };
//
//			HashMap<int32_t, Foo> map{ std::pair<int32_t, Foo>(10, a), std::pair<int32_t, Foo>(20, b), std::pair<int32_t, Foo>(30, c) };
//
//			int32_t count = 0;
//
//			for (auto iter = map.begin(); iter != map.end(); ++iter, ++count) {
//				;
//			}
//
//			Assert::AreEqual(3, count);
//
//			count = 0;
//
//			for (auto iter = map.begin(); iter != map.end(); iter++, ++count) {
//				;
//			}
//
//			Assert::AreEqual(3, count);
//
//			count = 0;
//
//			for (auto iter = map.cbegin(); iter != map.cend(); ++iter, ++count) {
//				;
//			}
//
//			Assert::AreEqual(3, count);
//
//			count = 0;
//
//			for (auto iter = map.cbegin(); iter != map.cend(); iter++, ++count) {
//				;
//			}
//
//			Assert::AreEqual(3, count);
//
//		}
//
//		
//	private:
//		inline static _CrtMemState _startMemState;
//	};
//}