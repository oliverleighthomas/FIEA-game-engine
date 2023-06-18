#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"
#include "Bar.h"
#include "ToStringSpecialization.h"

#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace FieaGameEngine {
	template<>
	struct DefaultEquality<UnitTests::Bar> final {
		bool operator()(const UnitTests::Bar& lhs, const UnitTests::Bar& rhs) const {
			return lhs.Data() == rhs.Data();
		}
	};

	template<>
	struct DefaultEquality<const UnitTests::Bar> final {
		bool operator()(const UnitTests::Bar& lhs, const UnitTests::Bar& rhs) const {
			return lhs.Data() == rhs.Data();
		}
	};
}

namespace Microsoft::VisualStudio::CppUnitTestFramework {
	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTests::Foo>::Iterator>(const FieaGameEngine::SList<UnitTests::Foo>::Iterator& t)
	{
		try {
			return ToString(*t);
		}
		catch (const std::exception) {
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTests::Bar>::Iterator>(const FieaGameEngine::SList<UnitTests::Bar>::Iterator& t)
	{
		try {
			return ToString(*t);
		}
		catch (const std::exception) {
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTests::Foo>::ConstIterator>(const FieaGameEngine::SList<UnitTests::Foo>::ConstIterator& t)
	{
		try {
			return ToString(*t);
		}
		catch (const std::exception) {
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTests::Bar>::ConstIterator>(const FieaGameEngine::SList<UnitTests::Bar>::ConstIterator& t)
	{
		try {
			return ToString(*t);
		}
		catch (const std::exception) {
			return L"end()"s;
		}
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(SListTests)
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

		TEST_METHOD(DefaultConstructor) {
			{
				const SList<Foo> list;

				Assert::AreEqual(size_t(0), list.Size());

				Assert::ExpectException<std::runtime_error>([&list]() {
					auto& front = list.Front(); UNREFERENCED_LOCAL(front);
					});
				Assert::ExpectException<std::runtime_error>([&list]() {
					auto& back = list.Back(); UNREFERENCED_LOCAL(back);
					});

				Assert::IsTrue(list.IsEmpty());
			}
			{
				const SList<Bar> list;

				Assert::AreEqual(size_t(0), list.Size());

				Assert::ExpectException<std::runtime_error>([&list]() {
					auto& front = list.Front(); UNREFERENCED_LOCAL(front);
					});
				Assert::ExpectException<std::runtime_error>([&list]() {
					auto& back = list.Back(); UNREFERENCED_LOCAL(back);
					});

				Assert::IsTrue(list.IsEmpty());
			}
		}

		TEST_METHOD(InitializerLists) {
			{
				SList<Foo> list{ Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(Foo(0), list.Front());
				Assert::AreEqual(Foo(2), list.Back());
				Assert::IsFalse(list.IsEmpty());

				list = { Foo(3), Foo(4), Foo(5) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(Foo(3), list.Front());
				Assert::AreEqual(Foo(5), list.Back());
				Assert::IsFalse(list.IsEmpty());
			}
			{
				SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(0, list.Front().Data());
				Assert::AreEqual(2, list.Back().Data());
				Assert::IsFalse(list.IsEmpty());

				list = { Bar(3), Bar(4), Bar(5) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(3, list.Front().Data());
				Assert::AreEqual(5, list.Back().Data());
				Assert::IsFalse(list.IsEmpty());
			}
		}

		TEST_METHOD(CopySemantics) {
			{
				SList<Foo> list{ Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(Foo(0), list.Front());
				Assert::AreEqual(Foo(2), list.Back());
				Assert::IsFalse(list.IsEmpty());
				{
					SList<Foo> copy{ list };
					Assert::IsTrue(std::equal(list.begin(), list.end(), copy.begin()));
				}
				{
					SList<Foo> copy;
					copy = list;
					Assert::IsTrue(std::equal(list.begin(), list.end(), copy.begin()));
				}
			}
			{
				SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(0, list.Front().Data());
				Assert::AreEqual(2, list.Back().Data());
				Assert::IsFalse(list.IsEmpty());
				{
					SList<Bar> copy{ list };
					Assert::AreEqual(size_t(3), list.Size());
					Assert::AreEqual(0, list.Front().Data());
					Assert::AreEqual(2, list.Back().Data());
					Assert::IsFalse(list.IsEmpty());
				}
				{
					SList<Bar> copy;
					copy = list;
					Assert::AreEqual(size_t(3), list.Size());
					Assert::AreEqual(0, list.Front().Data());
					Assert::AreEqual(2, list.Back().Data());
					Assert::IsFalse(list.IsEmpty());
				}
			}
		}

		TEST_METHOD(MoveSemantics) {
			{
				SList<Foo> list{ Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(Foo(0), list.Front());
				Assert::AreEqual(Foo(2), list.Back());
				Assert::IsFalse(list.IsEmpty());

				{
					SList<Foo> copy{ list };
					SList<Foo> move{ std::move(copy) };
					Assert::IsTrue(std::equal(list.begin(), list.end(), move.begin()));
				}
				{
					SList<Foo> copy{ list };
					SList<Foo> move;
					move = std::move(copy);
					Assert::IsTrue(std::equal(list.begin(), list.end(), move.begin()));
				}
			}
			{
				SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(0, list.Front().Data());
				Assert::AreEqual(2, list.Back().Data());
				Assert::IsFalse(list.IsEmpty());
				{
					SList<Bar> copy{ list };
					SList<Bar> move{ std::move(copy) };
					Assert::AreEqual(size_t(3), list.Size());
					Assert::AreEqual(0, list.Front().Data());
					Assert::AreEqual(2, list.Back().Data());
					Assert::IsFalse(list.IsEmpty());
				}
				{
					SList<Bar> copy{ list };
					SList<Bar> move;
					move = std::move(copy);
					Assert::AreEqual(size_t(3), list.Size());
					Assert::AreEqual(0, list.Front().Data());
					Assert::AreEqual(2, list.Back().Data());
					Assert::IsFalse(list.IsEmpty());
				}
			}
		}

		TEST_METHOD(IterateThroughList) {
			{
				{
					SList<Foo> list{ Foo(1), Foo(2), Foo(3) };
					int count = 0;
					for (SList<Foo>::Iterator iter = list.begin(); iter != list.end(); ++iter, ++count) {
						;
					}
					Assert::AreEqual(3, count);

					count = 0;
					for (SList<Foo>::Iterator iter = list.begin(); iter != list.end(); iter++, ++count) {
						;
					}
					Assert::AreEqual(3, count);
				}
				{
					const SList<Foo> list{ Foo(1), Foo(2), Foo(3) };
					int count = 0;
					for (SList<Foo>::ConstIterator iter = list.begin(); iter != list.end(); ++iter, ++count) {
						;
					}
					Assert::AreEqual(3, count);

					count = 0;
					for (SList<Foo>::ConstIterator iter = list.begin(); iter != list.end(); iter++, ++count) {
						;
					}
					Assert::AreEqual(3, count);
				}
			}
			{
				{
					SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
					int count = 0;
					for (SList<Bar>::Iterator iter = list.begin(); iter != list.end(); ++iter, ++count) {
						;
					}
					Assert::AreEqual(3, count);

					count = 0;
					for (SList<Bar>::Iterator iter = list.begin(); iter != list.end(); iter++, ++count) {
						;
					}
					Assert::AreEqual(3, count);
				}
				{
					const SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
					int count = 0;
					for (SList<Bar>::ConstIterator iter = list.begin(); iter != list.end(); ++iter, ++count) {
						;
					}
					Assert::AreEqual(3, count);

					count = 0;
					for (SList<Bar>::ConstIterator iter = list.begin(); iter != list.end(); iter++, ++count) {
						;
					}
					Assert::AreEqual(3, count);
				}
			}
		}

		TEST_METHOD(IteratorOperators) {
			{
				SList<int>::Iterator iter;
				Assert::ExpectException<std::runtime_error>([&iter]() {
					++iter;
					});
				Assert::ExpectException<std::runtime_error>([&iter]() {
					iter++;
					});
				Assert::ExpectException<std::runtime_error>([&iter]() {
					auto& fail = *iter; UNREFERENCED_LOCAL(fail);
					});
			}
			{
				SList<int>::ConstIterator iter;
				Assert::ExpectException<std::runtime_error>([&iter]() {
					++iter;
					});
				Assert::ExpectException<std::runtime_error>([&iter]() {
					iter++;
					});
				Assert::ExpectException<std::runtime_error>([&iter]() {
					auto& fail = *iter; UNREFERENCED_LOCAL(fail);
					});
			}
		}

		TEST_METHOD(PushFront) {
			{
				SList<Foo> list;

				Assert::AreEqual(size_t(0), list.Size());
				Assert::IsTrue(list.IsEmpty());

				Foo foo = Foo(1);
				list.PushFront(foo);

				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(foo, list.Front());
				Assert::AreEqual(foo, list.Back());

				foo = Foo(0);
				list.PushFront(std::move(foo));

				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(Foo(0), list.Front());
				Assert::AreEqual(Foo(1), list.Back());
			}
			{
				SList<Bar> list;

				Assert::AreEqual(size_t(0), list.Size());
				Assert::IsTrue(list.IsEmpty());

				Bar bar = Bar(1);
				list.PushFront(bar);

				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(bar.Data(), list.Front().Data());
				Assert::AreEqual(bar.Data(), list.Back().Data());

				bar = Bar(0);
				list.PushFront(std::move(bar));

				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(0, list.Front().Data());
				Assert::AreEqual(1, list.Back().Data());
			}
		}

		TEST_METHOD(PushBack) {
			{
				SList<Foo> list{ Foo(0), Foo(1) };

				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(Foo(1), list.Back());

				Foo foo = Foo(2);
				list.PushBack(foo);

				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(foo, list.Back());

				foo = Foo(3);
				list.PushBack(std::move(foo));

				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(Foo(3), list.Back());
			}
			{
				SList<Bar> list{ Bar(0), Bar(1) };

				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(1, list.Back().Data());

				Bar bar = Bar(2);
				list.PushBack(bar);

				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(bar.Data(), list.Back().Data());

				bar = Bar(3);
				list.PushBack(std::move(bar));

				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(3, list.Back().Data());
			}
		}

		TEST_METHOD(InsertAfter) {
			{
				SList<Foo> list{ Foo(0), Foo(1), Foo(2) };

				Assert::AreEqual(size_t(3), list.Size());

				Foo foo = Foo(3);
				list.InsertAfter(foo, list.begin());

				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(Foo(0), list.Front());
				list.PopFront();
				Assert::AreEqual(foo, list.Front());

				foo = Foo(4);
				list.InsertAfter(std::move(foo), list.begin());

				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(Foo(3), list.Front());
				list.PopFront();
				Assert::AreEqual(Foo(4), list.Front());

				Assert::AreEqual(Foo(2), list.Back());
				list.InsertAfter(Foo(5), list.Find(Foo(2)));
				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(Foo(5), list.Back());

				list.InsertAfter(Foo(6), list.end());
				Assert::AreEqual(size_t(5), list.Size());
				Assert::AreEqual(Foo(6), list.Back());

				Assert::ExpectException<std::runtime_error>([&list]() {
					SList<Foo>::Iterator iter;
					list.InsertAfter(Foo(7), iter);
					});
			}
			{
				SList<Bar> list{ Bar(0), Bar(1), Bar(2) };

				Assert::AreEqual(size_t(3), list.Size());

				Bar bar = Bar(3);
				list.InsertAfter(bar, list.begin());

				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(0, list.Front().Data());
				list.PopFront();
				Assert::AreEqual(bar.Data(), list.Front().Data());

				bar = Bar(4);
				list.InsertAfter(std::move(bar), list.begin());

				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(3, list.Front().Data());
				list.PopFront();
				Assert::AreEqual(4, list.Front().Data());

				Assert::AreEqual(2, list.Back().Data());
				list.InsertAfter(Bar(5), list.Find(Bar(2)));
				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(5, list.Back().Data());

				list.InsertAfter(Bar(6), list.end());
				Assert::AreEqual(size_t(5), list.Size());
				Assert::AreEqual(6, list.Back().Data());

				Assert::ExpectException<std::runtime_error>([&list]() {
					SList<Bar>::Iterator iter;
					list.InsertAfter(Bar(7), iter);
					});
			}
		}

		TEST_METHOD(PopFront) {
			{
				SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list]() {
					list.PopFront();
					});

				list = { Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(Foo(0), list.Front());

				list.PopFront();
				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(Foo(1), list.Front());

				list.PopFront();
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(Foo(2), list.Front());

				list.PopFront();
				Assert::AreEqual(size_t(0), list.Size());
			}
			{
				SList<Bar> list;
				Assert::ExpectException<std::runtime_error>([&list]() {
					list.PopFront();
					});

				list = { Bar(0), Bar(1), Bar(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(0, list.Front().Data());

				list.PopFront();
				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(1, list.Front().Data());

				list.PopFront();
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(2, list.Front().Data());

				list.PopFront();
				Assert::AreEqual(size_t(0), list.Size());
			}
		}

		TEST_METHOD(PopBack) {
			{
				SList<Foo> list{ Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(Foo(2), list.Back());

				list.PopBack();
				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(Foo(1), list.Back());

				list.PopBack();
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(Foo(0), list.Back());

				list.PopBack();
				Assert::AreEqual(size_t(0), list.Size());
			}
			{
				SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(2, list.Back().Data());

				list.PopBack();
				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(1, list.Back().Data());

				list.PopBack();
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(0, list.Back().Data());

				list.PopBack();
				Assert::AreEqual(size_t(0), list.Size());
			}
		}

		TEST_METHOD(Removal) {
			{
				SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list]() {
					SList<Foo>::Iterator iter;
					list.RemoveAt(iter);
					});

				Assert::IsFalse(list.RemoveAt(list.end()));

				list = { Foo(0), Foo(1), Foo(2) };

				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(Foo(0), list.Front());

				list.Remove(Foo(0));

				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(Foo(1), list.Front());

				list.RemoveAt(list.begin());

				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(Foo(2), list.Front());

				list.RemoveAt(list.begin());
				Assert::AreEqual(size_t(0), list.Size());
				Assert::IsTrue(list.IsEmpty());
			}
			{
				SList<Bar> list;
				Assert::ExpectException<std::runtime_error>([&list]() {
					SList<Bar>::Iterator iter;
					list.RemoveAt(iter);
					});

				Assert::IsFalse(list.RemoveAt(list.end()));

				list = { Bar(0), Bar(1), Bar(2) };

				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(0, list.Front().Data());

				list.Remove(Bar(0));

				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(1, list.Front().Data());

				list.RemoveAt(list.begin());

				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(2, list.Front().Data());

				list.RemoveAt(list.begin());
				Assert::AreEqual(size_t(0), list.Size());
				Assert::IsTrue(list.IsEmpty());
			}
		}

		TEST_METHOD(Find) {
			{
				{
					SList<Foo> list{ Foo(0), Foo(1), Foo(2) };

					SList<Foo>::Iterator iter = list.begin();

					Assert::AreEqual(list.Find(Foo(0)), iter);
					Assert::AreEqual(*list.Find(Foo(0)), Foo(0));

					Assert::AreEqual(list.Find(Foo(1)), ++iter);
					Assert::AreEqual(*list.Find(Foo(1)), Foo(1));

					Assert::AreEqual(list.Find(Foo(2)), ++iter);
					Assert::AreEqual(*list.Find(Foo(2)), Foo(2));
				}
				{
					const SList<Foo> list{ Foo(0), Foo(1), Foo(2) };

					SList<Foo>::ConstIterator iter = list.begin();

					Assert::AreEqual(list.Find(Foo(0)), iter);
					Assert::AreEqual(*list.Find(Foo(0)), Foo(0));

					Assert::AreEqual(list.Find(Foo(1)), ++iter);
					Assert::AreEqual(*list.Find(Foo(1)), Foo(1));

					Assert::AreEqual(list.Find(Foo(2)), ++iter);
					Assert::AreEqual(*list.Find(Foo(2)), Foo(2));
				}
			}
			{
				{
					SList<Bar> list{ Bar(0), Bar(1), Bar(2) };

					SList<Bar>::Iterator iter = list.begin();

					Assert::AreEqual(list.Find(Bar(0)), iter);
					Assert::AreEqual((*list.Find(Bar(0))).Data(), 0);

					Assert::AreEqual(list.Find(Bar(1)), ++iter);
					Assert::AreEqual((*list.Find(Bar(1))).Data(), 1);

					Assert::AreEqual(list.Find(Bar(2)), ++iter);
					Assert::AreEqual((*list.Find(Bar(2))).Data(), 2);
				}
				{
					const SList<Bar> list{ Bar(0), Bar(1), Bar(2) };

					SList<Bar>::ConstIterator iter = list.begin();

					Assert::AreEqual(list.Find(Bar(0)), iter);
					Assert::AreEqual((*list.Find(Bar(0))).Data(), 0);

					Assert::AreEqual(list.Find(Bar(1)), ++iter);
					Assert::AreEqual((*list.Find(Bar(1))).Data(), 1);

					Assert::AreEqual(list.Find(Bar(2)), ++iter);
					Assert::AreEqual((*list.Find(Bar(2))).Data(), 2);
				}
			}
		}

		TEST_METHOD(FrontAndBack) {
			{
				{
					SList<Foo> list;
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& front = list.Front(); UNREFERENCED_LOCAL(front);
						});
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& back = list.Back(); UNREFERENCED_LOCAL(back);
						});

					list = { Foo(0), Foo(1), Foo(2) };
					Assert::AreEqual(Foo(0), list.Front());
					Assert::AreEqual(Foo(2), list.Back());
				}
				{
					const SList<Foo> fail;
					Assert::ExpectException<std::runtime_error>([&fail]() {
						auto& front = fail.Front(); UNREFERENCED_LOCAL(front);
						});
					Assert::ExpectException<std::runtime_error>([&fail]() {
						auto& back = fail.Back(); UNREFERENCED_LOCAL(back);
						});

					const SList<Foo> list{ Foo(0), Foo(1), Foo(2) };
					Assert::AreEqual(Foo(0), list.Front());
					Assert::AreEqual(Foo(2), list.Back());
				}
			}
			{
				{
					SList<Bar> list;
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& front = list.Front(); UNREFERENCED_LOCAL(front);
						});
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& back = list.Back(); UNREFERENCED_LOCAL(back);
						});

					list = { Bar(0), Bar(1), Bar(2) };
					Assert::AreEqual(0, list.Front().Data());
					Assert::AreEqual(2, list.Back().Data());
				}
				{
					const SList<Bar> fail;
					Assert::ExpectException<std::runtime_error>([&fail]() {
						auto& front = fail.Front(); UNREFERENCED_LOCAL(front);
						});
					Assert::ExpectException<std::runtime_error>([&fail]() {
						auto& back = fail.Back(); UNREFERENCED_LOCAL(back);
						});

					const SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
					Assert::AreEqual(0, list.Front().Data());
					Assert::AreEqual(2, list.Back().Data());
				}
			}
		}

		TEST_METHOD(BeginAndEnd) {
			{
				{
					SList<Foo> list{ Foo(0), Foo(1), Foo(2) };

					Assert::AreEqual(Foo(0), *list.begin());
					Assert::AreEqual(Foo(0), *list.cbegin());
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& fail = *list.end(); UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& fail = *list.cend(); UNREFERENCED_LOCAL(fail);
						});
				}
				{
					const SList<Foo> list{ Foo(0), Foo(1), Foo(2) };
					Assert::AreEqual(Foo(0), *list.begin());
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& fail = *list.end(); UNREFERENCED_LOCAL(fail);
						});
				}
			}
			{
				{
					SList<Bar> list{ Bar(0), Bar(1), Bar(2) };

					Assert::AreEqual(0, (*list.begin()).Data());
					Assert::AreEqual(0, (*list.cbegin()).Data());
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& fail = *list.end(); UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& fail = *list.cend(); UNREFERENCED_LOCAL(fail);
						});
				}
				{
					const SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
					Assert::AreEqual(0, (*list.begin()).Data());
					Assert::ExpectException<std::runtime_error>([&list]() {
						auto& fail = *list.end(); UNREFERENCED_LOCAL(fail);
						});
				}
			}
		}

		TEST_METHOD(Clear) {
			{
				SList<Foo> list{ Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(Foo(0), list.Front());
				Assert::AreEqual(Foo(2), list.Back());
				Assert::IsFalse(list.IsEmpty());

				list.Clear();
				Assert::AreEqual(size_t(0), list.Size());
				Assert::ExpectException<std::runtime_error>([&list]() {
					auto& front = list.Front(); UNREFERENCED_LOCAL(front);
					});
				Assert::ExpectException<std::runtime_error>([&list]() {
					auto& back = list.Back(); UNREFERENCED_LOCAL(back);
					});
				Assert::IsTrue(list.IsEmpty());
			}
			{
				SList<Bar> list{ Bar(0), Bar(1), Bar(2) };
				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(0, list.Front().Data());
				Assert::AreEqual(2, list.Back().Data());
				Assert::IsFalse(list.IsEmpty());

				list.Clear();
				Assert::AreEqual(size_t(0), list.Size());
				Assert::ExpectException<std::runtime_error>([&list]() {
					auto& front = list.Front(); UNREFERENCED_LOCAL(front);
					});
				Assert::ExpectException<std::runtime_error>([&list]() {
					auto& back = list.Back(); UNREFERENCED_LOCAL(back);
					});
				Assert::IsTrue(list.IsEmpty());
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
