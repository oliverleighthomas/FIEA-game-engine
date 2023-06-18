#pragma once
#include "Datum.h"

namespace FieaGameEngine {
	inline Datum::Datum(std::initializer_list<int32_t> list) {
		InitializerListHelper<int32_t>(list, DatumTypes::Integer);
	}

	inline Datum::Datum(std::initializer_list<float> list) {
		InitializerListHelper<float>(list, DatumTypes::Float);
	}

	inline Datum::Datum(std::initializer_list<std::string> list) {
		InitializerListHelper<std::string>(list, DatumTypes::String);
	}

	inline Datum::Datum(std::initializer_list<glm::vec4> list) {
		InitializerListHelper<glm::vec4>(list, DatumTypes::Vector);
	}

	inline Datum::Datum(std::initializer_list<glm::mat4> list) {
		InitializerListHelper<glm::mat4>(list, DatumTypes::Matrix);
	}

	inline Datum::Datum(std::initializer_list<RTTI*> list) {
		InitializerListHelper<RTTI*>(list, DatumTypes::Pointer);
	}

	inline Datum::Datum(std::initializer_list<Scope*> list) {
		InitializerListHelper<Scope*>(list, DatumTypes::Table);
	}

	inline Datum& Datum::operator=(std::initializer_list<int32_t> list) {
		return InitializerListHelper(list, DatumTypes::Integer);
	}

	inline Datum& Datum::operator=(std::initializer_list<float> list) {
		return InitializerListHelper(list, DatumTypes::Float);
	}

	inline Datum& Datum::operator=(std::initializer_list<std::string> list) {
		return InitializerListHelper(list, DatumTypes::String);
	}

	inline Datum& Datum::operator=(std::initializer_list<glm::vec4> list) {
		return InitializerListHelper(list, DatumTypes::Vector);
	}

	inline Datum& Datum::operator=(std::initializer_list<glm::mat4> list) {
		return InitializerListHelper(list, DatumTypes::Matrix);
	}

	inline Datum& Datum::operator=(std::initializer_list<RTTI*> list) {
		return InitializerListHelper(list, DatumTypes::Pointer);
	}

	inline Datum& Datum::operator=(std::initializer_list<Scope*> list) {
		return InitializerListHelper(list, DatumTypes::Table);
	}

	inline Datum& Datum::operator=(int32_t rhs) {
		return ScalarInitHelper(rhs, DatumTypes::Integer);
	}

	inline Datum& Datum::operator=(float rhs) {
		return ScalarInitHelper(rhs, DatumTypes::Float);
	}

	inline Datum& Datum::operator=(const std::string& rhs) {
		return ScalarInitHelper(rhs, DatumTypes::String);
	}

	inline Datum& Datum::operator=(const glm::vec4& rhs) {
		return ScalarInitHelper(rhs, DatumTypes::Vector);
	}

	inline Datum& Datum::operator=(const glm::mat4& rhs) {
		return ScalarInitHelper(rhs, DatumTypes::Matrix);
	}

	inline Datum& Datum::operator=(RTTI* rhs) {
		return ScalarInitHelper(rhs, DatumTypes::Pointer);
	}

	inline Datum& Datum::operator=(Scope* rhs) {
		return ScalarInitHelper(rhs, DatumTypes::Table);
	}

	inline bool Datum::operator!=(const Datum& rhs) const {
		return !(operator==(rhs));
	}

	inline bool Datum::operator==(int32_t rhs) const {
		return (size == 1 && data.i[0] == rhs);
	}

	inline bool Datum::operator!=(int32_t rhs) const {
		return !(operator==(rhs));
	}

	inline bool Datum::operator==(float rhs) const {
		return (size == 1 && data.f[0] == rhs);
	}

	inline bool Datum::operator!=(float rhs) const {
		return !(operator==(rhs));
	}

	inline bool Datum::operator==(const std::string& rhs) const {
		return (size == 1 && data.s[0] == rhs);
	}

	inline bool Datum::operator!=(const std::string& rhs) const {
		return !(operator==(rhs));
	}

	inline bool Datum::operator==(const glm::vec4& rhs) const {
		return (size == 1 && data.v[0] == rhs);
	}

	inline bool Datum::operator!=(const glm::vec4& rhs) const {
		return !(operator==(rhs));
	}

	inline bool Datum::operator==(const glm::mat4& rhs) const {
		return (size == 1 && data.m[0] == rhs);
	}

