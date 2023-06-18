#pragma once
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"

namespace UnitTests {
	class JsonTestParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper);

	public:
		class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper {
			RTTI_DECLARATIONS(Wrapper, FieaGameEngine::JsonParseCoordinator::Wrapper);

		public:
			void Initialize() override;
			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const override;

			uint32_t maxDepth{ 0 };
		};

		std::shared_ptr<IJsonParseHelper> Create() const override;
		void Initialize() override;
		bool StartHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, bool isArrayElement, size_t index) override;
		bool EndHandler(FieaGameEngine::JsonParseCoordinator::Wrapper& wrapper, const std::string& key) override;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		std::uint32_t startHandlerCount = 0;
		std::uint32_t endHandlerCount = 0;
		std::uint32_t arrayElementCount = 0;
	};
}


