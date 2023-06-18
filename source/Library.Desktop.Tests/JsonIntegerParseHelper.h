#pragma once
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "HashMap.h"
#include "vector.h"

namespace UnitTests {
	class JsonIntegerParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, FieaGameEngine::IJsonParseHelper);

	public:
		class Wrapper final : public FieaGameEngine::JsonParseCoordinator::Wrapper {
			RTTI_DECLARATIONS(Wrapper, FieaGameEngine::JsonParseCoordinator::Wrapper);
		public:
			void Initialize() override;
			std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> Create() const override;

			FieaGameEngine::Vector<std::int32_t> data;
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

	private:
		inline static const std::string integerKey{ "integer" };
		bool isParsing = false;
	};
}


