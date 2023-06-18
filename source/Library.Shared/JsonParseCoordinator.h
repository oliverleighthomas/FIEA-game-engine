#pragma once
#include "RTTI.h"
#include "vector.h"
#include "json/json.h"
#include <memory>

namespace FieaGameEngine {
	class IJsonParseHelper;

	class JsonParseCoordinator final
	{
	public:
		class Wrapper : public RTTI {
			friend JsonParseCoordinator;
		public:
			Wrapper() = default;
			Wrapper(const Wrapper& rhs) = default;
			Wrapper& operator=(const Wrapper& rhs) = default;
			Wrapper(Wrapper&& rhs) = default;
			Wrapper& operator=(Wrapper&& rhs) = default;
			virtual ~Wrapper() = default;

			virtual void Initialize();
			virtual std::shared_ptr<Wrapper> Create() const = 0;

			uint32_t Depth() const;

		private:
			void IncrementDepth();
			void DecrementDepth();
			uint32_t depth = 0;
		};

		JsonParseCoordinator() = delete;
		explicit JsonParseCoordinator(std::shared_ptr<Wrapper> wrapper);
		JsonParseCoordinator(const JsonParseCoordinator& rhs) = delete;
		JsonParseCoordinator& operator=(const JsonParseCoordinator& rhs) = delete;
		JsonParseCoordinator(JsonParseCoordinator&& rhs) noexcept;
		JsonParseCoordinator& operator=(JsonParseCoordinator&& rhs) noexcept;
		~JsonParseCoordinator();
		
		void Initialize();
		std::unique_ptr<JsonParseCoordinator> Create();

		void AddHelper(std::shared_ptr<IJsonParseHelper> helper);
		void RemoveHelper(RTTI::IdType type);
		Vector<std::shared_ptr<IJsonParseHelper>> Helpers() const;

		void DeserializeObject(const std::string& jsonString);
		void DeserializeObject(std::istream& jsonStream);
		void DeserializeObjectFromFile(const std::string& fileName);

		JsonParseCoordinator::Wrapper& GetWrapper() const;
		void SetWrapper(std::shared_ptr<JsonParseCoordinator::Wrapper> wrapper);

	private:
		void ParseMembers(const Json::Value& object, bool isArrayElement = false, size_t index = 0);
		void Parse(const std::string& key, const Json::Value& value, bool isArrayElement = false, size_t index = 0);

		std::shared_ptr<Wrapper> wrapper = nullptr;
		Vector<std::shared_ptr<IJsonParseHelper>> helpers;
		bool isClone = false;
	};
}


