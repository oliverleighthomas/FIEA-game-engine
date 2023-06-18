#pragma once
#include "vector.h"

namespace FieaGameEngine {

	template <typename T>
	Vector<T>::Iterator::Iterator(const Vector& owner, size_type index) :
		owner{ &owner }, index{ index }
	{
	}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const Vector& owner, size_type index) :
		owner{ &owner }, index{ index } 
	{
	}

	template <typename T>
	Vector<T>::Vector(size_type initialCapacity, IncrementFunctor incrementFunctor) :
		incrementFunctor{ incrementFunctor }
	{
		Reserve(initialCapacity);
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<value_type> list) {
		Reserve(list.size());
		for (auto& data : list) {
			EmplaceBack(data);
		}
	}

	template <typename T>
	Vector<T>::Vector(const Vector& rhs) :
		incrementFunctor{ rhs.incrementFunctor }
	{
		Reserve(rhs.size);
		for (auto& data : rhs) {
			PushBack(data);
		}
	}

	template <typename T>
	Vector<T>::Vector(Vector&& rhs) noexcept :
		dataStart{ std::move(rhs.dataStart) }, size{ rhs.size }, capacity{ rhs.capacity }, incrementFunctor{ rhs.incrementFunctor }
	{
		rhs.dataStart = nullptr;
		rhs.size = 0;
		rhs.capacity = 0;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& rhs) {
		if (this != &rhs) {
			Clear();
			ShrinkToFit();
			incrementFunctor = rhs.incrementFunctor;

			Reserve(rhs.size);
			
			for (auto& data : rhs) {
				EmplaceBack(data);
			}
		}
		return *this;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept {
		if (this != &rhs) {
			Clear();
			ShrinkToFit();

			dataStart = std::move(rhs.dataStart);
			size = rhs.size;
			capacity = rhs.capacity;
			incrementFunctor = rhs.incrementFunctor;

			rhs.dataStart = nullptr;
			rhs.size = 0;
			rhs.capacity = 0;
		}

		return *this;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(std::initializer_list<value_type> list) {
		Clear();
		ShrinkToFit();
		Reserve(list.size());
		for (auto& data : list) {
			EmplaceBack(data);
		}

		return *this;
	}

	template <typename T>
	Vector<T>::~Vector() {
		Clear();
		ShrinkToFit();
	}

	template <typename T>
	bool Vector<T>::operator==(const Vector<T>& rhs) const {
		if (this == &rhs) return true;
		if (size != rhs.size) return false;

		return std::equal(begin(), end(), rhs.begin());
	}

	template <typename T>
	bool Vector<T>::operator!=(const Vector<T>& rhs) const {
		return !(operator==(rhs));
	}

	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& rhs) const {
		return (owner == rhs.owner && index == rhs.index);
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator==(const ConstIterator& rhs) const {
		return (owner == rhs.owner && index == rhs.index);
	}

	template <typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const {
		return !(operator==(rhs));
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator!=(const ConstIterator& rhs) const {
		return !(operator==(rhs));
	}

	template <typename T>
	bool Vector<T>::Iterator::operator>(const Iterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner != rhs.owner) throw std::runtime_error("Cannot compare iterators from two different lists!");
#endif
		return index > rhs.index;
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator>(const ConstIterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner != rhs.owner) throw std::runtime_error("Cannot compare iterators from two different lists!");
#endif
		return index > rhs.index;
	}

	template <typename T>
	bool Vector<T>::Iterator::operator>=(const Iterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner != rhs.owner) throw std::runtime_error("Cannot compare iterators from two different lists!");
#endif
		return index >= rhs.index;
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator>=(const ConstIterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner != rhs.owner) throw std::runtime_error("Cannot compare iterators from two different lists!");
#endif
		return index >= rhs.index;
	}

	template <typename T>
	bool Vector<T>::Iterator::operator<(const Iterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner != rhs.owner) throw std::runtime_error("Cannot compare iterators from two different lists!");
#endif
		return index < rhs.index;
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator<(const ConstIterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner != rhs.owner) throw std::runtime_error("Cannot compare iterators from two different lists!");
#endif
		return index < rhs.index;
	}

	template <typename T>
	bool Vector<T>::Iterator::operator<=(const Iterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner != rhs.owner) throw std::runtime_error("Cannot compare iterators from two different lists!");