	inline bool Datum::operator!=(const glm::mat4& rhs) const {
		return !(operator==(rhs));
	}

	inline bool Datum::operator!=(RTTI* rhs) const {
		return !(operator==(rhs));
	}

	inline bool Datum::operator!=(Scope* rhs) const {
		return !(operator==(rhs));
	}

	inline Scope& Datum::operator[](size_type index) {
		return *GetScope(index);
	}

	inline void Datum::PushBack(int32_t newData) {
		EmplaceBack<int32_t>(DatumTypes::Integer, newData);
	}

	inline void Datum::PushBack(float newData) {
		EmplaceBack<float>(DatumTypes::Float, newData);
	}

	inline void Datum::PushBack(const std::string& newData) {
		EmplaceBack<std::string>(DatumTypes::String, newData);
	}

	inline void Datum::PushBack(const glm::vec4& newData) {
		EmplaceBack<glm::vec4>(DatumTypes::Vector, newData);
	}

	inline void Datum::PushBack(const glm::mat4& newData) {
		EmplaceBack<glm::mat4>(DatumTypes::Matrix, newData);
	}

	inline void Datum::PushBack(RTTI* newData) {
		EmplaceBack<RTTI*>(DatumTypes::Pointer, newData);
	}

	inline void Datum::PushBack(Scope* newData) {
		EmplaceBack<Scope*>(DatumTypes::Table, newData);
	}

	inline Datum::DatumTypes Datum::Type() const {
		return type;
	}

	inline Datum::size_type Datum::Size() const {
		return size;
	}

	inline Datum::size_type Datum::Capacity() const {
		return capacity;
	}

	inline void Datum::Set(int32_t newData, size_type index) {
		SetHelper(newData, DatumTypes::Integer, index);
	}

	inline void Datum::Set(float newData, size_type index) {
		SetHelper(newData, DatumTypes::Float, index);
	}

	inline void Datum::Set(const std::string& newData, size_type index) {
		SetHelper(newData, DatumTypes::String, index);
	}

	inline void Datum::Set(const glm::vec4& newData, size_type index) {
		SetHelper(newData, DatumTypes::Vector, index);
	}

	inline 	void Datum::Set(const glm::mat4& newData, size_type index) {
		SetHelper(newData, DatumTypes::Matrix, index);
	}

	inline 	void Datum::Set(RTTI* newData, size_type index) {
		SetHelper(newData, DatumTypes::Pointer, index);
	}

	inline void Datum::Set(Scope* newData, size_type index) {
		SetHelper(newData, DatumTypes::Table, index);
	}

	inline void Datum::SetStorage(int32_t* newData, size_type dataSize) {
		SetStorageHelper(newData, DatumTypes::Integer, dataSize);
	}

	inline void Datum::SetStorage(float* newData, size_type dataSize) {
		SetStorageHelper(newData, DatumTypes::Float, dataSize);
	}

	inline void Datum::SetStorage(std::string* newData, size_type dataSize) {
		SetStorageHelper(newData, DatumTypes::String, dataSize);
	}

	inline void Datum::SetStorage(glm::vec4* newData, size_type dataSize) {
		SetStorageHelper(newData, DatumTypes::Vector, dataSize);
	}

	inline void Datum::SetStorage(glm::mat4* newData, size_type dataSize) {
		SetStorageHelper(newData, DatumTypes::Matrix, dataSize);
	}

	inline void Datum::SetStorage(RTTI** newData, size_type dataSize) {
		SetStorageHelper(newData, DatumTypes::Pointer, dataSize);
	}

	inline void Datum::SetStorage(Scope** newData, size_type dataSize) {
		SetStorageHelper(newData, DatumTypes::Table, dataSize);
	}

	inline void Datum::SetStorage(void* newData, size_type dataSize) {
		SetStorageHelper(newData, type, dataSize);
	}

	inline int32_t& Datum::GetInt(size_type index) {
		return GetHelper<int32_t>(DatumTypes::Integer, index);
	}

	inline int32_t Datum::GetInt(size_type index) const {
		return GetHelper<int32_t>(DatumTypes::Integer, index);
	}

	inline float& Datum::GetFloat(size_type index) {
		return GetHelper<float>(DatumTypes::Float, index);
	}

	inline float Datum::GetFloat(size_type index) const {
		return GetHelper<float>(DatumTypes::Float, index);
	}

