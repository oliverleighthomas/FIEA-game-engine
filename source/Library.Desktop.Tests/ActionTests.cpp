#include "pch.h"
#include "CppUnitTest.h"
#include "TypeManager.h"
#include "GameTime.h"
#include "JsonTableParseHelper.h"
#include "ActionIncrement.h"
#include "ActionList.h"
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
	TEST_CLASS(ActionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			TypeManager::CreateInstance();
			TypeManager::Instance()->RegisterSignature(Action::TypeIdClass(), Action::Signatures());
			TypeManager::Instance()->RegisterSignature(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeManager::Instance()->RegisterSignature(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			Factory<Scope>::Add(std::make_unique<ActionListFactory>());
			Factory<Scope>::Add(std::make_unique<ActionIncrementFactory>());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Factory<Scope>::Remove(std::make_unique<ActionIncrementFactory>());
			Factory<Scope>::Remove(std::make_unique<ActionListFactory>());
			TypeManager::Instance()->RemoveSignature(ActionIncrement::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(ActionList::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(Action::TypeIdClass());
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

		TEST_METHOD(ActionIncrementConstructor) {
			ActionIncrement action;

			Assert::AreEqual("Increment"s, action.Name());
			Assert::AreEqual(""s, action.TargetName());
			Assert::AreEqual(0, action.Step());
		}

		TEST_METHOD(ActionListConstructor) {
			{
				ActionList list;

				Assert::IsNotNull(list.Actions());
				Assert::AreEqual(""s, list.Name());
			}
			{
				ActionList list("List");

				Assert::IsNotNull(list.Actions());
				Assert::AreEqual("List"s, list.Name());
			}
		}

		TEST_METHOD(SetName) {
			ActionIncrement action;

			Assert::AreEqual("Increment"s, action.Name());
			action.SetName("Decrement");
			Assert::AreEqual("Decrement"s, action.Name());
		}

		TEST_METHOD(Increment) {
			const string inputString = R"(
			{
				"A": {
					"type": "integer",
					"value": 10
				},
				"Action": {
					"type": "table",
					"class": "ActionIncrement",
					"value": {
						"Name": {
							"type": "string",
							"value": "ActionTest"
						},
						"Target": {
							"type": "string",
							"value": "A"
						},
						"Step": {
							"type": "integer",
							"value": 5
						}
					}
				}
			})"s;

			Scope root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			auto a = root.Find("A");
			Assert::IsNotNull(a);
			Assert::AreEqual(10, a->GetInt());

			auto actionDatum = root.Find("Action");
			Assert::IsNotNull(actionDatum);
			auto action = actionDatum->GetScope()->As<ActionIncrement>();
			Assert::IsNotNull(action);

			Assert::AreEqual("A"s, action->TargetName());
			Assert::AreEqual(5, action->Step());

			GameTime time;
			action->Update(time);

			Assert::AreEqual(15, a->GetInt());
		}

		TEST_METHOD(ActionLists) {
			const string inputString = R"(
			{
				"A": {
					"type": "integer",
					"value": 10
				},
				"B": {
					"type": "integer",
					"value": 5
				},
				"List": {
					"type": "table",
					"class": "ActionList",
					"value": {
						"Actions": {
							"type": "table",
							"value": [
								{
									"type": "table",
									"class": "ActionIncrement",
									"value": {
										"Name": {
											"type": "string",
											"value": "IncrementA"
										},
										"Target": {
											"type": "string",
											"value": "A"
										},
										"Step": {
											"type": "integer",
											"value": 10
										}
									}
								},
								{
									"type": "table",
									"class": "ActionIncrement",
									"value": {
										"Name": {
											"type": "string",
											"value": "IncrementB"
										},
										"Target": {
											"type": "string",
											"value": "B"
										},
										"Step": {
											"type": "integer",
											"value": 5
										}
									}
								}
							]
						}
					}
				}
			})"s;

			Scope root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			auto a = root.Find("A");
			Assert::AreEqual(10, a->GetInt());
			auto b = root.Find("B");
			Assert::AreEqual(5, b->GetInt());

			auto listDatum = root.Find("List");
			Assert::IsNotNull(listDatum);
			auto actionList = listDatum->GetScope()->As<ActionList>();
			Assert::IsNotNull(actionList);

			GameTime time;
			actionList->Update(time);

			Assert::AreEqual(20, a->GetInt());
			Assert::AreEqual(10, b->GetInt());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}


