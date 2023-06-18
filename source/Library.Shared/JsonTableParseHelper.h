//#pragma once
//#include "IJsonParseHelper.h"
//#include "HashMap.h"
//#include "Datum.h"
//#include "Scope.h"
//#include "Stack.h"
//#include "Factory.h"
//
//namespace FieaGameEngine {
//	//class Scope;
//
//	class JsonTableParseHelper final : public IJsonParseHelper
//	{
//		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper);
//
//	public:
//		class Wrapper final : public JsonParseCoordinator::Wrapper {
//			RTTI_DECLARATIONS(Wrapper, JsonParseCoordinator::Wrapper);
//			friend JsonTableParseHelper;
//
//		public:
//			explicit Wrapper(Scope* root);
//			std::shared_ptr<JsonParseCoordinator::Wrapper> Create() const override;
//
//		private:
//			Scope* root;
//		};
//
//		std::shared_ptr<IJsonParseHelper> Create() const override;
//		void Initialize() override;
//		bool StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, bool isArrayElement, size_t index) override;
//		bool EndHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key) override;
//
//	private:
//		struct ContextFrame {
//			ContextFrame(const std::string& key, Scope* context, Datum* data);
//
//			const std::string& key;
//			Scope* context;
//			Datum* data;
//			std::string classKey;
//		};
//
//		inline static const HashMap<std::string, Datum::DatumTypes> typeMap{
//			{ "integer", Datum::DatumTypes::Integer },
//			{ "float", Datum::DatumTypes::Float },
//			{ "string", Datum::DatumTypes::String },
//			{ "vec4", Datum::DatumTypes::Vector },
//			{ "mat4", Datum::DatumTypes::Matrix },
//			{ "table", Datum::DatumTypes::Table }
//		};
//
//		Stack<ContextFrame> stack;
//
//		std::uint32_t startHandlerCount = 0;
//		std::uint32_t endHandlerCount = 0;
//	};
//}
//
//
