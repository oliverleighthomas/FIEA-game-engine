#include "pch.h"
#include "CppUnitTest.h"
#include "Vector.h"
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
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTests::Foo>::Iterator>(const FieaGameEngine::Vector<UnitTests::Foo>::Iterator& t)
	{
		std::wstring value;
		try {
			value = ToString(*t);
		}
		catch (const std::exception&) {
			value = L"end()"s;
		}
		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTests::Bar>::Iterator>(const FieaGameEngine::Vector<UnitTests::Bar>::Iterator& t)
	{
		std::wstring value;
		try {
			value = ToString(*t);
		}
		catch (const std::exception&) {
			value = L"end()"s;
		}
		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTests::Foo>::ConstIterator>(const FieaGameEngine::Vector<UnitTests::Foo>::ConstIterator& t)
	{
		std::wstring value;
		try {
			value = ToString(*t);
		}
		catch (const std::exception&) {
			value = L"end()"s;
		}
		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTests::Bar>::ConstIterator>(const FieaGameEngine::Vector<UnitTests::Bar>::ConstIterator& t)
	{
		std::wstring value;
		try {
			value = ToString(*t);
		}
		catch (const std::exception&) {
			value = L"end()"s;
		}
		return value;
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(VectorTests)
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

		TEST_METHOD(Constructor)
		{
			{
				{
					Vector<Foo> vector;
					Assert::AreEqual(size_t(0), vector.Size());
					Assert::AreEqual(size_t(0), vector.Capacity());
					Assert::IsTrue(vector.IsEmpty());
					Assert::AreEqual(vector.begin(), vector.end());
				}
				{
					Vector<Foo> vector(10);
					Assert::AreEqual(size_t(0), vector.Size());
					Assert::AreEqual(size_t(10), vector.Capacity());
					Assert::IsTrue(vector.IsEmpty());
					Assert::AreEqual(vector.begin(), vector.end());
				}
			}
			{
				{
					Vector<Bar> vector;
					Assert::AreEqual(size_t(0), vector.Size());
					Assert::AreEqual(size_t(0), vector.Capacity());
					Assert::IsTrue(vector.IsEmpty());
					Assert::AreEqual(vector.begin(), vector.end());
				}
				{
					Vector<Bar> vector(10);
					Assert::AreEqual(size_t(0), vector.Size());
					Assert::AreEqual(size_t(10), vector.Capacity());
					Assert::IsTrue(vector.IsEmpty());
					Assert::AreEqual(vector.begin(), vector.end());
				}
			}
		}

		TEST_METHOD(CopySemantics) {
			{
				Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
				{
					Vector<Foo> copy(vector);
					Assert::IsTrue(std::equal(copy.begin(), copy.end(), vector.begin()));
				}
				{
					Vector<Foo> copy;
					copy = vector;
					Assert::IsTrue(std::equal(copy.begin(), copy.end(), vector.begin()));
				}
			}
			{
				Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };
				{
					Vector<Bar> copy(vector);
					for (size_t i = 0; i < copy.Size(); ++i) {
						Assert::AreEqual(vector[i].Data(), copy[i].Data());
					}
					Assert::AreEqual(vector.Size(), copy.Size());
					Assert::AreEqual(vector.Capacity(), copy.Capacity());
				}
				{
					Vector<Bar> copy;
					copy = vector;
					for (size_t i = 0; i < copy.Size(); ++i) {
						Assert::AreEqual(vector[i].Data(), copy[i].Data());
					}
					Assert::AreEqual(vector.Size(), copy.Size());
					Assert::AreEqual(vector.Capacity(), copy.Capacity());
				}
			}
		}

		TEST_METHOD(MoveSemantics) {
			{
				Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
				{
					Vector<Foo> temp(vector);
					Vector<Foo> copy(std::move(temp));
					Assert::IsTrue(std::equal(copy.begin(), copy.end(), vector.begin()));
				}
				{
					Vector<Foo> temp(vector);
					Vector<Foo> copy;
					copy = std::move(temp);
					Assert::IsTrue(std::equal(copy.begin(), copy.end(), vector.begin()));
				}
			}
			{
				Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };
				{
					Vector<Bar> temp(vector);
					Vector<Bar> copy(std::move(temp));
					for (size_t i = 0; i < copy.Size(); ++i) {
						Assert::AreEqual(vector[i].Data(), copy[i].Data());
					}
					Assert::AreEqual(vector.Size(), copy.Size());
					Assert::AreEqual(vector.Capacity(), copy.Capacity());
				}
				{
					Vector<Bar> temp(vector);
					Vector<Bar> copy;
					copy = std::move(temp);
					for (size_t i = 0; i < copy.Size(); ++i) {
						Assert::AreEqual(vector[i].Data(), copy[i].Data());
					}
					Assert::AreEqual(vector.Size(), copy.Size());
					Assert::AreEqual(vector.Capacity(), copy.Capacity());
				}
			}
		}

		TEST_METHOD(InitializerLists) {
			{
				{
					Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
					Assert::AreEqual(size_t(3), vector.Size());
					Assert::AreEqual(Foo(10), vector.Front());
					Assert::AreEqual(Foo(30), vector.Back());
				}
				{
					Vector<Foo> vector;
					vector = { Foo(10), Foo(20), Foo(30) };
					Assert::AreEqual(size_t(3), vector.Size());
					Assert::AreEqual(Foo(10), vector.Front());
					Assert::AreEqual(Foo(30), vector.Back());
				}
			}
			{
				{
					Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };
					Assert::AreEqual(size_t(3), vector.Size());
					Assert::AreEqual(10, vector.Front().Data());
					Assert::AreEqual(30, vector.Back().Data());
				}
				{
					Vector<Bar> vector;
					vector = { Bar(10), Bar(20), Bar(30) };
					Assert::AreEqual(size_t(3), vector.Size());
					Assert::AreEqual(10, vector.Front().Data());
					Assert::AreEqual(30, vector.Back().Data());
				}
			}
		}

		TEST_METHOD(IteratorOperators) {
			{
				Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
				Vector<Foo> differentVector{ Foo(10), Foo(20), Foo(30) };
				{
					Vector<Foo>::Iterator iterOne = vector.begin();
					Vector<Foo>::Iterator iterTwo = differentVector.begin();

					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						bool fail = iterOne > iterTwo; UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						bool fail = iterOne >= iterTwo; UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						bool fail = iterOne < iterTwo; UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						bool fail = iterOne <= iterTwo; UNREFERENCED_LOCAL(fail);
						});

					iterTwo = vector.end();

					Assert::IsFalse(iterOne > iterTwo);
					Assert::IsFalse(iterOne >= iterTwo);
					Assert::IsTrue(iterOne < iterTwo);
					Assert::IsTrue(iterOne <= iterTwo);
				}
				{
					Vector<Foo>::Iterator iter;
					Assert::ExpectException<std::runtime_error>([&iter] {
						iter++;
						});
					Assert::ExpectException<std::runtime_error>([&iter] {
						++iter;
						});
					Assert::ExpectException<std::runtime_error>([&iter] {
						iter--;
						});
					Assert::ExpectException<std::runtime_error>([&iter] {
						--iter;
						});

					iter = vector.begin();
					Assert::AreEqual(Foo(10), *iter);
					iter++;
					Assert::AreEqual(Foo(20), *iter);
					++iter;
					Assert::AreEqual(Foo(30), *iter);
					--iter;
					Assert::AreEqual(Foo(20), *iter);
					iter--;
					Assert::AreEqual(Foo(10), *iter);
				}
				{
					Vector<Foo>::Iterator iterOne;
					Vector<Foo>::Iterator iterTwo = differentVector.begin();
					Assert::ExpectException<std::runtime_error>([&iterOne] {
						iterOne += 2;
						});
					Assert::ExpectException<std::runtime_error>([&iterOne] {
						iterOne -= 2;
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						iterOne + iterTwo;
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						iterOne - iterTwo;
						});

					iterOne = vector.begin();
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						iterOne + iterTwo;
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						iterOne - iterTwo;
						});

					iterTwo = vector.begin();

					Assert::AreEqual(Foo(10), *iterOne);
					iterOne += 2;
					Assert::AreEqual(Foo(30), *iterOne);
					iterOne -= 2;
					Assert::AreEqual(Foo(10), *iterOne);

					++iterTwo;
					auto count = iterOne + iterTwo;
					Assert::AreEqual(ptrdiff_t(1), count);
					count = iterOne - iterTwo;
					Assert::AreEqual(ptrdiff_t(-1), count);
				}
				{
					Vector<Foo>::Iterator iter;
					Assert::ExpectException<std::runtime_error>([&iter] {
						auto& fail = *iter; UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&iter] {
						auto& fail = iter[1]; UNREFERENCED_LOCAL(fail);
						});
					iter = vector.end();
					Assert::ExpectException<std::runtime_error>([&iter] {
						auto& fail = *iter; UNREFERENCED_LOCAL(fail);
						});

					Assert::AreEqual(Foo(10), iter[0]);
				}
			}
			{
				const Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
				const Vector<Foo> differentVector{ Foo(10), Foo(20), Foo(30) };
				{
					Vector<Foo>::ConstIterator iterOne = vector.begin();
					Vector<Foo>::ConstIterator iterTwo = differentVector.begin();

					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						bool fail = iterOne > iterTwo; UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						bool fail = iterOne >= iterTwo; UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						bool fail = iterOne < iterTwo; UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						bool fail = iterOne <= iterTwo; UNREFERENCED_LOCAL(fail);
						});

					iterTwo = vector.end();

					Assert::IsFalse(iterOne > iterTwo);
					Assert::IsFalse(iterOne >= iterTwo);
					Assert::IsTrue(iterOne < iterTwo);
					Assert::IsTrue(iterOne <= iterTwo);
				}
				{
					Vector<Foo>::ConstIterator iter;
					Assert::ExpectException<std::runtime_error>([&iter] {
						iter++;
						});
					Assert::ExpectException<std::runtime_error>([&iter] {
						++iter;
						});
					Assert::ExpectException<std::runtime_error>([&iter] {
						iter--;
						});
					Assert::ExpectException<std::runtime_error>([&iter] {
						--iter;
						});

					iter = vector.begin();
					Assert::AreEqual(Foo(10), *iter);
					iter++;
					Assert::AreEqual(Foo(20), *iter);
					++iter;
					Assert::AreEqual(Foo(30), *iter);
					--iter;
					Assert::AreEqual(Foo(20), *iter);
					iter--;
					Assert::AreEqual(Foo(10), *iter);
				}
				{
					Vector<Foo>::ConstIterator iterOne;
					Vector<Foo>::ConstIterator iterTwo = differentVector.begin();
					Assert::ExpectException<std::runtime_error>([&iterOne] {
						iterOne += 2;
						});
					Assert::ExpectException<std::runtime_error>([&iterOne] {
						iterOne -= 2;
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						iterOne + iterTwo;
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						iterOne - iterTwo;
						});

					iterOne = vector.begin();
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						iterOne + iterTwo;
						});
					Assert::ExpectException<std::runtime_error>([&iterOne, &iterTwo] {
						iterOne - iterTwo;
						});

					iterTwo = vector.begin();

					Assert::AreEqual(Foo(10), *iterOne);
					iterOne += 2;
					Assert::AreEqual(Foo(30), *iterOne);
					iterOne -= 2;
					Assert::AreEqual(Foo(10), *iterOne);

					++iterTwo;
					auto count = iterOne + iterTwo;
					Assert::AreEqual(ptrdiff_t(1), count);
					count = iterOne - iterTwo;
					Assert::AreEqual(ptrdiff_t(-1), count);
				}
				{
					Vector<Foo>::ConstIterator iter;
					Assert::ExpectException<std::runtime_error>([&iter] {
						auto& fail = *iter; UNREFERENCED_LOCAL(fail);
						});
					Assert::ExpectException<std::runtime_error>([&iter] {
						auto& fail = iter[1]; UNREFERENCED_LOCAL(fail);
						});
					iter = vector.end();
					Assert::ExpectException<std::runtime_error>([&iter] {
						auto& fail = *iter; UNREFERENCED_LOCAL(fail);
						});

					Assert::AreEqual(Foo(10), iter[0]);
				}
			}
		}

		TEST_METHOD(OperatorBracketBracket) {
			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };

				{
					Vector<Foo> vector{ a, b, c };
					Assert::AreEqual(a, vector[0]);
					Assert::AreEqual(b, vector[1]);
					Assert::AreEqual(c, vector[2]);
					Assert::ExpectException<std::runtime_error>([&vector] {
						auto& fail = vector[4]; UNREFERENCED_LOCAL(fail);
						});
				}
				{
					const Vector<Foo> vector{ a, b, c };
					Assert::AreEqual(a, vector[0]);
					Assert::AreEqual(b, vector[1]);
					Assert::AreEqual(c, vector[2]);
					Assert::ExpectException<std::runtime_error>([&vector] {
						auto& fail = vector[4]; UNREFERENCED_LOCAL(fail);
						});
				}
			}
			{
				const Bar a{ 10 };
				const Bar b{ 20 };
				const Bar c{ 30 };

				{
					Vector<Bar> vector{ a, b, c };
					Assert::AreEqual(a.Data(), vector[0].Data());
					Assert::AreEqual(b.Data(), vector[1].Data());
					Assert::AreEqual(c.Data(), vector[2].Data());
					Assert::ExpectException<std::runtime_error>([&vector] {
						auto& fail = vector[4]; UNREFERENCED_LOCAL(fail);
						});
				}
				{
					const Vector<Bar> vector{ a, b, c };
					Assert::AreEqual(a.Data(), vector[0].Data());
					Assert::AreEqual(b.Data(), vector[1].Data());
					Assert::AreEqual(c.Data(), vector[2].Data());
					Assert::ExpectException<std::runtime_error>([&vector] {
						auto& fail = vector[4]; UNREFERENCED_LOCAL(fail);
						});
				}
			}
		}

		TEST_METHOD(PushBack) {
			{
				Foo a{ 10 };
				Foo b{ 20 };
				Foo c{ 30 };
				Vector<Foo> vector;

				Assert::AreEqual(size_t(0), vector.Size());
				Assert::IsTrue(vector.IsEmpty());

				vector.PushBack(a);
				Assert::AreEqual(size_t(1), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(a, vector.Back());
				Assert::IsFalse(vector.IsEmpty());

				vector.PushBack(b);
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(b, vector.Back());
				Assert::IsFalse(vector.IsEmpty());

				vector.PushBack(std::move(c));
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(a, vector.Front());
				Assert::AreEqual(Foo{ 30 }, vector.Back());
				Assert::IsFalse(vector.IsEmpty());
			}
			{
				Bar a{ 10 };
				Bar b{ 20 };
				Bar c{ 30 };
				Vector<Bar> vector;

				Assert::AreEqual(size_t(0), vector.Size());
				Assert::IsTrue(vector.IsEmpty());

				vector.PushBack(a);
				Assert::AreEqual(size_t(1), vector.Size());
				Assert::AreEqual(a.Data(), vector.Front().Data());
				Assert::AreEqual(a.Data(), vector.Back().Data());
				Assert::IsFalse(vector.IsEmpty());

				vector.PushBack(b);
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(a.Data(), vector.Front().Data());
				Assert::AreEqual(b.Data(), vector.Back().Data());
				Assert::IsFalse(vector.IsEmpty());

				vector.PushBack(std::move(c));
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(a.Data(), vector.Front().Data());
				Assert::AreEqual(30, vector.Back().Data());
				Assert::IsFalse(vector.IsEmpty());
			}
		}

		TEST_METHOD(PopBack) {
			{
				Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(Foo(10), vector.Front());
				Assert::AreEqual(Foo(30), vector.Back());

				vector.PopBack();
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(Foo(10), vector.Front());
				Assert::AreEqual(Foo(20), vector.Back());

				vector.PopBack();
				Assert::AreEqual(size_t(1), vector.Size());
				Assert::AreEqual(Foo(10), vector.Front());
				Assert::AreEqual(Foo(10), vector.Back());

				vector.PopBack();
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::IsTrue(vector.IsEmpty());
			}
			{
				Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(10, vector.Front().Data());
				Assert::AreEqual(30, vector.Back().Data());

				vector.PopBack();
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(10, vector.Front().Data());
				Assert::AreEqual(20, vector.Back().Data());

				vector.PopBack();
				Assert::AreEqual(size_t(1), vector.Size());
				Assert::AreEqual(10, vector.Front().Data());
				Assert::AreEqual(10, vector.Back().Data());

				vector.PopBack();
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::IsTrue(vector.IsEmpty());

			}
		}

		TEST_METHOD(Remove) {
			{
				Vector<Foo> vector{ Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) };
				Assert::AreEqual(size_t(5), vector.Size());

				vector.Remove(Foo(1));
				Assert::AreEqual(size_t(4), vector.Size());
				Assert::AreEqual(Foo(2), vector.Front());

				vector.RemoveAt(0);
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(Foo(3), vector.Front());

				Assert::IsFalse(vector.RemoveAt(10));

				vector.RemoveAt(vector.begin());
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(Foo(4), vector.Front());

				Assert::ExpectException<std::runtime_error>([&vector] {
					vector.RemoveAt(Vector<Foo>::Iterator());
					});

				vector.RemoveRange(vector.begin(), vector.end());
				Assert::AreEqual(size_t(0), vector.Size());

				vector = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) };
				Assert::ExpectException<std::runtime_error>([&vector] {
					vector.RemoveRange(vector.end(), vector.begin());
					});
				Assert::ExpectException<std::runtime_error>([&vector] {
					vector.RemoveRange(vector.begin(), Vector<Foo>::Iterator());
					});
				Assert::ExpectException<std::runtime_error>([&vector] {
					vector.RemoveRange(Vector<Foo>::Iterator(), vector.end());
					});
				vector.RemoveRange(vector.Find(Foo(2)), vector.Find(Foo(4)));
				Assert::AreEqual(size_t(3), vector.Size());
			}
			{
				Vector<Bar> vector{ Bar(1), Bar(2), Bar(3), Bar(4), Bar(5) };
				Assert::AreEqual(size_t(5), vector.Size());

				vector.Remove(Bar(1));
				Assert::AreEqual(size_t(4), vector.Size());
				Assert::AreEqual(2, vector.Front().Data());

				vector.RemoveAt(0);
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(3, vector.Front().Data());

				Assert::IsFalse(vector.RemoveAt(10));

				vector.RemoveAt(vector.begin());
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(4, vector.Front().Data());

				Assert::ExpectException<std::runtime_error>([&vector] {
					vector.RemoveAt(Vector<Bar>::Iterator());
					});

				vector.RemoveRange(vector.begin(), vector.end());
				Assert::AreEqual(size_t(0), vector.Size());

				vector = { Bar(1), Bar(2), Bar(3), Bar(4), Bar(5) };
				Assert::ExpectException<std::runtime_error>([&vector] {
					vector.RemoveRange(vector.end(), vector.begin());
					});
				Assert::ExpectException<std::runtime_error>([&vector] {
					vector.RemoveRange(vector.begin(), Vector<Bar>::Iterator());
					});
				Assert::ExpectException<std::runtime_error>([&vector] {
					vector.RemoveRange(Vector<Bar>::Iterator(), vector.end());
					});
				vector.RemoveRange(vector.Find(Bar(2)), vector.Find(Bar(4)));
				Assert::AreEqual(size_t(3), vector.Size());
			}
		}

		TEST_METHOD(FrontAndBack) {
			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };

				{
					Vector<Foo> vector{ a, b, c };

					Assert::AreEqual(a, vector.Front());
					Assert::AreEqual(c, vector.Back());
				}
				{
					const Vector<Foo> vector{ a, b, c };

					Assert::AreEqual(a, vector.Front());
					Assert::AreEqual(c, vector.Back());
				}
			}
			{
				const Bar a{ 10 };
				const Bar b{ 20 };
				const Bar c{ 30 };

				{
					Vector<Bar> vector{ a, b, c };

					Assert::AreEqual(a.Data(), vector.Front().Data());
					Assert::AreEqual(c.Data(), vector.Back().Data());
				}
				{
					const Vector<Bar> vector{ a, b, c };

					Assert::AreEqual(a.Data(), vector.Front().Data());
					Assert::AreEqual(c.Data(), vector.Back().Data());
				}
			}
		}

		TEST_METHOD(At) {
			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };

				{
					Vector<Foo> vector{ a, b, c };
					Assert::AreEqual(a, vector.At(0));
					Assert::AreEqual(b, vector.At(1));
					Assert::AreEqual(c, vector.At(2));
				}
				{
					const Vector<Foo> vector{ a, b, c };
					Assert::AreEqual(a, vector.At(0));
					Assert::AreEqual(b, vector.At(1));
					Assert::AreEqual(c, vector.At(2));
				}
			}
			{
				const Bar a{ 10 };
				const Bar b{ 20 };
				const Bar c{ 30 };

				{
					Vector<Bar> vector{ a, b, c };
					Assert::AreEqual(a.Data(), vector.At(0).Data());
					Assert::AreEqual(b.Data(), vector.At(1).Data());
					Assert::AreEqual(c.Data(), vector.At(2).Data());
				}
				{
					const Vector<Bar> vector{ a, b, c };
					Assert::AreEqual(a.Data(), vector.At(0).Data());
					Assert::AreEqual(b.Data(), vector.At(1).Data());
					Assert::AreEqual(c.Data(), vector.At(2).Data());
				}
			}
		}

		TEST_METHOD(BeginAndEnd) {
			{
				{
					Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
					Assert::AreEqual(Foo(10), *vector.begin());
					Assert::AreEqual(Foo(10), *vector.cbegin());
					Assert::ExpectException<std::runtime_error>([&vector]() {
						auto& data = *vector.end(); UNREFERENCED_LOCAL(data);
						});
					Assert::ExpectException<std::runtime_error>([&vector]() {
						auto& data = *vector.cend(); UNREFERENCED_LOCAL(data);
						});
				}
				{
					const Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
					Assert::AreEqual(Foo(10), *vector.begin());
					Assert::ExpectException<std::runtime_error>([&vector]() {
						auto& data = *vector.end(); UNREFERENCED_LOCAL(data);
						});
				}
			}
			{
				{
					Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };
					Assert::AreEqual(10, (*vector.begin()).Data());
					Assert::AreEqual(10, (*vector.cbegin()).Data());
					Assert::ExpectException<std::runtime_error>([&vector]() {
						auto& data = *vector.end(); UNREFERENCED_LOCAL(data);
						});
					Assert::ExpectException<std::runtime_error>([&vector]() {
						auto& data = *vector.cend(); UNREFERENCED_LOCAL(data);
						});
				}
				{
					const Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };
					Assert::AreEqual(10, (*vector.begin()).Data());
					Assert::ExpectException<std::runtime_error>([&vector]() {
						auto& data = *vector.end(); UNREFERENCED_LOCAL(data);
						});
				}
			}
		}

		TEST_METHOD(Find) {
			{
				{
					Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };

					Assert::AreEqual(Foo(10), *vector.Find(Foo(10)));
					Assert::AreEqual(Foo(20), *vector.Find(Foo(20)));
					Assert::AreEqual(Foo(30), *vector.Find(Foo(30)));
					Assert::AreEqual(vector.end(), vector.Find(Foo(40)));
				}
				{
					const Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };

					Assert::AreEqual(Foo(10), *vector.Find(Foo(10)));
					Assert::AreEqual(Foo(20), *vector.Find(Foo(20)));
					Assert::AreEqual(Foo(30), *vector.Find(Foo(30)));
					Assert::AreEqual(vector.end(), vector.Find(Foo(40)));
				}
			}
			{
				{
					Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };

					Assert::AreEqual(10, (*vector.Find(Bar(10))).Data());
					Assert::AreEqual(20, (*vector.Find(Bar(20))).Data());
					Assert::AreEqual(30, (*vector.Find(Bar(30))).Data());
					Assert::AreEqual(vector.end(), vector.Find(Bar(40)));
				}
				{
					const Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };

					Assert::AreEqual(10, (*vector.Find(Bar(10))).Data());
					Assert::AreEqual(20, (*vector.Find(Bar(20))).Data());
					Assert::AreEqual(30, (*vector.Find(Bar(30))).Data());
					Assert::AreEqual(vector.end(), vector.Find(Bar(40)));
				}
			}
		}

		TEST_METHOD(GrowingAndShrinking) {
			{
				Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(Foo(10), *vector.begin());
				Assert::AreEqual(Foo(30), *(--vector.end()));

				vector.Resize(5);
				Assert::AreEqual(size_t(5), vector.Size());
				Assert::AreEqual(Foo(10), *vector.begin());
				Assert::AreEqual(Foo(), *(--vector.end()));
				Assert::AreNotEqual(vector.Size(), vector.Capacity());

				vector.RemoveRange(vector.Find(Foo()), vector.end());
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(Foo(10), *vector.begin());
				Assert::AreEqual(Foo(30), *(--vector.end()));
				Assert::AreNotEqual(vector.Size(), vector.Capacity());

				vector.ShrinkToFit();
				Assert::AreEqual(vector.Size(), vector.Capacity());

				vector.Clear();
				vector.ShrinkToFit();

				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(size_t(0), vector.Capacity());
			}
			{
				Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(10, (*vector.begin()).Data());
				Assert::AreEqual(30, (*(--vector.end())).Data());

				vector.Resize(5);
				Assert::AreEqual(size_t(5), vector.Size());
				Assert::AreEqual(10, (*vector.begin()).Data());
				Assert::AreEqual(0, (*(--vector.end())).Data());
				Assert::AreNotEqual(vector.Size(), vector.Capacity());

				vector.RemoveRange(vector.Find(Bar()), vector.end());
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(10, (*vector.begin()).Data());
				Assert::AreEqual(30, (*(--vector.end())).Data());
				Assert::AreNotEqual(vector.Size(), vector.Capacity());

				vector.ShrinkToFit();
				Assert::AreEqual(vector.Size(), vector.Capacity());

				vector.Clear();
				vector.ShrinkToFit();

				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(size_t(0), vector.Capacity());
			}
		}

		TEST_METHOD(Clear) {
			{
				Vector<Foo> vector{ Foo(10), Foo(20), Foo(30) };
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::IsFalse(vector.IsEmpty());
				Assert::AreNotEqual(vector.begin(), vector.end());

				size_t initialCapacity = vector.Capacity();

				vector.Clear();
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(initialCapacity, vector.Capacity());
				Assert::IsTrue(vector.IsEmpty());
				Assert::AreEqual(vector.begin(), vector.end());
			}
			{
				Vector<Bar> vector{ Bar(10), Bar(20), Bar(30) };
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::IsFalse(vector.IsEmpty());
				Assert::AreNotEqual(vector.begin(), vector.end());

				size_t initialCapacity = vector.Capacity();

				vector.Clear();
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(initialCapacity, vector.Capacity());
				Assert::IsTrue(vector.IsEmpty());
				Assert::AreEqual(vector.begin(), vector.end());
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
