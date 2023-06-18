#include "pch.h"
//#include "Scope.h"
//#include "JsonTableParseHelper.h"
//
//namespace FieaGameEngine {
//	RTTI_DEFINITIONS(JsonTableParseHelper);
//	RTTI_DEFINITIONS(JsonTableParseHelper::Wrapper);
//
//	JsonTableParseHelper::ContextFrame::ContextFrame(const std::string& key, Scope* context, Datum* data) :
//		key{ key }, context{ context }, data{ data }
//	{
//	}
//
//	JsonTableParseHelper::Wrapper::Wrapper(Scope* root) :
//		root{ root }
//	{
//	}
//
//	std::shared_ptr<FieaGameEngine::JsonParseCoordinator::Wrapper> JsonTableParseHelper::Wrapper::Create() const {
//		return std::make_shared<JsonTableParseHelper::Wrapper>(root->Clone());
//	}
//
//	std::shared_ptr<FieaGameEngine::IJsonParseHelper> JsonTableParseHelper::Create() const {
//		return std::make_shared<JsonTableParseHelper>();
//	}
//
//	void JsonTableParseHelper::Initialize() {
//		IJsonParseHelper::Initialize();
//	}
//
//	bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, bool isArrayElement, size_t index) {
//		JsonTableParseHelper::Wrapper* customWrapper = wrapper.As<JsonTableParseHelper::Wrapper>();
//		if (customWrapper == nullptr) return false;
//
//		++startHandlerCount;
//
//		if (key == "type") {
//			auto iter = typeMap.Find(value.asString());
//			if (iter == typeMap.end()) return false;
//
//			stack.Top().data->SetType(iter->second);
//
//			return true;
//		}
//		else if (key == "class") {
//			stack.Top().classKey = value.asString();
//			return true;
//		}
//		else if (key == "value") {
//			Datum* data = stack.Top().data;
//
//			if (data->Type() == Datum::DatumTypes::Table) {
//				Scope* childScope;
//				if (data != nullptr && data->Size() > index) {
//					childScope = data->GetScope(index);
//				}
//				else {
//					if (stack.Top().classKey.empty()) {
//						childScope = &stack.Top().context->AppendScope(stack.Top().key);
//					}
//					else {
//						childScope = Factory<Scope>::Create(stack.Top().classKey);
//						stack.Top().context->Adopt(*childScope, stack.Top().key);
//					}
//				}
//
//				if (isArrayElement) stack.Push(ContextFrame(key, childScope, data));
//				else stack.Top().context = childScope;
//			}
//
//			else {
//				if (data->Size() <= index) data->Resize(index + 1);
//				data->SetFromString(value.asString(), index);
//			}
//			return true;
//		}
//
//		else if (value.isObject()) {
//			Scope* scope;
//			if (stack.IsEmpty()) scope = customWrapper->root;
//			else scope = stack.Top().context;
//
//			Datum* data = &scope->Append(key);
//			stack.Push(ContextFrame(key, scope, data));
//
//			return true;
//		}
//
//
//		return false;
//	}
//
//	bool JsonTableParseHelper::EndHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key) {
//		JsonTableParseHelper::Wrapper* customWrapper = wrapper.As<JsonTableParseHelper::Wrapper>();
//		if (customWrapper == nullptr) return false;
//
//		++endHandlerCount;
//		auto& topKey = stack.Top().key;
//		if (&topKey == &key) {
//			stack.Pop();
//		}
//
//		return true;
//	}
//}
