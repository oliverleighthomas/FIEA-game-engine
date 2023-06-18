#include "pch.h"
#include "JsonIntegerParseHelper.h"

using namespace FieaGameEngine;

namespace UnitTests {
	RTTI_DEFINITIONS(JsonIntegerParseHelper);
	RTTI_DEFINITIONS(JsonIntegerParseHelper::Wrapper);

	void JsonIntegerParseHelper::Wrapper::Initialize() {
	}

	std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonIntegerParseHelper::Wrapper::Create() const {
		return std::make_shared<JsonIntegerParseHelper::Wrapper>();
	}

	std::shared_ptr<IJsonParseHelper> JsonIntegerParseHelper::Create() const {
		return std::make_shared<JsonIntegerParseHelper>();
	}

	void JsonIntegerParseHelper::Initialize() {
		IJsonParseHelper::Initialize();
		initializeCalled = true;
		startHandlerCount = 0;
		endHandlerCount = 0;
	}

	bool JsonIntegerParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, bool isArrayElement, size_t) {
		JsonIntegerParseHelper::Wrapper* customWrapper = wrapper.As<JsonIntegerParseHelper::Wrapper>();
		if (customWrapper == nullptr) return false;

		if (key != integerKey || !value.isInt()) {
			return false;
		}

		isParsing = true;
		++startHandlerCount;

		if (isArrayElement) {
			customWrapper->data.PushBack(value.asInt());
		}
		else {
			if (customWrapper->data.IsEmpty()) {
				customWrapper->data.PushBack(value.asInt());
			}
			else {
				customWrapper->data[0] = value.asInt();
			}
		}

		return true;
	}

	bool JsonIntegerParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key) {
		JsonIntegerParseHelper::Wrapper* customWrapper = wrapper.As<JsonIntegerParseHelper::Wrapper>();
		if (customWrapper == nullptr) return false;

		assert(key == integerKey); UNREFERENCED_LOCAL(key);

		++endHandlerCount;
		isParsing = false;

		return true;
	}

}