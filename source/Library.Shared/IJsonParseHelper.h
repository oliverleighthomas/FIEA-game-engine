#pragma once
#include "RTTI.h"
#include "json/json.h"
#include "gsl/gsl"
#include "JsonParseCoordinator.h"

namespace FieaGameEngine {
	class IJsonParseHelper : public RTTI {
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI);

	public:
		IJsonParseHelper() = default;
		IJsonParseHelper(const IJsonParseHelper& rhs) = delete;
		IJsonParseHelper(IJsonParseHelper&& rhs) = default;
		IJsonParseHelper& operator=(const IJsonParseHelper& rhs) = delete;
		IJsonParseHelper& operator=(IJsonParseHelper&& rhs) = default;
		virtual ~IJsonParseHelper() = default;

		virtual void Initialize() {};
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;
		virtual bool StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, bool isArrayElement, size_t index) = 0;
		virtual bool EndHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key) = 0;
	};
}