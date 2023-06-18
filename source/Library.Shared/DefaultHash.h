#pragma once

namespace FieaGameEngine {
	template <typename T>
	struct DefaultHash final {
		size_t operator()(const T& key) const;
	};

	template <>
	struct DefaultHash<std::string> final {
		size_t operator()(const std::string& key) const;
	};

	template <>
	struct DefaultHash<const std::string> final {
		size_t operator()(const std::string& key) const;
	};

	template <>
	struct DefaultHash<char*> final {
		size_t operator()(const char* key) const;
	};

	template <>
	struct DefaultHash<const char*> final {
		size_t operator()(const char* key) const;
	};
	
}

#include "DefaultHash.inl"