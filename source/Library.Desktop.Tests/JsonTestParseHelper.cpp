#include "pch.h"
#include "JsonTestParseHelper.h"

using namespace FieaGameEngine;

namespace UnitTests {
	RTTI_DEFINITIONS(JsonTestParseHelper);
	RTTI_DEFINITIONS(JsonTestParseHelper::Wrapper);

	void JsonTestParseHelper::Wrapper::Initialize() {
		JsonParseCoordinator::Wrapper::Initialize();
		maxDepth = 0;
	}

	std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonTestParseHelper::Wrapper::Create() const {
		return std::make_shared<JsonTestParseHelper::Wrapper>();
	}

	std::shared_ptr<IJsonParseHelper> JsonTestParseHelper::Create() const {
		return std::make_shared<JsonTestParseHelper>();
	}

	void JsonTestParseHelper::Initialize() {
		IJsonParseHelper::Initialize();
		initializeCalled = true;
		startHandlerCount = 0;
		endHandlerCount = 0;
	}

	bool JsonTestParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string&, const Json::Value&, bool, size_t) {
		JsonTestParseHelper::Wrapper* customWrapper = wrapper.As<JsonTestParseHelper::Wrapper>();
		if (customWrapper == nullptr) return false;

		++startHandlerCount;

		if (customWrapper->Depth() > customWrapper->maxDepth) {
			customWrapper->maxDepth = customWrapper->Depth();
		}

		return true;
	}

	bool JsonTestParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key) {
		JsonTestParseHelper::Wrapper* customWrapper = wrapper.As<JsonTestParseHelper::Wrapper>();
		if (customWrapper == nullptr) return false;

		++endHandlerCount;

		UNREFERENCED_LOCAL(key);

		return true;
	}

}