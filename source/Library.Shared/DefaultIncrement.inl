#include "DefaultIncrement.h"

namespace FieaGameEngine {
	template <typename T>
	size_t DefaultIncrement<T>::operator()(const T& lhs, const T& rhs) {
		return lhs + (rhs * 2);
	}
}