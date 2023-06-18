#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Event.h"
#include "EventQueue.h"
#include "EventTestSubscribers.h"
#include "ToStringSpecialization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework {
	
}

namespace LibraryDesktopTests
{
	TEST_CLASS(EventTests)
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

		TEST_METHOD(RTTITests) {
			Foo foo{ 10 };
			Event<Foo> fooEvent(foo);

			Assert::AreEqual(Event<Foo>::TypeName(), fooEvent.TypeName());

			Assert::IsTrue(fooEvent.Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(fooEvent.Is(EventPublisher::TypeIdClass()));

			Assert::IsNotNull(fooEvent.As<Event<Foo>>());
			Assert::IsNotNull(fooEvent.As<EventPublisher>());
		}

		TEST_METHOD(EventConstructor) {
			Foo foo{ 10 };
			Event<Foo> fooEvent(foo);

			Assert::AreEqual(foo, fooEvent.Message());
			Assert::AreEqual(size_t(0), fooEvent.Subscribers().Size());
		}

		TEST_METHOD(EventQueueConstructor) {
			EventQueue eventQueue;

			Assert::AreEqual(size_t(0), eventQueue.Size());
			Assert::AreEqual(size_t(0), eventQueue.Events().Size());
			Assert::IsTrue(eventQueue.IsEmpty());
		}

		TEST_METHOD(SubscribeAndUnsubscribe) {
			Foo foo{ 10 };
			Event<Foo> fooEvent(foo);

			SubscriberFoo subOne, subTwo, subThree;

			Assert::AreEqual(size_t(0), fooEvent.Subscribers().Size());

			Event<Foo>::Subscribe(subOne);
			Assert::AreEqual(size_t(1), fooEvent.Subscribers().Size());

			Event<Foo>::Subscribe(subTwo);
			Assert::AreEqual(size_t(2), fooEvent.Subscribers().Size());

			Event<Foo>::Subscribe(subThree);
			Assert::AreEqual(size_t(3), fooEvent.Subscribers().Size());

			Event<Foo>::Unsubscribe(subThree);
			Assert::AreEqual(size_t(2), fooEvent.Subscribers().Size());

			Event<Foo>::Unsubscribe(subTwo);
			Assert::AreEqual(size_t(1), fooEvent.Subscribers().Size());

			Event<Foo>::Unsubscribe(subOne);
			Assert::AreEqual(size_t(0), fooEvent.Subscribers().Size());

			Event<Foo>::Clear();
		}

		TEST_METHOD(Enqueue) {
			EventQueue eventQueue;

			Assert::AreEqual(size_t(0), eventQueue.Size());
			Assert::AreEqual(size_t(0), eventQueue.Events().Size());
			Assert::IsTrue(eventQueue.IsEmpty());

			Foo foo(23);
			std::shared_ptr<Event<Foo>> queueOne = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> queueTwo = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> queueThree = std::make_shared<Event<Foo>>(foo);

			eventQueue.Enqueue(queueOne, GameTime());
			Assert::AreEqual(size_t(1), eventQueue.Size());
			Assert::AreEqual(size_t(1), eventQueue.Events().Size());
			Assert::IsFalse(eventQueue.IsEmpty());

			eventQueue.Enqueue(queueTwo, GameTime());
			Assert::AreEqual(size_t(2), eventQueue.Size());
			Assert::AreEqual(size_t(2), eventQueue.Events().Size());

			eventQueue.Enqueue(queueThree, GameTime());
			Assert::AreEqual(size_t(3), eventQueue.Size());
			Assert::AreEqual(size_t(3), eventQueue.Events().Size());

			Event<Foo>::Clear();
		}

		TEST_METHOD(Clear) {
			{
				EventQueue eventQueue;

				Assert::AreEqual(size_t(0), eventQueue.Size());
				Assert::AreEqual(size_t(0), eventQueue.Events().Size());
				Assert::IsTrue(eventQueue.IsEmpty());

				Foo foo(23);
				std::shared_ptr<Event<Foo>> queueOne = std::make_shared<Event<Foo>>(foo);
				std::shared_ptr<Event<Foo>> queueTwo = std::make_shared<Event<Foo>>(foo);
				std::shared_ptr<Event<Foo>> queueThree = std::make_shared<Event<Foo>>(foo);

				eventQueue.Enqueue(queueOne, GameTime());
				Assert::AreEqual(size_t(1), eventQueue.Size());
				Assert::AreEqual(size_t(1), eventQueue.Events().Size());
				Assert::IsFalse(eventQueue.IsEmpty());

				eventQueue.Enqueue(queueTwo, GameTime());
				Assert::AreEqual(size_t(2), eventQueue.Size());
				Assert::AreEqual(size_t(2), eventQueue.Events().Size());

				eventQueue.Enqueue(queueThree, GameTime());
				Assert::AreEqual(size_t(3), eventQueue.Size());
				Assert::AreEqual(size_t(3), eventQueue.Events().Size());

				eventQueue.Clear();
				Assert::AreEqual(size_t(0), eventQueue.Size());
				Assert::AreEqual(size_t(0), eventQueue.Events().Size());
				Assert::IsTrue(eventQueue.IsEmpty());
			}
			{
				Foo foo{ 10 };
				Event<Foo> fooEvent(foo);

				SubscriberFoo subOne, subTwo, subThree;

				Assert::AreEqual(size_t(0), fooEvent.Subscribers().Size());

				Event<Foo>::Subscribe(subOne);
				Assert::AreEqual(size_t(1), fooEvent.Subscribers().Size());

				Event<Foo>::Subscribe(subTwo);
				Assert::AreEqual(size_t(2), fooEvent.Subscribers().Size());

				Event<Foo>::Subscribe(subThree);
				Assert::AreEqual(size_t(3), fooEvent.Subscribers().Size());

				Event<Foo>::Clear();
				Assert::AreEqual(size_t(0), fooEvent.Subscribers().Size());
			}
		}

		TEST_METHOD(Deliver) {
			Foo foo{ 10 };
			Event<Foo> fooEvent(foo);
			SubscriberFoo subFoo;

			Assert::AreEqual(fooEvent.Message(), foo);
			Assert::AreEqual(0, subFoo.count);
			Assert::AreEqual(Foo(), subFoo.foo);

			Event<Foo>::Subscribe(subFoo);
			fooEvent.Deliver();

			Assert::AreEqual(subFoo.count, 1);
			Assert::AreEqual(subFoo.foo, foo);

			Event<Foo>::Clear();
		}

		TEST_METHOD(Update) {
			{
				Foo foo{ 10 };
				SubscriberFoo subFoo;
				GameTime gameTime;

				Event<Foo>::Subscribe(subFoo);
				std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(foo);

				EventQueue eventQueue;
				eventQueue.Enqueue(fooEvent, gameTime);

				Assert::AreEqual(0, subFoo.count);
				Assert::AreEqual(Foo(), subFoo.foo);
				Assert::AreEqual(size_t(1), eventQueue.Size());

				eventQueue.Update(gameTime);

				Assert::AreEqual(1, subFoo.count);
				Assert::AreEqual(foo, subFoo.foo);
				Assert::AreEqual(size_t(0), eventQueue.Size());

				Event<Foo>::Clear();
			}
			{
				Foo foo{ 10 };
				SubscriberFoo subFoo;
				GameTime gameTime;

				Event<Foo>::Subscribe(subFoo);
				std::shared_ptr<Event<Foo>> fooEvent = std::make_shared<Event<Foo>>(foo);

				EventQueue eventQueue;
				eventQueue.Enqueue(fooEvent, gameTime, std::chrono::milliseconds(5));

				Assert::AreEqual(0, subFoo.count);
				Assert::AreEqual(Foo(), subFoo.foo);
				Assert::AreEqual(size_t(1), eventQueue.Size());

				eventQueue.Update(gameTime);
				Assert::AreEqual(0, subFoo.count);
				Assert::AreEqual(Foo(), subFoo.foo);
				Assert::AreEqual(size_t(1), eventQueue.Size());

				gameTime.SetCurrentTime(eventQueue.Events()[0].expirationTime);
				eventQueue.Update(gameTime);

				Assert::AreEqual(1, subFoo.count);
				Assert::AreEqual(foo, subFoo.foo);
				//Assert::AreEqual(size_t(0), eventQueue.Size());

				Event<Foo>::Clear();
			}
			{
				Foo foo{ 10 };
				EnqueueDuringUpdate subscriber;
				GameTime gameTime;

				Event<Foo>::Subscribe(subscriber);
				Event<Foo> fooEvent = Event<Foo>(foo);

				Assert::AreEqual(0, subscriber.count);
				Assert::IsNull(subscriber.queue);

				EventQueue eventQueue;
				subscriber.queue = &eventQueue;
				Assert::IsNotNull(subscriber.queue);

				fooEvent.Deliver();
				Assert::AreEqual(size_t(1), eventQueue.Size());

				gameTime.SetCurrentTime(eventQueue.Events()[0].expirationTime);
				eventQueue.Update(gameTime);
				Assert::AreEqual(size_t(1), eventQueue.Size());

				Event<Foo>::Clear();
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}


