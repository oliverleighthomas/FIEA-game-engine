#include "Stack.h"

namespace FieaGameEngine {
	template <typename T>
	void Stack<T>::Push(const_reference data) {
		EmplaceBack(data);
	}

	template <typename T>
	void Stack<T>::Push(rvalue_reference data) {
		EmplaceBack(std::move(data));
	}

	template <typename T>
	template <typename... Args>
	void Stack<T>::EmplaceBack(Args&&... args) {
		list.PushBack(std::forward<Args>(args)...);
	}

	template <typename T>
	void Stack<T>::Pop() {
		list.PopBack();
	}

	template <typename T>
	typename Stack<T>::reference Stack<T>::Top() {
		return list.Back();
	}

	template <typename T>
	typename Stack<T>::const_reference Stack<T>::Top() const {
		return list.Back();
	}

	template <typename T>
	void Stack<T>::Size() const {
		return list.Size();
	}

	template <typename T>
	bool Stack<T>::IsEmpty() const {
		return list.IsEmpty();
	}

	template <typename T>
	void Stack<T>::Clear() {
		list.Clear();
	}
}