	inline std::string& Datum::GetString(size_type index) {
		return GetHelper<std::string>(DatumTypes::String, index);
	}

	inline const std::string& Datum::GetString(size_type index) const {
		return GetHelper<std::string>(DatumTypes::String, index);
	}

	inline glm::vec4& Datum::GetVector(size_type index) {
		return GetHelper<glm::vec4>(DatumTypes::Vector, index);
	}

	inline const glm::vec4& Datum::GetVector(size_type index) const {
		return GetHelper<glm::vec4>(DatumTypes::Vector, index);
	}

	inline glm::mat4& Datum::GetMatrix(size_type index) {
		return GetHelper<glm::mat4>(DatumTypes::Matrix, index);
	}

	inline const glm::mat4& Datum::GetMatrix(size_type index) const {
		return GetHelper<glm::mat4>(DatumTypes::Matrix, index);
	}

	inline RTTI*& Datum::GetPointer(size_type index) {
		return GetHelper<RTTI*>(DatumTypes::Pointer, index);
	}

	inline RTTI* const& Datum::GetPointer(size_type index) const {
		return GetHelper<RTTI*>(DatumTypes::Pointer, index);
	}

	inline Scope*& Datum::GetScope(size_type index) {
		return GetHelper<Scope*>(DatumTypes::Table, index);
	}

	inline Scope* const& Datum::GetScope(size_type index) const {
		return GetHelper<Scope*>(DatumTypes::Table, index);
	}

	inline int32_t& Datum::FrontInt() {
		return GetInt();
	}

	inline int32_t Datum::FrontInt() const {
		return GetInt();
	}

	inline float& Datum::FrontFloat() {
		return GetFloat();
	}
	
	inline float Datum::FrontFloat() const {
		return GetFloat();
	}
	
	inline std::string& Datum::FrontString() {
		return GetString();
	}
	
	inline const std::string& Datum::FrontString() const {
		return GetString();
	}
	
	inline glm::vec4& Datum::FrontVector() {
		return GetVector();
	}
	
	inline const glm::vec4& Datum::FrontVector() const {
		return GetVector();
	}
	
	inline glm::mat4& Datum::FrontMatrix() {
		return GetMatrix();
	}
	
	inline const glm::mat4& Datum::FrontMatrix() const {
		return GetMatrix();
	}
	
	inline RTTI*& Datum::FrontPointer() {
		return GetPointer();
	}
	
	inline RTTI* const& Datum::FrontPointer() const {
		return GetPointer();
	}
	
	inline Scope*& Datum::FrontScope() {
		return GetScope();
	}
	
	inline Scope* const& Datum::FrontScope() const {
		return GetScope();
	}

	inline int32_t& Datum::BackInt() {
		return (size == 1) ? FrontInt() : GetInt(size - 1);
	}
	
	inline int32_t Datum::BackInt() const {
		return (size == 1) ? FrontInt() : GetInt(size - 1);
	}
	
	inline float& Datum::BackFloat() {
		return (size == 1) ? FrontFloat() : GetFloat(size - 1);
	}
	
	inline float Datum::BackFloat() const {
		return (size == 1) ? FrontFloat() : GetFloat(size - 1);
	}
	
	inline std::string& Datum::BackString() {
		return (size == 1) ? FrontString() : GetString(size - 1);
	}
	
	inline const std::string& Datum::BackString() const {
		return (size == 1) ? FrontString() : GetString(size - 1);
	}
	
	inline glm::vec4& Datum::BackVector() {
		return (size == 1) ? FrontVector() : GetVector(size - 1);
	}
	
	inline const glm::vec4& Datum::BackVector() const {
		return (size == 1) ? FrontVector() : GetVector(size - 1);
	}
	
	inline glm::mat4& Datum::BackMatrix() {
		return (size == 1) ? FrontMatrix() : GetMatrix(size - 1);
	}

	inline const glm::mat4& Datum::BackMatrix() const {
		return (size == 1) ? FrontMatrix() : GetMatrix(size - 1);
	}
	
	inline RTTI*& Datum::BackPointer() {
		return (size == 1) ? FrontPointer() : GetPointer(size - 1);
	}
	
	inline RTTI* const& Datum::BackPointer() const {
		return (size == 1) ? FrontPointer() : GetPointer(size - 1);
	}
	
	inline Scope*& Datum::BackScope() {
		return (size == 1) ? FrontScope() : GetScope(size - 1);
	}

