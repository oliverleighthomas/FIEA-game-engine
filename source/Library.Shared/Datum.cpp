#include "pch.h"
#include "Datum.h"
#include "RTTI.h"
#include "Scope.h"

namespace FieaGameEngine {
	const Datum::CreateFunction Datum::Create[] = {
		nullptr,
		[](void* data, size_type index) { static_cast<int32_t*>(data)[index] = int32_t(); },
		[](void* data, size_type index) { static_cast<float*>(data)[index] = float(); },
		[](void* data, size_type index) { new(static_cast<std::string*>(data) + index) std::string(); },
		[](void* data, size_type index) { static_cast<glm::vec4*>(data)[index] = glm::vec4(); },
		[](void* data, size_type index) { static_cast<glm::mat4*>(data)[index] = glm::mat4(); },
		[](void* data, size_type index) { new(static_cast<RTTI**>(data) + index) RTTI*(); },
		[](void* data, size_type index) { new(static_cast<Scope**>(data) + index) Scope*(); },
	};

	const Datum::EqualityFunction Datum::Equality[] = {
		nullptr,
		[](const Datum& lhs, const Datum& rhs) { return !memcmp(lhs.data.i, rhs.data.i, lhs.size); },
		[](const Datum& lhs, const Datum& rhs) { return !memcmp(lhs.data.f, rhs.data.f, lhs.size); },
		[](const Datum& lhs, const Datum& rhs) {
			for (size_type i = 0; i < lhs.size; ++i) {
				if (lhs.data.s[i] != rhs.data.s[i]) return false;
			}
			return true; },
		[](const Datum& lhs, const Datum& rhs) { return !memcmp(lhs.data.v, rhs.data.v, lhs.size); },
		[](const Datum& lhs, const Datum& rhs) { return !memcmp(lhs.data.m, rhs.data.m, lhs.size); },
		[](const Datum& lhs, const Datum& rhs) {
			for (size_type i = 0; i < lhs.size; ++i) {
				if (lhs.data.p[i] != rhs.data.p[i]) {
					if (lhs.data.p[i] == nullptr || !lhs.data.p[i]->Equals(rhs.data.p[i])) return false;
				}
			}
			return true; },
		[](const Datum& lhs, const Datum& rhs) {
			for (size_type i = 0; i < lhs.size; ++i) {
				if (lhs.data.t[i] != rhs.data.t[i]) {
					if (lhs.data.t[i] == nullptr || !lhs.data.t[i]->Equals(rhs.data.t[i])) return false;
				}
			}
			return true; }
	};

	const Datum::ToStringFunction Datum::ToStrings[] = {
		nullptr,
		[](size_type index, void* data) { return std::to_string(static_cast<int32_t*>(data)[index]); },
		[](size_type index, void* data) { return std::to_string(static_cast<float*>(data)[index]); },
		[](size_type index, void* data) { return static_cast<std::string*>(data)[index]; },
		[](size_type index, void* data) { return glm::to_string(static_cast<glm::vec4*>(data)[index]); },
		[](size_type index, void* data) { return glm::to_string(static_cast<glm::mat4*>(data)[index]);  },
		[](size_type index, void* data) { return static_cast<RTTI**>(data)[index]->ToString(); },
		[](size_type index, void* data) { return static_cast<Scope**>(data)[index]->ToString(); },
	};

