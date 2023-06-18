#include "DefaultHash.h"

namespace FieaGameEngine {
	template <typename T>
	inline size_t DefaultHash<T>::operator()(const T& key) const {
		size_t hash = 0;
		const size_t prime = 13;
		const uint8_t* data = reinterpret_cast<const uint8_t*>(&key);

		for (size_t i = 0; i < sizeof(T); i++)
		{
			hash += prime * data[i];
		}

		return hash;
	}

	inline size_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		size_t hash = 0;
		const size_t prime = 13;
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());

		for (size_t i = 0; i < key.length(); ++i) {
			hash += prime * data[i];
		}

		return hash;
	}

	inline size_t DefaultHash<const std::string>::operator()(const std::string& key) const
	{
		size_t hash = 0;
		const size_t prime = 13;
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());

		for (size_t i = 0; i < key.length(); ++i) {
			hash += prime * data[i];
		}

		return hash;
	}

	inline size_t DefaultHash<char*>::operator()(const char* key) const
	{
		size_t hash = 0;
		const size_t prime = 13;
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key);

		for (size_t i = 0; i < strlen(key); ++i) {
			hash += prime * data[i];
		}

		return hash;
	}

	inline size_t DefaultHash<const char*>::operator()(const char* key) const
	{
		size_t hash = 0;
		const size_t prime = 13;
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key);

		for (size_t i = 0; i < strlen(key); ++i) {
			hash += prime * data[i];
		}

		return hash;
	}
}