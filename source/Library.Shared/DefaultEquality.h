#pragma once

#include <cstdint>
#include <string.h>
//#include "TypeManager.h"

namespace FieaGameEngine {
	template <typename T>
	struct DefaultEquality final {
		bool operator()(const T& lhs, const T& rhs);
	};

	template <>
	struct DefaultEquality<char*> final {
		bool operator()(const char* lhs, const char* rhs);
	};

	template <>
	struct DefaultEquality<const char*> final {
		bool operator()(const char* lhs, const char* rhs);
	};

	template <>
	struct DefaultEquality<char* const> final {
		bool operator()(const char* const lhs, const char* const rhs);
	};

	template <>
	struct DefaultEquality<const char* const> final {
		bool operator()(const char* const lhs, const char* const rhs);
	};

	//template <>
	//struct DefaultEquality<Signature> final {
	//	//bool operator()(Signature lhs, Signature rhs);
	//};
}

#include "DefaultEquality.inl"