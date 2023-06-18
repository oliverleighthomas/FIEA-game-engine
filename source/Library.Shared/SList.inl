#include "SList.h"

namespace FieaGameEngine {
	template<typename T>
	template<typename... Args>
	inline SList<T>::Node::Node(Node* next, Args&&... args) :
		next(next), data(std::forward<Args>(args)...)
	{
	}

	template<typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		owner{ &owner }, node{node}
	{
	}

	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const SList& owner, Node* node) :
		owner{ &owner }, node{ node }
	{
	}

	template<typename T>
	SList<T>::SList(std::initializer_list<value_type> list) {
		for (const auto& value : list) {
			EmplaceBack(value);
		}
	}

	template<typename T>
	SList<T>::SList(const SList& rhs) {
		Node* currentNode = rhs.front;
		while (currentNode != nullptr) {
			EmplaceBack(currentNode->data);
			currentNode = currentNode->next;
		}
	}
	
	template<typename T>
	SList<T>::SList(SList&& rhs) noexcept :
		front{ rhs.front }, back{ rhs.back }, size{ rhs.size }
	{
		rhs.front = nullptr;
		rhs.back = nullptr;
		rhs.size = 0;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(std::initializer_list<value_type> list) {
		Clear();
		for (const auto& value : list) {
			EmplaceBack(std::move(value));
		}

		return *this;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& rhs) {
		if (this != &rhs) {
			Clear();
			Node* currentNode = rhs.front;
			while (currentNode != nullptr) {
				EmplaceBack(currentNode->data);
				currentNode = currentNode->next;
			}
		}

		return *this;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(SList&& rhs) noexcept {
		if (this != &rhs) {
			Clear();
			front = rhs.front;
			back = rhs.back;
			size = rhs.size;

			rhs.front = nullptr;
			rhs.back = nullptr;
			rhs.size = 0;
		}

		return *this;
	}

	template <typename T>
	SList<T>::~SList() {
		Clear();
	}

	template <typename T>
	inline bool SList<T>::Iterator::operator==(const SList<T>::Iterator& rhs) const {
		return (owner == rhs.owner && node == rhs.node);
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator==(const SList<T>::ConstIterator& rhs) const {
		return (owner == rhs.owner && node == rhs.node);
	}

	template <typename T>
	inline bool SList<T>::Iterator::operator!=(const SList<T>::Iterator& rhs) const {
		return !(operator==(rhs));
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator!=(const SList<T>::ConstIterator& rhs) const {
		return !(operator==(rhs));
	}

	template <typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++() {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator is not associated with a container.");
#endif

		if (node != nullptr) node = node->next;
		return *this;
	}

	template <typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++() {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator is not associated with a container.");
#endif

		if (node != nullptr) node = node->next;
		return *this;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int) {
		Iterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int) {
		ConstIterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	inline typename SList<T>::reference SList<T>::Iterator::operator*() const {
#ifdef USE_EXCEPTIONS
		if (node == nullptr) throw std::runtime_error("Cannot dereference null pointer.");
#endif
		return (*node).data;
	}

	template <typename T>
	inline typename SList<T>::const_reference SList<T>::ConstIterator::operator*() const {
#ifdef USE_EXCEPTIONS
		if (node == nullptr) throw std::runtime_error("Cannot dereference null pointer.");
#endif
		return (*node).data;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(const_reference data) {
		return EmplaceFront(data);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushFront(rvalue_reference data) {
		return EmplaceFront(std::move(data));
	}

	template<typename T>
	template <typename... Args>
	inline typename SList<T>::Iterator SList<T>::EmplaceFront(Args&&... args) {
		front = new Node(front, std::forward<Args>(args)...);
		if (IsEmpty()) back = front;
		++size;

		return begin();
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushBack(const_reference data) {
		return EmplaceBack(data);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::PushBack(rvalue_reference data) {
		return EmplaceBack(std::move(data));
	}

	template<typename T>
	template <typename... Args>
	inline typename SList<T>::Iterator SList<T>::EmplaceBack(Args&&... args) {
		Node* toAdd = new Node(nullptr, std::forward<Args>(args)...);

		if (IsEmpty()) front = toAdd;
		else back->next = toAdd;

		back = toAdd;
		++size;

		return { *this, back };
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const_reference data, const Iterator& iter) {
		return EmplaceAfter(iter, data);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(rvalue_reference data, const Iterator& iter) {
		return EmplaceAfter(iter, std::move(data));
	}

	template<typename T>
	template <typename... Args>
	inline typename SList<T>::Iterator SList<T>::EmplaceAfter(const Iterator& position, Args&&... args) {
#ifdef USE_EXCEPTIONS
		if (this != position.owner) throw std::runtime_error("Cannot use an iterator from a different list.");
#endif

		if (position == end()) return EmplaceBack(std::forward<Args>(args)...);

		Node* currNode = position.node;
		Node* nextNode = currNode->next;
		Node* toAdd = new Node{ nextNode, std::forward<Args>(args)... };

		currNode->next = toAdd;
		if (toAdd->next == nullptr) back = toAdd;

		++size;

		return { *this, toAdd };
	}

	template <typename T>
	inline void SList<T>::PopFront() {
#ifdef USE_EXCEPTIONS
		if (IsEmpty()) throw std::runtime_error("List is empty.");
#endif
		Node* newFront = front->next;
		delete(front);
		front = newFront;
		size--;
	}

	template <typename T>
	inline void SList<T>::PopBack() {
		if (Size() <= 1) PopFront();
		else {
			Node* newBack = front;
			while (newBack->next != back) {
				newBack = newBack->next;
			}
			delete(back);
			back = newBack;
			newBack->next = nullptr;
			size--;
		}
	}

	template <typename T>
	bool SList<T>::Remove(const_reference value, EqualityFunctor equalityFunctor) {
		return RemoveAt(Find(value, equalityFunctor));
	}

	template <typename T>
	bool SList<T>::RemoveAt(const Iterator& position) {
#ifdef USE_EXCEPTIONS
		if (position.owner != this) throw std::runtime_error("Cannot use an iterator from a different list.");
#endif

		if (position == end()) return false;

		if (position.node == back) PopBack();
		else {
			Node* next = position.node->next;

			position.node->data.~T();
			new(&position.node->data)T(std::move(next->data));
			position.node->next = next->next;
			delete next;

			if (position.node->next == nullptr) back = position.node;

			--size;
		}
		return true;
	}

	template <typename T>
	inline typename SList<T>::size_type SList<T>::Size() const {
		return size;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const {
		return size == 0;
	}

	template <typename T>
	inline typename SList<T>::reference SList<T>::Front() {
#ifdef USE_EXCEPTIONS
		if (IsEmpty()) throw std::runtime_error("List is empty.");
#endif
		return front->data;
	}

	template <typename T>
	inline typename SList<T>::const_reference SList<T>::Front() const {
#ifdef USE_EXCEPTIONS
		if (IsEmpty()) throw std::runtime_error("List is empty.");
#endif
		return front->data;
	}

	template <typename T>
	inline typename SList<T>::reference SList<T>::Back() {
#ifdef USE_EXCEPTIONS
		if (IsEmpty()) throw std::runtime_error("List is empty.");
#endif
		return back->data;
	}

	template <typename T>
	inline typename SList<T>::const_reference SList<T>::Back() const {
#ifdef USE_EXCEPTIONS
		if (IsEmpty()) throw std::runtime_error("List is empty.");
#endif
		return back->data;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::begin() {
		return { *this, front };
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::end() {
		return { *this, nullptr };
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const {
		return { *this, front };
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const {
		return { *this, nullptr };
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const {
		return begin();
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const {
		return end();
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::Find(const_reference value, EqualityFunctor equalityFunctor) {
		Iterator position = begin();
		for (; position != end(); ++position) {
			if (equalityFunctor(*position, value)) break;
		}
		return position;
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::Find(const_reference value, EqualityFunctor equalityFunctor) const {
		ConstIterator position = begin();
		for (; position != end(); ++position) {
			if (equalityFunctor(*position, value)) break;
		}
		return position;
	}

	template <typename T>
	inline void SList<T>::Clear() {
		if (!IsEmpty()) {
			Node* currentNode = front;
			while (currentNode != nullptr) {
				Node* nextNode = currentNode->next;
				delete(currentNode);
				size--;
				currentNode = nextNode;
			}
		}
	}
}