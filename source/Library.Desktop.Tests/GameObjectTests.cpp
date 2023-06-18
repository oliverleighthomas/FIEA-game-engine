#include "pch.h"
#include "CppUnitTest.h"
#include "TypeManager.h"
#include "GameObject.h"
#include "Action.h"
#include "TestAction.h"
#include "Factory.h"
#include "JsonTableParseHelper.h"
#include "TestGameObject.h"
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
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework {
	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		std::string temp = glm::to_string(t);
		std::wstring ws(temp.begin(), temp.end());

		return ws;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(const FieaGameEngine::Datum& t) {
		RETURN_WIDE_STRING(&t);
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(GameObjectTests)
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
			TypeManager::Instance()->RegisterSignature(TestAction::TypeIdClass(), TestAction::Signatures());
			TypeManager::Instance()->RegisterSignature(GameObject::TypeIdClass(), GameObject::Signatures());
			Factory<Scope>::Add(std::make_unique<GameObjectFactory>());
			Factory<Scope>::Add(std::make_unique<TestActionFactory>());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Factory<Scope>::Remove(std::make_unique<TestActionFactory>());
			Factory<Scope>::Remove(std::make_unique<GameObjectFactory>());
			TypeManager::Instance()->RemoveSignature(GameObject::TypeIdClass());
			TypeManager::Instance()->RemoveSignature(TestAction::TypeIdClass());
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

		TEST_METHOD(Constructor) {
			GameObject gameObject;
			
			Assert::IsTrue(gameObject.name.empty());
			Assert::AreEqual(glm::vec4(0), gameObject.position);
			Assert::AreEqual(glm::vec4(0), gameObject.rotation);
			Assert::AreEqual(glm::vec4(0), gameObject.scale);
			Assert::AreEqual(size_t(0), gameObject.Find("Children")->Size());
			Assert::AreEqual(size_t(0), gameObject.Find("Actions")->Size());

			TypeManager::Instance()->RegisterSignature(TestGameObject::TypeIdClass(), TestGameObject::Signatures());
			Factory<Scope>::Add(std::make_unique<TestGameObjectFactory>());

			TestGameObject testGameObject;

			Assert::AreEqual(0, testGameObject.timesUpdated);

			Factory<Scope>::Remove(std::make_unique<TestGameObjectFactory>());
			TypeManager::Instance()->RemoveSignature(TestGameObject::TypeIdClass());
		}

		TEST_METHOD(Actions) {
			{
				GameObject gameObject;

				Assert::AreEqual(size_t(0), gameObject.Actions().Size());
				Assert::AreEqual(gameObject.Actions(), *gameObject.Find("Actions"));
			}
			{
				const GameObject gameObject;

				Assert::AreEqual(size_t(0), gameObject.Actions().Size());
				Assert::AreEqual(gameObject.Actions(), *gameObject.Find("Actions"));
			}
		}

		TEST_METHOD(Parse) {
			TypeManager::Instance()->RegisterSignature(TestGameObject::TypeIdClass(), TestGameObject::Signatures());
			Factory<Scope>::Add(std::make_unique<TestGameObjectFactory>());

			const string inputString = R"(
			{
				"Name": {
					"type": "string",
					"value": "Parent"
				},
				"Children": {
					"type": "table",
					"value": [
						{
							"type": "table",
							"class" : "GameObject",
							"value": {
								"Name": {
									"type": "string",
									"value": "ChildOne"
								}
							}
						},
						{
							"type": "table",
							"class" : "GameObject",
							"value": {
								"Name": {
									"type": "string",
									"value": "ChildTwo"
								}
							}
						},
						{
							"type": "table",
							"class" : "TestGameObject",
							"value": {
								"Name": {
									"type": "string",
									"value": "ChildThree"
								}
							}
						}
					]
				},
				"Actions": {
					"type": "table",
					"value": [
						{
							"type": "table",
							"class": "TestAction",
							"value": {
								"Name": {
									"type": "string",
									"value": "ActionOne"
								}
							}
						}
					]
				}
			})"s;

			GameObject root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			auto name = root.Find("Name");
			Assert::IsNotNull(name);
			Assert::AreEqual("Parent"s, name->GetString());

			auto children = root.Find("Children");
			Assert::IsNotNull(children);
			Assert::AreEqual(size_t(3), children->Size());

			auto childOne = children->GetScope(0);
			Assert::IsNotNull(childOne);
			auto childOneName = childOne->Find("Name");
			Assert::IsNotNull(childOneName);
			Assert::AreEqual("ChildOne"s, childOneName->GetString());

			auto childTwo = children->GetScope(1);
			Assert::IsNotNull(childTwo);
			auto childTwoName = childTwo->Find("Name");
			Assert::IsNotNull(childTwoName);
			Assert::AreEqual("ChildTwo"s, childTwoName->GetString());

			auto childThree = children->GetScope(2);
			Assert::IsNotNull(childThree);
			auto childThreeName = childThree->Find("Name");
			Assert::IsNotNull(childThreeName);
			Assert::AreEqual("ChildThree"s, childThreeName->GetString());

			auto actions = root.Find("Actions");
			Assert::IsNotNull(actions);
			Assert::AreEqual(size_t(1), actions->Size());

			auto action = actions->GetScope()->As<TestAction>();
			Assert::IsNotNull(action);
			Assert::AreEqual("ActionOne"s, action->Name());

			Assert::AreEqual(*actions, root.Actions());

			Factory<Scope>::Remove(std::make_unique<TestGameObjectFactory>());
			TypeManager::Instance()->RemoveSignature(TestGameObject::TypeIdClass());
		}

		TEST_METHOD(Update) {
			TypeManager::Instance()->RegisterSignature(TestGameObject::TypeIdClass(), TestGameObject::Signatures());
			Factory<Scope>::Add(std::make_unique<TestGameObjectFactory>());
			
			const string inputString = R"(
			{
				"Name": {
					"type": "string",
					"value": "Grandparent"
				},
				"Children": {
					"type": "table",
					"value": [
						{
							"type": "table",
							"class" : "TestGameObject",
							"value": {
								"Name": {
									"type": "string",
									"value": "Parent"
								},
								"Children": {
									"type": "table",
									"class": "TestGameObject",
									"value": {
										"Name": {
											"type": "string",
											"value": "Child"
										}
									}
								}
							}
						}
					]
				},
				"Actions": {
					"type": "table",
					"value": [
						{
							"type": "table",
							"class": "TestAction",
							"value": {
								"Name": {
									"type": "string",
									"value": "ActionOne"
								}
							}
						}
					]
				}
			})"s;

			GameObject root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			GameObject* grandParent = &root;

			TestAction* action = grandParent->Find("Actions")->GetScope()->As<TestAction>();

			TestGameObject* parent = grandParent->Find("Children")->GetScope()->As<TestGameObject>();
			Assert::IsNotNull(parent);

			TestGameObject* child = parent->Find("Children")->GetScope()->As<TestGameObject>();
			Assert::IsNotNull(child);

			Assert::AreEqual(0, parent->timesUpdated);
			Assert::AreEqual(0, child->timesUpdated);
			Assert::AreEqual(0, action->Count());

			GameTime time;
			grandParent->Update(time);

			Assert::AreEqual(1, parent->timesUpdated);
			Assert::AreEqual(1, child->timesUpdated);
			Assert::AreEqual(1, action->Count());

			parent->Update(time);

			Assert::AreEqual(2, parent->timesUpdated);
			Assert::AreEqual(2, child->timesUpdated);
			Assert::AreEqual(1, action->Count());

			child->Update(time);

			Assert::AreEqual(2, parent->timesUpdated);
			Assert::AreEqual(3, child->timesUpdated);
			Assert::AreEqual(1, action->Count());

			Factory<Scope>::Remove(std::make_unique<TestGameObjectFactory>());
			TypeManager::Instance()->RemoveSignature(TestGameObject::TypeIdClass());
		}

		TEST_METHOD(CreateAction) {
			const string inputString = R"(
			{
				"Name": {
					"type": "string",
					"value": "Grandparent"
				},
				"Actions": {
					"type": "table",
					"value": [
						{
							"type": "table",
							"class": "TestAction",
							"value": {
								"Name": {
									"type": "string",
									"value": "ActionOne"
								}
							}
						}
					]
				}
			})"s;

			GameObject root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			Assert::AreEqual(size_t(1), root.Actions().Size());

			auto actionOne = root.Actions().GetScope()->As<TestAction>();
			Assert::IsNotNull(actionOne);
			Assert::AreEqual(0, actionOne->Count());

			GameTime time;
			root.Update(time);

			Assert::AreEqual(1, actionOne->Count());

			root.CreateAction("TestAction", "ActionTwo");
			Assert::AreEqual(size_t(2), root.Actions().Size());

			auto actionTwo = root.Actions().GetScope(1)->As<TestAction>();
			Assert::IsNotNull(actionTwo);
			Assert::AreEqual(0, actionTwo->Count());

			root.Update(time);

			Assert::AreEqual(2, actionOne->Count());
			Assert::AreEqual(1, actionTwo->Count());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}