	inline Scope* const& Datum::BackScope() const {
		return (size == 1) ? FrontScope() : GetScope(size - 1);
	}

	inline Datum::size_type Datum::GetIndex(int32_t toFind) const {
		return GetIndexHelper(toFind, DatumTypes::Integer);
	}

	inline Datum::size_type Datum::GetIndex(float toFind) const {
		return GetIndexHelper(toFind, DatumTypes::Float);
	}
	
	inline Datum::size_type Datum::GetIndex(const std::string& toFind) const {
		return GetIndexHelper(toFind, DatumTypes::String);
	}
	
	inline Datum::size_type Datum::GetIndex(const glm::vec4& toFind) const {
		return GetIndexHelper(toFind, DatumTypes::Vector);
	}
	
	inline Datum::size_type Datum::GetIndex(const glm::mat4& toFind) const {
		return GetIndexHelper(toFind, DatumTypes::Matrix);
	}
	
	inline Datum::size_type Datum::GetIndex(RTTI* toFind) const {
		return GetIndexHelper(toFind, DatumTypes::Pointer);
	}
	
	inline Datum::size_type Datum::GetIndex(Scope* toFind) const {
		return GetIndexHelper(toFind, DatumTypes::Table);
	}

	inline void Datum::Remove(int32_t toRemove) {
		RemoveAt(GetIndex(toRemove));
	}
	
	inline void Datum::Remove(float toRemove) {
		RemoveAt(GetIndex(toRemove));
	}
	
	inline void Datum::Remove(const std::string& toRemove) {
		RemoveAt(GetIndex(toRemove));
	}
	
	inline void Datum::Remove(const glm::vec4& toRemove) {
		RemoveAt(GetIndex(toRemove));
	}
	
	inline void Datum::Remove(const glm::mat4& toRemove) {
		RemoveAt(GetIndex(toRemove));
	}
	
	inline void Datum::Remove(RTTI* toRemove) {
		RemoveAt(GetIndex(toRemove));
	}
	
	inline void Datum::Remove(Scope* toRemove) {
		RemoveAt(GetIndex(toRemove));
	}

	inline void Datum::ExternalCheck() {
		if (isExternal) throw std::runtime_error("Cannot change size of external data.");
	}

	inline void Datum::UnknownTypeCheck() {
		if (type == DatumTypes::Unknown) throw std::runtime_error("Cannot modify uninitialized data.");
	}

	template <typename T>
	Datum& Datum::InitializerListHelper(std::initializer_list<T> list, DatumTypes dataType) {
		Clear();
		Resize(0);

		SetType(dataType);
		Reserve(list.size());

		for (auto& obj : list) {
			EmplaceBack<T>(dataType, obj);
		}

		return *this;
	}

	template <typename T>
	Datum& Datum::ScalarInitHelper(const T& data, DatumTypes dataType) {
		SetType(dataType);
		if (size < 1) Resize(1);
		SetHelper(data, dataType, 0);

		return *this;
	}

	template <typename T>
	void Datum::SetHelper(const T& data, DatumTypes dataType, size_type index) {
		GetHelper<T>(dataType, index) = data;
	}

	template<typename T>
	T& Datum::GetHelper(DatumTypes dataType, size_type index) const {
		if (type != dataType) throw std::runtime_error("Type mismatch.");
		if (index >= size) throw std::runtime_error("Index out-of-bounds.");

		return static_cast<T*>(data.vp)[index];
	}

	template <typename T, typename... Args>
	void Datum::EmplaceBack(DatumTypes dataType, Args&&... args) {
		ExternalCheck();

		SetType(dataType);

		if (size == capacity) Reserve(std::max(size_type(1), incrementFunctor(size, capacity)));

		new (static_cast<T*>(data.vp) + size) T(std::forward<Args>(args)...);
		++size;
	}

	template <typename T>
	void Datum::SetStorageHelper(T* newData, DatumTypes dataType, size_type dataSize) {
		if (capacity > 0) throw std::runtime_error("Cannot make internal data into external data.");

		SetType(dataType);
		isExternal = true;
		data.vp = newData;
		size = dataSize;
	}

	template <typename T>
	Datum::size_type Datum::GetIndexHelper(const T& toFind, DatumTypes dataType) const {
		if (type != dataType) throw std::runtime_error("Type mismatch.");

		for (size_type i = 0; i < size; ++i) {
			if (static_cast<T*>(data.vp)[i] == toFind) return i;
		}

		return size;
	}
}
