#pragma once

#include <cstdint>

namespace FieaGameEngine {
	template <typename T>
	struct DefaultIncrement final {
		size_t operator()(const T& lhs, const T& rhs);
	};
}

#include "DefaultIncrement.inl"