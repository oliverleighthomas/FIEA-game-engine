#include "pch.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include <fstream>

namespace FieaGameEngine {
	void JsonParseCoordinator::Wrapper::Initialize() {
		depth = 0;
	}

	uint32_t JsonParseCoordinator::Wrapper::Depth() const {
		return depth;
	}

	void JsonParseCoordinator::Wrapper::IncrementDepth() {
		++depth;
	}

	void JsonParseCoordinator::Wrapper::DecrementDepth() {
		assert(depth > 0);
		--depth;
	}

	JsonParseCoordinator::JsonParseCoordinator(std::shared_ptr<Wrapper> wrapper) :
		wrapper(wrapper)
	{
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& rhs) noexcept :
		wrapper(std::move(rhs.wrapper)), helpers(std::move(rhs.helpers)), isClone(rhs.isClone)
	{
		rhs.wrapper = nullptr;
		rhs.isClone = false;
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& rhs) noexcept {
		if (this != &rhs) {
			if (isClone) {
				wrapper = nullptr;
				helpers.Clear();
			}

			wrapper = std::move(rhs.wrapper);
			helpers = std::move(rhs.helpers);
			isClone = rhs.isClone;

			rhs.wrapper = nullptr;
			rhs.isClone = false;
		}
		return *this;
	}

	JsonParseCoordinator::~JsonParseCoordinator() {
		if (isClone) {
			wrapper = nullptr;
			helpers.Clear();
		}
	}

	void JsonParseCoordinator::Initialize() {
		wrapper->Initialize();
		for (auto& helper : helpers) {
			helper->Initialize();
		}
	}

	std::unique_ptr<JsonParseCoordinator> JsonParseCoordinator::Create() {
		std::unique_ptr<JsonParseCoordinator> clone = std::make_unique<JsonParseCoordinator>(JsonParseCoordinator(wrapper->Create()));
		clone->helpers.Reserve(helpers.Size());
		for (auto& helper : helpers) {
			clone->AddHelper(helper->Create());
		}
		clone->isClone = true;
		return clone;
	}

	Vector<std::shared_ptr<IJsonParseHelper>> JsonParseCoordinator::Helpers() const {
		return helpers;
	}

	void JsonParseCoordinator::AddHelper(std::shared_ptr<IJsonParseHelper> helper) {
		auto pos = std::find_if(helpers.begin(), helpers.end(), [&helper](const std::shared_ptr<IJsonParseHelper>& h) {
			return helper->TypeIdInstance() == h->TypeIdInstance();
			});

		if (pos == helpers.end()) {
			helpers.PushBack(helper);
		}
	}

	void JsonParseCoordinator::RemoveHelper(RTTI::IdType type) {
		auto pos = std::find_if(helpers.begin(), helpers.end(), [type](const std::shared_ptr<IJsonParseHelper>& h) {
			return h->TypeIdInstance() == type;
			});

		helpers.RemoveAt(pos);
	}

	void JsonParseCoordinator::DeserializeObject(const std::string& jsonString) {
		std::stringstream jsonStream(jsonString);
		DeserializeObject(jsonStream);
	}

	void JsonParseCoordinator::DeserializeObject(std::istream& jsonStream) {
		Initialize();
		Json::Value root;
		jsonStream >> root;

		wrapper->IncrementDepth();
		ParseMembers(root);
		wrapper->DecrementDepth();
		// Cleanup
	}

	void JsonParseCoordinator::DeserializeObjectFromFile(const std::string& fileName) {
		std::ifstream file(fileName);
		if (!file.good()) {
			throw std::runtime_error("File not opened properly.");
		}

		DeserializeObject(file);
	}

	JsonParseCoordinator::Wrapper& JsonParseCoordinator::GetWrapper() const {
		return *wrapper;
	}

	void JsonParseCoordinator::SetWrapper(std::shared_ptr<JsonParseCoordinator::Wrapper> newWrapper) {
		wrapper = newWrapper;
	}

	void JsonParseCoordinator::ParseMembers(const Json::Value& object, bool isArrayElement, size_t index) {
		for (auto& name : object.getMemberNames()) {
			Parse(name, object[name], isArrayElement, index);
		}
	}

	void JsonParseCoordinator::Parse(const std::string& key, const Json::Value& value, bool isArrayElement, size_t index) {
		if (value.isObject()) {
			wrapper->IncrementDepth();
			for (auto& helper : helpers) {
				if (helper->StartHandler(*wrapper, key, value, isArrayElement, index)) {
					ParseMembers(value);
					helper->EndHandler(*wrapper, key);
					break;
				}
			}
			wrapper->DecrementDepth();
		}
		else if (value.isArray()) {
			for (unsigned int i = 0; i < value.size(); ++i) {
				if (value[i].isObject()) {
					wrapper->IncrementDepth();
					ParseMembers(value[i], true, i);
					wrapper->DecrementDepth();
				}
				else {
					Parse(key, value[i], true, i);
				}
			}
		}
		else {
			for (auto& helper : helpers) {
				if (helper->StartHandler(*wrapper, key, value, isArrayElement, index)) {
					helper->EndHandler(*wrapper, key);
					break;
				}
			}
		}
	}
}