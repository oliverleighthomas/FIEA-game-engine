#pragma once
// Headers used in this file //
#include <glm/glm.hpp>
#pragma warning(push)
#pragma warning(disable:4201)
#include "glm/gtx/string_cast.hpp"
#pragma warning(pop)
#include <functional>
#include "DefaultIncrement.h"

namespace FieaGameEngine {
	class RTTI;
	class Scope;

	class Datum final {
		using size_type = std::size_t;

		using CreateFunction = std::function<void(void* data, size_type index)>;
		using EqualityFunction = std::function<bool(const Datum& lhs, const Datum& rhs)>;
		using FromStringFunction = std::function<void(std::string s, size_type index, void* data)>;
		using ToStringFunction = std::function<std::string(size_type index, void* data)>;
		using IncrementFunctor = std::function<size_type(size_type, size_type)>;

	public:
		enum class DatumTypes {
			Unknown,
			Integer,
			Float,
			String,
			Vector,
			Matrix,
			Pointer,
			Table
		};

		explicit Datum(DatumTypes type = DatumTypes::Unknown, size_type capacity = 0, IncrementFunctor incremetFunctor = DefaultIncrement<size_type>{});
		Datum(std::initializer_list<int32_t> list);
		Datum(std::initializer_list<float> list);
		Datum(std::initializer_list<std::string> list);
		Datum(std::initializer_list<glm::vec4> list);
		Datum(std::initializer_list<glm::mat4> list);
		Datum(std::initializer_list<RTTI*> list);
		Datum(std::initializer_list<Scope*> list);
		Datum(const Datum& rhs);
		Datum(Datum&& rhs) noexcept;
		Datum& operator=(const Datum& rhs);
		Datum& operator=(Datum&& rhs) noexcept;
		Datum& operator=(std::initializer_list<int32_t> list);
		Datum& operator=(std::initializer_list<float> list);
		Datum& operator=(std::initializer_list<std::string> list);
		Datum& operator=(std::initializer_list<glm::vec4> list);
		Datum& operator=(std::initializer_list<glm::mat4> list);
		Datum& operator=(std::initializer_list<RTTI*> list);
		Datum& operator=(std::initializer_list<Scope*> list);
		Datum& operator=(int32_t rhs);
		Datum& operator=(float rhs);
		Datum& operator=(const std::string& rhs);
		Datum& operator=(const glm::vec4& rhs);
		Datum& operator=(const glm::mat4& rhs);
		Datum& operator=(RTTI* rhs);
		Datum& operator=(Scope* rhs);
		~Datum();

		[[nodiscard]] bool operator==(const Datum& rhs) const;
		[[nodiscard]] bool operator!=(const Datum& rhs) const;
		[[nodiscard]] bool operator==(int32_t rhs) const;
		[[nodiscard]] bool operator!=(int32_t rhs) const;
		[[nodiscard]] bool operator==(float rhs) const;
		[[nodiscard]] bool operator!=(float rhs) const;
		[[nodiscard]] bool operator==(const std::string& rhs) const;
		[[nodiscard]] bool operator!=(const std::string& rhs) const;
		[[nodiscard]] bool operator==(const glm::vec4& rhs) const;
		[[nodiscard]] bool operator!=(const glm::vec4& rhs) const;
		[[nodiscard]] bool operator==(const glm::mat4& rhs) const;
		[[nodiscard]] bool operator!=(const glm::mat4& rhs) const;
		[[nodiscard]] bool operator==(RTTI* rhs) const;
		[[nodiscard]] bool operator!=(RTTI* rhs) const;
		[[nodiscard]] bool operator==(Scope* rhs) const;
		[[nodiscard]] bool operator!=(Scope* rhs) const;

		[[nodiscard]] Scope& operator[](size_type index);

		void PushBack(int32_t newData);
		void PushBack(float newData);
		void PushBack(const std::string& newData);
		void PushBack(const glm::vec4& newData);
		void PushBack(const glm::mat4& newData);
		void PushBack(RTTI* newData);
		void PushBack(Scope* newData);

		void Set(int32_t newData, size_type index = 0);
		void Set(float newData, size_type index = 0);
		void Set(const std::string& newData, size_type index = 0);
		void Set(const glm::vec4& newData, size_type index = 0);
		void Set(const glm::mat4& newData, size_type index = 0);
		void Set(RTTI* newData, size_type index = 0);
		void Set(Scope* newData, size_type index = 0);

		void SetStorage(int32_t* newData, size_type dataSize);
		void SetStorage(float* newData, size_type dataSize);
		void SetStorage(std::string* newData, size_type dataSize);
		void SetStorage(glm::vec4* newData, size_type dataSize);
		void SetStorage(glm::mat4* newData, size_type dataSize);
		void SetStorage(RTTI** newData, size_type dataSize);
		void SetStorage(Scope** newData, size_type dataSize);
		void SetStorage(void* newData, size_type dataSize);

		[[nodiscard]] int32_t& GetInt(size_type index = 0);
		[[nodiscard]] int32_t GetInt(size_type index = 0) const;
		[[nodiscard]] float& GetFloat(size_type index = 0);
		[[nodiscard]] float GetFloat(size_type index = 0) const;
		[[nodiscard]] std::string& GetString(size_type index = 0);
		[[nodiscard]] const std::string& GetString(size_type index = 0) const;
		[[nodiscard]] glm::vec4& GetVector(size_type index = 0);
		[[nodiscard]] const glm::vec4& GetVector(size_type index = 0) const;
		[[nodiscard]] glm::mat4& GetMatrix(size_type index = 0);
		[[nodiscard]] const glm::mat4& GetMatrix(size_type index = 0) const;
		[[nodiscard]] RTTI*& GetPointer(size_type index = 0);
		[[nodiscard]] RTTI* const& GetPointer(size_type index = 0) const;
		[[nodiscard]] Scope*& GetScope(size_type index = 0);
		[[nodiscard]] Scope* const& GetScope(size_type index = 0) const;

