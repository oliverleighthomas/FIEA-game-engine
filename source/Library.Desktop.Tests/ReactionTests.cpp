#include "pch.h"
#include "CppUnitTest.h"
#include "ActionIncrement.h"
#include "Reaction.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "ActionEvent.h"
#include "GameState.h"
#include "GameObject.h"
#include "EventMessageAttributed.h"
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
	TEST_CLASS(ReactionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif

			TypeManager::CreateInstance();
			TypeManager::Instance()->RegisterSignature(GameObject::TypeIdClass(), GameObject::Signatures());
			TypeManager::Instance()->RegisterSignature(Action::TypeIdClass(), Action::Signatures());
			TypeManager::Instance()->RegisterSignature(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeManager::Instance()->RegisterSignature(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeManager::Instance()->RegisterSignature(ActionEvent::TypeIdClass(), ActionEvent::Signatures());
			TypeManager::Instance()->RegisterSignature(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::Signatures());
			TypeManager::Instance()->RegisterSignature(ReactionAttributed::TypeIdClass(), ReactionAttributed::Signatures());
			Factory<Scope>::Add(std::make_unique<GameObjectFactory>());
			Factory<Scope>::Add(std::make_unique<ActionListFactory>());
			Factory<Scope>::Add(std::make_unique<ActionIncrementFactory>());
			Factory<Scope>::Add(std::make_unique<ReactionAttributedFactory>());
			Factory<Scope>::Add(std::make_unique<ActionEventFactory>());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Event<EventMessageAttributed>::Clear();
			Factory<Scope>::Remove(std::make_unique<ActionEventFactory>());
			Factory<Scope>::Remove(std::make_unique<ReactionAttributedFactory>());
			Factory<Scope>::Remove(std::make_unique<ActionIncrementFactory>());
			Factory<Scope>::Remove(std::make_unique<ActionListFactory>());
			Factory<Scope>::Remove(std::make_unique<GameObjectFactory>());
			TypeManager::Instance()->RemoveSignature(ReactionAttributed::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(EventMessageAttributed::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(ActionList::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(ActionEvent::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(ActionIncrement::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(Action::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(GameObject::TypeIdClass());
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

		TEST_METHOD(RTTI) {
			ReactionAttributed reaction;

			Assert::IsNotNull(reaction.As<Reaction>());
			Assert::IsNotNull(reaction.As<ReactionAttributed>());
		}

		TEST_METHOD(Constructor) {
			{
				ReactionAttributed reaction;

				Assert::AreEqual(size_t(1), Event<EventMessageAttributed>::Subscribers().Size());
				Assert::IsTrue(reaction.GetSubType().empty());
				Assert::IsTrue(reaction.Name().empty());
			}
			{
				ReactionAttributed reaction("TestSubtype");

				Assert::AreEqual(size_t(1), Event<EventMessageAttributed>::Subscribers().Size());
				Assert::AreEqual("TestSubtype"s, reaction.GetSubType());
				Assert::IsTrue(reaction.Name().empty());
			}
			{
				ReactionAttributed reaction("TestSubtype", "TestName");

				Assert::AreEqual(size_t(1), Event<EventMessageAttributed>::Subscribers().Size());
				Assert::AreEqual("TestSubtype"s, reaction.GetSubType());
				Assert::AreEqual("TestName"s, reaction.Name());
			}
			{
				EventMessageAttributed message;

				Assert::IsTrue(message.GetSubType().empty());
			}
		}

		TEST_METHOD(SetSubtype) {
			{
				ReactionAttributed reaction("TestSubtype");

				Assert::AreEqual("TestSubtype"s, reaction.GetSubType());

				reaction.SetSubType("NewSubtype");

				Assert::AreEqual("NewSubtype"s, reaction.GetSubType());
			}
			{
				ActionEvent actionEvent;
				actionEvent.SetSubType("TestSubtype");

				Assert::AreEqual("TestSubtype"s, actionEvent.GetSubType());
			}
			{
				EventMessageAttributed message;
				message.SetSubType("TestSubtype");

				Assert::AreEqual("TestSubtype"s, message.GetSubType());
			}
		}

		TEST_METHOD(SetDelay) {
			ActionEvent actionEvent;

			Assert::AreEqual(0U, actionEvent.GetDelay());

			actionEvent.SetDelay(5U);

			Assert::AreEqual(5U, actionEvent.GetDelay());
		}

		TEST_METHOD(SetName) {
			GameObject root("Root");
			GameState::Initialize(&root);

			Assert::AreEqual("Root"s, GameState::GetName());

			GameState::SetName("NewName");

			Assert::AreEqual("NewName"s, GameState::GetName());
			Assert::AreEqual("NewName"s, root.name);

			GameState::CleanUp();
		}

		TEST_METHOD(Clone) {
			{
				ReactionAttributed reaction("TestSubtype", "TestName");

				Assert::AreEqual(size_t(1), Event<EventMessageAttributed>::Subscribers().Size());
				Assert::AreEqual("TestSubtype"s, reaction.GetSubType());
				Assert::AreEqual("TestName"s, reaction.Name());

				ReactionAttributed* clone = reaction.Clone();

				Assert::AreEqual(size_t(2), Event<EventMessageAttributed>::Subscribers().Size());
				Assert::AreEqual(clone->GetSubType(), reaction.GetSubType());
				Assert::AreEqual(clone->Name(), reaction.Name());

				delete(clone);

				Assert::AreEqual(size_t(1), Event<EventMessageAttributed>::Subscribers().Size());
			}
			{
				ActionEvent actionEvent;
				ActionEvent* clone = actionEvent.Clone();

				Assert::AreEqual(clone->GetSubType(), actionEvent.GetSubType());
				Assert::AreEqual(clone->GetDelay(), actionEvent.GetDelay());
				Assert::AreEqual(clone->Name(), actionEvent.Name());

				delete(clone);
			}
			{
				EventMessageAttributed message;
				EventMessageAttributed* clone = message.Clone();

				Assert::AreEqual(clone->GetSubType(), message.GetSubType());

				delete(clone);
			}
		}

		TEST_METHOD(ActionTests) {
			Assert::AreEqual(size_t(0), Event<EventMessageAttributed>::Subscribers().Size());
			{
				GameObject gameObject;
				GameState::Initialize(&gameObject);
				Assert::IsNotNull(GameState::GetEventQueue());
				auto& eventQueue = *GameState::GetEventQueue();
				Datum& a = gameObject.AppendAuxiliaryAttribute("A"s) = 1;

				const std::string name = "Test Reaction";
				const std::string subType = "Test";
				ReactionAttributed* reactionAttributed = new ReactionAttributed(subType, name);
				auto& reactionScope = gameObject.Adopt(*reactionAttributed, "Reactions"s);
				auto reaction = reactionScope.As<ReactionAttributed>();
				Assert::IsNotNull(reaction);
				Action* increment = reaction->CreateAction(ActionIncrement::TypeName(), "Increment Action"s);
				ActionIncrement* incrementAction = increment->As<ActionIncrement>();
				Assert::IsNotNull(incrementAction);
				Assert::IsTrue(incrementAction->TargetName().empty());
				Assert::AreEqual(0, incrementAction->Step());

				GameObject* child = gameObject.CreateChild(GameObject::TypeName(), "Test Child");
				Action* action = child->CreateAction(ActionEvent::TypeName(), "Test Action");
				ActionEvent* actionEvent = action->As<ActionEvent>();
				Assert::IsNotNull(actionEvent);
				actionEvent->SetSubType(subType);
				actionEvent->AppendAuxiliaryAttribute("Target"s) = "A"s;
				actionEvent->AppendAuxiliaryAttribute("Step"s) = 2;
				Assert::AreEqual(0U, actionEvent->GetDelay());

				GameState::GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point());
				Assert::AreEqual(size_t(0), eventQueue.Size());
				GameState::Update();
				Assert::AreEqual(size_t(1), eventQueue.Size());
				GameState::GetGameTime().SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
				GameState::Update();

				Assert::AreEqual("A"s, incrementAction->TargetName());
				Assert::AreEqual(2, incrementAction->Step());
				Assert::AreEqual(3, a.GetInt());

				GameState::CleanUp();
			}
		}
		

	private:
		inline static _CrtMemState _startMemState;
	};
}