#endif
		return index <= rhs.index;
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator<=(const ConstIterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner != rhs.owner) throw std::runtime_error("Cannot compare iterators from two different lists!");
#endif
		return index <= rhs.index;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++() {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		if (index < owner->size) ++index;

		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++() {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		if (index < owner->size) ++index;

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int) {
		Iterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int) {
		ConstIterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--() {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		if (index > 0) --index;

		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--() {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif
		if (index > 0) --index;

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int) {
		Iterator it = *this;
		operator--();
		return it;
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int) {
		ConstIterator it = *this;
		operator--();
		return it;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator+=(difference_type increment) {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		index = std::min(owner->size, index + increment);
		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator+=(difference_type increment) {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		index = std::min(owner->size, index + increment);
		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator::difference_type Vector<T>::Iterator::operator+(const Iterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
		if (owner != rhs.owner) throw std::runtime_error("Cannot complete expression with iterators from different containers.");
#endif
		auto diff = std::min(owner->size, index + rhs.index);
		return diff;
	}

	template <typename T>
	typename Vector<T>::ConstIterator::difference_type Vector<T>::ConstIterator::operator+(const ConstIterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
		if (owner != rhs.owner) throw std::runtime_error("Cannot complete expression with iterators from different containers.");
#endif

		auto diff = std::min(owner->size, index + rhs.index);
		return diff;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator-=(difference_type decrement) {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		index = std::max(size_t(0), index - decrement);
		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator-=(difference_type decrement) {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		index = std::max(size_t(0), index - decrement);
		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator::difference_type Vector<T>::Iterator::operator-(const Iterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
		if (owner != rhs.owner) throw std::runtime_error("Cannot complete expression with iterators from different containers.");
#endif

		auto diff = std::max(size_t(0), index - rhs.index);
		return diff;
	}

	template <typename T>
	typename Vector<T>::ConstIterator::difference_type Vector<T>::ConstIterator::operator-(const ConstIterator& rhs) const {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
		if (owner != rhs.owner) throw std::runtime_error("Cannot complete expression with iterators from different containers.");
#endif

		auto diff = std::max(size_t(0), index - rhs.index);
		return diff;
	}

	template <typename T>
	inline typename Vector<T>::reference Vector<T>::Iterator::operator[](difference_type newIndex) {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif
		index = newIndex;
		return owner->dataStart[index];
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::ConstIterator::operator[](difference_type newIndex) {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif
		index = newIndex;
		return owner->dataStart[index];
	}

	template <typename T>
	inline typename Vector<T>::reference Vector<T>::Iterator::operator*() const {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
		if (index >= owner->size) throw std::runtime_error("Cannot dereference nullptr.");
#endif

		return owner->dataStart[index];
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::ConstIterator::operator*() const {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
		if (index >= owner->size) throw std::runtime_error("Cannot dereference nullptr.");
#endif

		return owner->dataStart[index];
	}

	template <typename T>
	typename Vector<T>::reference Vector<T>::operator[](size_type index) {
#ifdef USE_EXCEPTIONS
		if (index >= size || index < 0) throw std::runtime_error("Invalid index.");
#endif
		return dataStart[index];
	}

	template <typename T>
	typename Vector<T>::const_reference Vector<T>::operator[](size_type index) const {
#ifdef USE_EXCEPTIONS
		if (index >= size || index < 0) throw std::runtime_error("Invalid index.");
#endif
		return dataStart[index];
	}

	template <typename T>
	void Vector<T>::PushBack(const_reference data) {
		EmplaceBack(data);
	}

	template <typename T>
	void Vector<T>::PushBack(rvalue_reference data) {
		EmplaceBack(std::move(data));
	}

	template <typename T>
	template <typename... Args>
	void Vector<T>::EmplaceBack(Args&&... args) {
		if (size == capacity) Reserve(std::max(size_type(1), incrementFunctor(size, capacity)));
		new (dataStart + size) value_type(std::forward<Args>(args)...);
		++size;
	}

	template <typename T>
	inline void Vector<T>::PopBack() {
		Back().~value_type();
		--size;
	}

	template <typename T>
	bool Vector<T>::Remove(const_reference data, EqualityFunctor equalityFunctor) {
		return RemoveAt(Find(data, equalityFunctor).index);
	}

	template <typename T>
	bool Vector<T>::RemoveAt(const Iterator& position) {
#ifdef USE_EXCEPTIONS
		if (position.owner != this) throw std::runtime_error("Cannot remove using iterator from different container.");
#endif
		return RemoveAt(position.index);
	}

	template <typename T>
	bool Vector<T>::RemoveAt(size_type index) {
		if (index < size && index >= 0) {
			dataStart[index].~T();
			memmove(dataStart + index, dataStart + index + 1, sizeof(value_type) * (size - index - 1));
			--size;
			return true;
		}
		return false;
	}

	template <typename T>
	bool Vector<T>::RemoveRange(Iterator begin, Iterator end) {
#ifdef USE_EXCEPTIONS
		if (begin > end) throw std::runtime_error("Invalid range.");
		if (begin.owner != this || end.owner != this) throw std::runtime_error("Cannot remove using iterator from different container.");
#endif

		for (size_type i = begin.index; i < end.index; ++i) {
			dataStart[i].~T();
		}

		memmove(dataStart + begin.index, dataStart + end.index, sizeof(value_type) * (size - end.index));
		size -= (end.index - begin.index);

		return true;
	}

	template <typename T>
	inline typename Vector<T>::reference Vector<T>::Front() {
		return operator[](0);
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::Front() const {
		return operator[](0);
	}

	template <typename T>
	inline typename Vector<T>::reference Vector<T>::Back() {
		return operator[](size - 1);
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::Back() const {
		return operator[](size - 1);
	}

	template <typename T>
	inline typename Vector<T>::reference Vector<T>::At(size_type index) {
		return operator[](index);
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::At(size_type index) const {
		return operator[](index);
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin() {
		return { *this, 0 };
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const {
		return { *this, 0 };
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const {
		return { *this, 0 };
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::end() {
		return { *this, size };
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const {
		return { *this, size };
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const {
		return { *this, size };
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::Find(const_reference data, EqualityFunctor equalityFunctor) {
		Iterator pos = begin();
		for (; pos != end(); ++pos) {
			if (equalityFunctor(*pos, data)) {
				break;
			}
		}
		return pos;
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const_reference data, EqualityFunctor equalityFunctor) const {
		ConstIterator pos = begin();
		for (; pos != end(); ++pos) {
			if (equalityFunctor(*pos, data)) {
				break;
			}
		}
		return pos;
	}

	template <typename T>
	inline bool Vector<T>::IsEmpty() const {
		return size == 0;
	}

	template <typename T>
	inline typename Vector<T>::size_type Vector<T>::Size() const {
		return size;
	}

	template <typename T>
	inline typename Vector<T>::size_type Vector<T>::Capacity() const {
		return capacity;
	}

	template <typename T>
	inline void Vector<T>::Reserve(size_type newCapacity) {
		if (newCapacity > capacity) {
			pointer newData = static_cast<pointer>(realloc(dataStart, newCapacity * sizeof(value_type)));
			
			assert(newData != nullptr);

			dataStart = newData;
			capacity = newCapacity;
		}
	}

	template <typename T>
	inline void Vector<T>::Resize(size_type newSize) {
		if (newSize > size) {
			Reserve(size);
			for (size_type i = size; i < newSize; ++i) {
				PushBack(value_type());
			}
		}
	}


	template <typename T>
	inline void Vector<T>::ShrinkToFit() {
		if (capacity > size) {
			if (size == 0) {
				free(dataStart);
				dataStart = nullptr;
			}
			else {
				pointer newData = static_cast<pointer>(realloc(dataStart, size * sizeof(value_type)));

				assert(newData != nullptr);

				dataStart = newData;
			}
			capacity = size;
		}
	}

	template <typename T>
	inline void Vector<T>::Clear() {
		RemoveRange(begin(), end());
	}
}