		[[nodiscard]] int32_t& FrontInt();
		[[nodiscard]] int32_t FrontInt() const;
		[[nodiscard]] float& FrontFloat();
		[[nodiscard]] float FrontFloat() const;
		[[nodiscard]] std::string& FrontString();
		[[nodiscard]] const std::string& FrontString() const;
		[[nodiscard]] glm::vec4& FrontVector();
		[[nodiscard]] const glm::vec4& FrontVector() const;
		[[nodiscard]] glm::mat4& FrontMatrix();
		[[nodiscard]] const glm::mat4& FrontMatrix() const;
		[[nodiscard]] RTTI*& FrontPointer();
		[[nodiscard]] RTTI* const& FrontPointer() const;
		[[nodiscard]] Scope*& FrontScope();
		[[nodiscard]] Scope* const& FrontScope() const;

		[[nodiscard]] int32_t& BackInt();
		[[nodiscard]] int32_t BackInt() const;
		[[nodiscard]] float& BackFloat();
		[[nodiscard]] float BackFloat() const;
		[[nodiscard]] std::string& BackString();
		[[nodiscard]] const std::string& BackString() const;
		[[nodiscard]] glm::vec4& BackVector();
		[[nodiscard]] const glm::vec4& BackVector() const;
		[[nodiscard]] glm::mat4& BackMatrix();
		[[nodiscard]] const glm::mat4& BackMatrix() const;
		[[nodiscard]] RTTI*& BackPointer();
		[[nodiscard]] RTTI* const& BackPointer() const;
		[[nodiscard]] Scope*& BackScope();
		[[nodiscard]] Scope* const& BackScope() const;

		[[nodiscard]] size_type GetIndex(int32_t toFind) const;
		[[nodiscard]] size_type GetIndex(float toFind) const;
		[[nodiscard]] size_type GetIndex(const std::string& toFind) const;
		[[nodiscard]] size_type GetIndex(const glm::vec4& toFind) const;
		[[nodiscard]] size_type GetIndex(const glm::mat4& toFind) const;
		[[nodiscard]] size_type GetIndex(RTTI* toFind) const;
		[[nodiscard]] size_type GetIndex(Scope* toFind) const;

		void PopBack();

		void Remove(int32_t toRemove);
		void Remove(float toRemove);
		void Remove(const std::string& toRemove);
		void Remove(const glm::vec4& toRemove);
		void Remove(const glm::mat4& toRemove);
		void Remove(RTTI* toRemove);
		void Remove(Scope* toRemove);

		void RemoveAt(size_type index);

		[[nodiscard]] DatumTypes Type() const;
		[[nodiscard]] size_type Size() const;
		[[nodiscard]] size_type Capacity() const;

		void SetType(DatumTypes newType);

		void SetFromString(const std::string& stringData, size_type index = 0);
		[[nodiscard]] std::string ToString(size_type index = 0) const;

		void Reserve(size_type newCapacity);

		void Resize(size_type newSize);
		void Clear();

	private:
		union DatumValues final {
			int32_t* i;
			float* f;
			std::string* s;
			glm::vec4* v;
			glm::mat4* m;
			RTTI** p;
			Scope** t;

			char* b;
			void* vp{ nullptr };
		};

		/// <summary>
		/// A look-up table for the datum's types' sizes.
		/// </summary>
		static constexpr size_t Sizes[] = {
			0,
			sizeof(int32_t),
			sizeof(float),
			sizeof(std::string),
			sizeof(glm::vec4),
			sizeof(glm::mat4),
			sizeof(RTTI*),
			sizeof(Scope*)
		};
		/// <summary>
		/// A look-up table for the default constructors for the datum's types.
		/// </summary>
		static const CreateFunction Create[];
		/// <summary>
		/// A look-up table for the equality functions for the datum's types.
		/// </summary>
		static const EqualityFunction Equality[];
		/// <summary>
		/// A look-up table for converting the datum's types into strings.
		/// </summary>
		static const ToStringFunction ToStrings[];
		/// <summary>
		/// A look-up table for parsing strings into the datum's types.
		/// </summary>
		static const FromStringFunction FromString[];

		template <typename T>
		Datum& InitializerListHelper(std::initializer_list<T> list, DatumTypes dataType);

		template <typename T>
		Datum& ScalarInitHelper(const T& data, DatumTypes dataType);

		template <typename T>
		void SetHelper(const T& data, DatumTypes dataType, size_type index);

		template <typename T>
		T& GetHelper(DatumTypes dataType, size_type index) const;

		template <typename T, typename... Args>
		void EmplaceBack(DatumTypes dataType, Args&&... args);

		template <typename T>
		void SetStorageHelper(T* data, DatumTypes dataType, size_type dataSize);

		template <typename T>
		size_type GetIndexHelper(const T& toFind, DatumTypes dataType) const;

		void ExternalCheck();
		void UnknownTypeCheck();

		DatumValues data;
		size_type size{ 0 };
		size_type capacity{ 0 };
		DatumTypes type{ DatumTypes::Unknown };
		bool isExternal{ false };
		IncrementFunctor incrementFunctor{ DefaultIncrement<size_type>() };
	};
}

#include "Datum.inl"