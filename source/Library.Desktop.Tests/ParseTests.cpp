#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "AttributedFoo.h"
#include "Factory.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "JsonTestParseHelper.h"
#include "JsonIntegerParseHelper.h"
#include "JsonTableParseHelper.h"
#include "ToStringSpecialization.h"

#include <fstream>
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework {
	template<>
	std::wstring ToString<JsonParseCoordinator>(JsonParseCoordinator* t) {
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<JsonParseCoordinator::Wrapper>(const JsonParseCoordinator::Wrapper& t) {
		RETURN_WIDE_STRING(&t);
	}

	template<>
	std::wstring ToString<UnitTests::JsonTestParseHelper>(UnitTests::JsonTestParseHelper* t) {
		RETURN_WIDE_STRING(t);
	}

	template<>
	std::wstring ToString<UnitTests::JsonTestParseHelper::Wrapper>(UnitTests::JsonTestParseHelper::Wrapper* t) {
		RETURN_WIDE_STRING(t);
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(ParseTests)
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

		TEST_METHOD(JsonParseHelperRTTI) {
			JsonTestParseHelper helper;

			RTTI* rtti = &helper;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTestParseHelper::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			JsonTestParseHelper* h = rtti->As<JsonTestParseHelper>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&helper, h);
		}

		TEST_METHOD(JsonParseHelperWrapperRTTI) {
			JsonTestParseHelper::Wrapper wrapper;

			RTTI* rtti = &wrapper;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::Wrapper::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTestParseHelper::Wrapper::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			JsonTestParseHelper::Wrapper* h = rtti->As<JsonTestParseHelper::Wrapper>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&wrapper, h);
		}

		TEST_METHOD(Constructor) {
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().Size());
			Assert::AreSame(*wrapper, parseCoordinator.GetWrapper());

			parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().Size());

			std::shared_ptr<JsonParseCoordinator::Wrapper> anotherWrapper(new JsonTestParseHelper::Wrapper);
			parseCoordinator.SetWrapper(anotherWrapper);
			Assert::AreSame(*anotherWrapper, parseCoordinator.GetWrapper());
		}

		TEST_METHOD(MoveSemantics) {
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			{
				JsonParseCoordinator parseCoordinator(wrapper);
				parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

				JsonParseCoordinator otherParseCoordinator = std::move(parseCoordinator);
				Assert::AreEqual(size_t(1), otherParseCoordinator.Helpers().Size());
				Assert::AreSame(*wrapper, otherParseCoordinator.GetWrapper());
			}
			{
				JsonParseCoordinator parseCoordinator(wrapper);
				parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

				std::shared_ptr<JsonParseCoordinator::Wrapper> otherWrapper(new JsonTestParseHelper::Wrapper);
				JsonParseCoordinator otherParseCoordinator(otherWrapper);
				otherParseCoordinator = std::move(parseCoordinator);
				Assert::AreEqual(size_t(1), otherParseCoordinator.Helpers().Size());
				Assert::AreSame(*wrapper, otherParseCoordinator.GetWrapper());
			}
			{
				JsonParseCoordinator parseCoordinator(wrapper);
				parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

				JsonParseCoordinator toMove(wrapper);
				toMove.AddHelper(std::make_shared<JsonTestParseHelper>());
				toMove.AddHelper(std::make_shared<JsonIntegerParseHelper>());
				toMove.AddHelper(std::make_shared<JsonTableParseHelper>());

				auto clone = parseCoordinator.Create();
				*clone = std::move(*toMove.Create());

				Assert::AreNotEqual(clone.get(), &parseCoordinator);
				Assert::AreEqual(size_t(3), clone->Helpers().Size());

				auto cloneHelper = clone->Helpers()[0].get()->As<JsonTestParseHelper>();
				Assert::IsNotNull(cloneHelper);
				Assert::AreNotEqual(cloneHelper, parseCoordinator.Helpers()[0].get()->As<JsonTestParseHelper>());
				Assert::AreNotSame(clone->GetWrapper(), parseCoordinator.GetWrapper());
			}
		}

		TEST_METHOD(Create) {
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());
			parseCoordinator.AddHelper(std::make_shared<JsonIntegerParseHelper>());

			auto clone = parseCoordinator.Create();
			Assert::AreNotEqual(clone.get(), &parseCoordinator);
			Assert::AreEqual(size_t(2), clone->Helpers().Size());

			auto cloneHelper = clone->Helpers()[0].get()->As<JsonTestParseHelper>();
			Assert::IsNotNull(cloneHelper);
			Assert::AreNotEqual(cloneHelper, parseCoordinator.Helpers()[0].get()->As<JsonTestParseHelper>());
			Assert::AreNotSame(clone->GetWrapper(), parseCoordinator.GetWrapper());
		}

		TEST_METHOD(ParseFromString) {
			const std::string inputString = R"(
			{
				"Address":
				{
					"type": "table",
					"value":
					{
						"Street":
						{
							"type": "string",
							"value": "123 Anystreet St."
						}
					}
				}
			})"s;

			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParserHelper = std::make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(testParserHelper);

			Assert::IsFalse(testParserHelper->initializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParserHelper->initializeCalled);

			Assert::AreEqual(0U, testParserHelper->startHandlerCount);
			Assert::AreEqual(0U, testParserHelper->endHandlerCount);
			Assert::AreEqual(0U, wrapper->Depth());
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(0U, rawWrapper->maxDepth);

			parseCoordinator.DeserializeObject(inputString);
			Assert::AreEqual(6U, testParserHelper->startHandlerCount);
			Assert::AreEqual(6U, testParserHelper->endHandlerCount);
			//Assert::AreEqual(0U, wrapper->Depth());
			Assert::AreEqual(4U, rawWrapper->maxDepth);
		}

		TEST_METHOD(ParseFromStream) {
			std::stringstream inputStream;
			inputStream << R"(
			{
				"Health":
				{
					"type": "integer",
					"value": 100
				}
			})"s;

			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParserHelper = std::make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(testParserHelper);

			Assert::IsFalse(testParserHelper->initializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParserHelper->initializeCalled);

			parseCoordinator.DeserializeObject(inputStream);

			Assert::AreEqual(3U, testParserHelper->startHandlerCount);
			Assert::AreEqual(3U, testParserHelper->endHandlerCount);
			//Assert::AreEqual(0U, wrapper->Depth());
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(2U, rawWrapper->maxDepth);
		}

		TEST_METHOD(ParseFromFile) {
			std::string fileName = "TestFile.json";
			const std::string inputString = R"({ "Health": { "type": "integer", "value": 100 } })";

			std::ofstream outputFile(fileName);
			Assert::IsTrue(outputFile.good());

			outputFile << inputString;
			outputFile.close();

			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParserHelper = std::make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(testParserHelper);

			Assert::IsFalse(testParserHelper->initializeCalled);
			testParserHelper->Initialize();
			Assert::IsTrue(testParserHelper->initializeCalled);

			parseCoordinator.DeserializeObjectFromFile(fileName);

			Assert::AreEqual(3U, testParserHelper->startHandlerCount);
			Assert::AreEqual(3U, testParserHelper->endHandlerCount);
			//Assert::AreEqual(0U, wrapper->Depth());
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(2U, rawWrapper->maxDepth);

			Assert::ExpectException<std::exception>([&parseCoordinator] {
				parseCoordinator.DeserializeObjectFromFile("BadFile.json");
				});
		}

		TEST_METHOD(ParsingInvalidJson) {
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonIntegerParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonIntegerParseHelper>());

			Assert::ExpectException<std::exception>([&parseCoordinator] {
				const std::string inputString = R"({ "integer": { 100 } })";
				parseCoordinator.DeserializeObject(inputString);
				});
		}

		TEST_METHOD(IntegerParsing) {
			std::string inputString = R"({ "integer": 100 })"s;

			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonIntegerParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonIntegerParseHelper>());

			parseCoordinator.DeserializeObject(inputString);
			JsonIntegerParseHelper::Wrapper* rawWrapper = static_cast<JsonIntegerParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(size_t(1), rawWrapper->data.Size());
			Assert::AreEqual(100, rawWrapper->data[0]);
		}

		TEST_METHOD(IntegerArrayParsing) {
			std::string inputString = R"({ "integer": [ 10, 20, 30, 40 ] })"s;

			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonIntegerParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonIntegerParseHelper>());

			parseCoordinator.DeserializeObject(inputString);
			JsonIntegerParseHelper::Wrapper* rawWrapper = static_cast<JsonIntegerParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(size_t(4), rawWrapper->data.Size());
			Assert::AreEqual(10, rawWrapper->data[0]);
			Assert::AreEqual(20, rawWrapper->data[1]);
			Assert::AreEqual(30, rawWrapper->data[2]);
			Assert::AreEqual(40, rawWrapper->data[3]);
		}

		TEST_METHOD(ValueArrayParsing) {
			const std::string inputString = R"(
			{
				"Numbers": [ 1, 2, 3 ]
			})"s;

			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParserHelper = std::make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(testParserHelper);

			Assert::IsFalse(testParserHelper->initializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParserHelper->initializeCalled);

			Assert::AreEqual(0U, testParserHelper->startHandlerCount);
			Assert::AreEqual(0U, testParserHelper->endHandlerCount);
			Assert::AreEqual(0U, wrapper->Depth());
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(0U, rawWrapper->maxDepth);

			parseCoordinator.DeserializeObject(inputString);

			Assert::AreEqual(3U, testParserHelper->startHandlerCount);
			Assert::AreEqual(3U, testParserHelper->endHandlerCount);
			Assert::AreEqual(0U, wrapper->Depth());
			Assert::AreEqual(1U, rawWrapper->maxDepth);
		}

		TEST_METHOD(ObjectArrayParsing) {
			std::string inputString = R"(
			{
				"Objects": 
				[
					{
						"1": 1,
						"2": 1,
						"3": 1,
						"4": 1
					},
					{ "2": 2 },
					{ "3": 3 }
				]
			})"s;

			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParserHelper = std::make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(testParserHelper);

			Assert::IsFalse(testParserHelper->initializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParserHelper->initializeCalled);

			Assert::AreEqual(0U, testParserHelper->startHandlerCount);
			Assert::AreEqual(0U, testParserHelper->endHandlerCount);
			Assert::AreEqual(0U, wrapper->Depth());
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(0U, rawWrapper->maxDepth);
			
			parseCoordinator.DeserializeObject(inputString);

			Assert::AreEqual(6U, testParserHelper->startHandlerCount);
			Assert::AreEqual(6U, testParserHelper->endHandlerCount);
			Assert::AreEqual(0U, wrapper->Depth());
			Assert::AreEqual(2U, rawWrapper->maxDepth);
		}

		TEST_METHOD(AddRemoveHelpers) {
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);

			auto testParserHelper = std::make_shared<JsonTestParseHelper>();
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().Size());
			parseCoordinator.AddHelper(testParserHelper);
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().Size());

			parseCoordinator.AddHelper(std::make_shared<JsonIntegerParseHelper>());
			Assert::AreEqual(size_t(2), parseCoordinator.Helpers().Size());

			parseCoordinator.RemoveHelper(JsonTestParseHelper::TypeIdClass());
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().Size());

			parseCoordinator.RemoveHelper(JsonTestParseHelper::TypeIdClass());
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().Size());

			parseCoordinator.RemoveHelper(JsonIntegerParseHelper::TypeIdClass());
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().Size());
		}

		TEST_METHOD(FallingThroughChains) {
			const std::string inputString = R"(
			{
				"Test": 100
			})"s;

			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTestParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			Assert::AreEqual(0U, wrapper->Depth());
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(1U, rawWrapper->maxDepth);
		}

		TEST_METHOD(ValueParsingWithTable) {
			std::string inputString = R"(
			{ 
				"Test": {
					"type": "integer",
					"value": 100
				}
			})"s;

			Scope root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);
			Assert::AreEqual(size_t(1), root.Size());
			Assert::AreEqual(root.Find("Test")->GetInt(), 100);
		}

		TEST_METHOD(MultipleValues) {
			std::string inputString = R"(
			{ 
				"Test": {
					"type": "integer",
					"value": 100
				},
				"Test2": {
					"type": "float",
					"value": 1.5
				},
				"Test3": {
					"type": "string",
					"value": "hello"
				}
			})"s;

			Scope root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			Assert::AreEqual(size_t(3), root.Size());
			Assert::AreEqual(root.Find("Test")->GetInt(), 100);
			Assert::AreEqual(root.Find("Test2")->GetFloat(), 1.5f);
			Assert::AreEqual(root.Find("Test3")->GetString(), "hello"s);
		}

		TEST_METHOD(ParseTable) {
			std::string inputString = R"(
			{ 
				"Root": {
					"type": "table",
					"value": {
						"Inner": {
							"type": "string",
							"value": "Hello"
						}
					}
				}
			})"s;

			Scope root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			auto rootScope = root.Find("Root")->GetScope();
			Assert::IsNotNull(rootScope);
			auto innerString = rootScope->Find("Inner");
			Assert::IsNotNull(innerString);
			Assert::AreEqual(innerString->GetString(), "Hello"s);
		}

		TEST_METHOD(ParseTableArray) {
			std::string inputString = R"(
			{ 
				"Root": {
					"type": "table",
					"value": [
						{
							"type": "table",
							"value": {
								"Inner1": {
									"type": "string",
									"value": "Hello"
								}
							}
						},
						{
							"type": "table",
							"value": {
								"Inner2": {
									"type": "integer",
									"value": 1
								}
							}
						},
						{
							"type": "table",
							"value": {
								"Inner3": {
									"type": "float",
									"value": 1.5
								}
							}
						}
					]
				}
			})"s;

			Scope root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			auto rootScope = root.Find("Root");
			Assert::IsNotNull(rootScope);
			Assert::AreEqual(size_t(3), rootScope->Size());

			Assert::IsNotNull(rootScope->GetScope(0));
			Assert::IsNotNull(rootScope->GetScope(1));
			Assert::IsNotNull(rootScope->GetScope(2));

		}

		TEST_METHOD(ParseFactory) {
			std::string inputString = R"(
			{ 
				"Root": {
					"type": "table",
					"class": "AttributedFoo",
					"value":
					{
						"externalInteger":
						{
							"type": "integer",
							"value": 1
						}
					}
				}
			})"s;

			Scope root;
			std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper = std::make_shared<JsonTableParseHelper::Wrapper>(&root);
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(std::make_shared<JsonTableParseHelper>());

			parseCoordinator.DeserializeObject(inputString);

			auto rootScope = root.Find("Root")->GetScope();
			Assert::IsNotNull(rootScope);
			Assert::AreEqual(rootScope->Find("externalInteger")->GetInt(), 1);
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