	const Datum::FromStringFunction Datum::FromString[] = {
		nullptr,
		[](std::string s, size_type index, void* data) { static_cast<int32_t*>(data)[index] = stoi(s); },
		[](std::string s, size_type index, void* data) { static_cast<float*>(data)[index] = stof(s); },
		[](std::string s, size_type index, void* data) { static_cast<std::string*>(data)[index] = s; },
		[](std::string s, size_type index, void* data) {
			glm::vec4 vector;
			int32_t count = sscanf_s(s.c_str(), "vec4(%f, %f, %f, %f)",
				&vector[0], &vector[1], &vector[2], &vector[3]);
			assert(count == 4); UNREFERENCED_LOCAL(count);
			static_cast<glm::vec4*>(data)[index] = vector;
		},
		[](std::string s, size_type index, void* data) {
			glm::mat4 matrix;
			int32_t count = sscanf_s(s.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
					&matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3],
					&matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3],
					&matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3],
					&matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3]);
			assert(count == 16); UNREFERENCED_LOCAL(count);
			static_cast<glm::mat4*>(data)[index] = matrix;
		},
		[](std::string, size_type, void*) { 
#ifdef USE_EXCEPTIONS
			throw std::runtime_error("FromString for a pointer is not supported."); 
#endif
		},
		[](std::string, size_type, void*) {
#ifdef USE_EXCEPTIONS
			throw std::runtime_error("FromString for a table is not supported.");
#endif
		}
	};

	Datum::Datum(DatumTypes type, size_type capacity, IncrementFunctor incrementFunctor) :
		type{ type }, incrementFunctor{ incrementFunctor }
	{
		Reserve(capacity);
	}

	Datum::Datum(const Datum& rhs) :
		size{ rhs.size }, type{ rhs.type }, isExternal{ rhs.isExternal }
	{
		if (isExternal) data.vp = rhs.data.vp;
		else {
			Reserve(rhs.capacity);
			if (type == DatumTypes::String) {
				for (size_type i = 0; i < size; ++i) {
					new(data.s + i) std::string(rhs.data.s[i]);
				}
			}
			else {
				memcpy(data.vp, rhs.data.vp, size * Sizes[static_cast<int32_t>(type)]);
			}
		}
	}

	Datum::Datum(Datum&& rhs) noexcept :
		data{ rhs.data }, size { rhs.size }, capacity{ rhs.capacity }, type{ rhs.type }, isExternal{ rhs.isExternal }
	{
		rhs.data.vp = nullptr;
		rhs.size = 0;
		rhs.capacity = 0;
		rhs.type = DatumTypes::Unknown;
		rhs.isExternal = false;
	}

	Datum& Datum::operator=(const Datum& rhs) {
		if (this != &rhs) {
			Clear();
			Resize(0);

			SetType(rhs.type);
			size = rhs.size;
			incrementFunctor = rhs.incrementFunctor;

			if (rhs.isExternal) {
				data.vp = rhs.data.vp;
			}
			else {
				Reserve(rhs.capacity);
				if (type == DatumTypes::String) {
					for (size_type i = 0; i < size; ++i) {
						new(data.s + i) std::string(rhs.data.s[i]);
					}
				}
				else {
					memcpy(data.vp, rhs.data.vp, size * Sizes[static_cast<int32_t>(type)]);
				}
			}

			isExternal = rhs.isExternal;
		}

		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs) noexcept {
		if (this != &rhs) {
			Clear();
			Resize(0);

			SetType(rhs.type);
			data.vp = rhs.data.vp;
			size = rhs.size;
			capacity = rhs.capacity;
			isExternal = rhs.isExternal;
			incrementFunctor = rhs.incrementFunctor;

			rhs.data.vp = nullptr;
			rhs.size = 0;
			rhs.capacity = 0;
			rhs.type = DatumTypes::Unknown;
			rhs.isExternal = false;
		}

		return *this;
	}

	Datum::~Datum() {
		Clear();
		if (!isExternal) {
			Resize(0);
		}
	}

	bool Datum::operator==(const Datum& rhs) const {
		if (type == rhs.type && size == rhs.size) {
			if (type == DatumTypes::Unknown) return true;

			EqualityFunction equalityFunction = Equality[static_cast<int32_t>(type)];
			assert(equalityFunction != nullptr);
			return equalityFunction(*this, rhs);
		}
		return false;
	}

	bool Datum::operator==(RTTI* rhs) const {
		if (size == 1) {
			return data.p[0] != nullptr && data.p[0]->Equals(rhs);
		}
		return false;
	}

	bool Datum::operator==(Scope* rhs) const {
		if (size == 1) {
			return data.p[0] != nullptr && data.p[0]->Equals(rhs);
		}
		return false;
	}

	void Datum::PopBack() {
#ifdef USE_EXCEPTIONS
		ExternalCheck();
		if (size <= 0) throw std::runtime_error("Datum is empty");
#endif

		if (type == DatumTypes::String) data.s[size - 1].~basic_string();

		--size;
	}

	void Datum::RemoveAt(size_type index) {
#ifdef USE_EXCEPTIONS
		ExternalCheck();
		if (index >= size) throw std::runtime_error("Value does not exist.");
#endif

		if (type == DatumTypes::String) data.s[index].~basic_string();

		size_t dataSize = Sizes[static_cast<int32_t>(type)];
		memmove(data.b + (index * dataSize),
				data.b + ((index + 1) * dataSize),
				(size - index - 1) * dataSize);

		--size;
	}

	void Datum::SetType(DatumTypes newType) {
		if (type == newType) return;

#ifdef USE_EXCEPTIONS
		if (newType == DatumTypes::Unknown) throw std::runtime_error("Cannot change to unknown type.");
		if (type != DatumTypes::Unknown) throw std::runtime_error("Cannot change type post-initialization.");
#endif

		type = newType;
	}

	void Datum::SetFromString(const std::string& stringData, size_type index) {
#ifdef USE_EXCEPTIONS
		if (index >= size) throw std::runtime_error("Index out of bounds.");
#endif

		FromStringFunction fromStringFunction = FromString[static_cast<int32_t>(type)];
		assert(fromStringFunction != nullptr);
		fromStringFunction(stringData, index, data.vp);
	}

	std::string Datum::ToString(size_type index) const {
#ifdef USE_EXCEPTIONS
		if (index >= size) throw std::runtime_error("Index out of bounds.");
#endif

		ToStringFunction toStringFunction = ToStrings[static_cast<int32_t>(type)];
		assert(toStringFunction != nullptr);
		return toStringFunction(index, data.vp);
	}

	void Datum::Reserve(size_type newCapacity) {
		if (newCapacity > capacity) {
#ifdef USE_EXCEPTIONS
			UnknownTypeCheck();
			ExternalCheck();
#endif
		
			void* newData = realloc(data.vp, newCapacity * Sizes[static_cast<int32_t>(type)]);
			assert(newData != nullptr);
			data.vp = newData;
			capacity = newCapacity;
		}
	}

	void Datum::Resize(size_type newSize) {
#ifdef USE_EXCEPTIONS
		ExternalCheck();
#endif
		if (newSize == 0) {
			Clear();
			free(data.vp);
			data.vp = nullptr;
		}
		else if (newSize < size) {
			if (type == DatumTypes::String) {
				for (size_type i = newSize; i < size; ++i) {
					data.s[i].~basic_string();
				}
			}
			void* newData = realloc(data.vp, newSize * Sizes[static_cast<int32_t>(type)]);
			assert(newData != nullptr);
			data.vp = newData;
		}

		else if (newSize > size) {
			if (newSize > capacity) Reserve(newSize);

			for (size_type i = size; i < newSize; ++i) {
				CreateFunction createFunction = Create[static_cast<int32_t>(type)];
				assert(createFunction != nullptr);
				createFunction(data.vp, i);
			}
		}

		capacity = size = newSize;
	}

	void Datum::Clear() {
		if (isExternal) data.vp = nullptr;
		else if (type == DatumTypes::String) {
			for (size_type i = 0; i < size; ++i) {
				data.s[i].~basic_string();
			}
		}

		isExternal = false;
		size = 0;
	}
}