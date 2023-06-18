#include "DefaultEquality.h"

namespace FieaGameEngine {
	template <typename T>
	inline bool DefaultEquality<T>::operator()(const T& lhs, const T& rhs) {
		return lhs == rhs;
	}

	inline bool DefaultEquality<char*>::operator()(const char* lhs, const char* rhs) {
		return strcmp(lhs, rhs) == 0;
	}

	inline bool DefaultEquality<const char*>::operator()(const char* lhs, const char* rhs) {
		return strcmp(lhs, rhs) == 0;
	}

	inline bool DefaultEquality<char* const>::operator()(const char* const lhs, const char* const rhs) {
		return strcmp(lhs, rhs) == 0;
	}

	inline bool DefaultEquality<const char* const>::operator()(const char* const lhs, const char* const rhs) {
		return strcmp(lhs, rhs) == 0;
	}
}