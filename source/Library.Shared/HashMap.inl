#pragma once
#include "HashMap.h"

namespace FieaGameEngine {
	template <typename TKey, typename TData>
	HashMap<TKey, TData>::Iterator::Iterator(HashMap& owner, const size_type& mapIndex, const chain_iterator& iter) :
		owner{ &owner }, mapIndex{ mapIndex }, iter{ iter }
	{
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& owner, const size_type& mapIndex, const const_chain_iterator& iter) :
		owner{ &owner }, mapIndex{ mapIndex }, iter{ iter }
	{
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(size_type size, HashFunctor hashFunctor) :
		hashFunctor{ hashFunctor }
	{
		assert(size > 1);
		chains.Resize(size);
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(std::initializer_list<pair_type> list) {
		assert(list.size() > 1);

		chains.Resize(list.size());
		for (const auto& value : list) {
			Insert(value);
		}
	}

	template<typename TKey, typename TData>
	HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(std::initializer_list<pair_type> list)
	{
		Clear();
		assert(list.size() > 1);

		chains.Resize(list.size());
		for (const auto& value : list) {
			Insert(value);
		}

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++() {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		if (*this != owner->end()) {
			++iter;

			if (iter == owner->chains[mapIndex].end()) {
				do {
					++mapIndex;
				} while (mapIndex < owner->chains.Size() && owner->chains[mapIndex].IsEmpty());

				if (mapIndex == owner->chains.Size()) *this = owner->end();
				else iter = owner->chains[mapIndex].begin();
			}
		}

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator++() {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
#endif

		if (*this != owner->end()) {
			++iter;

			if (iter == owner->chains[mapIndex].end()) {
				do {
					++mapIndex;
				} while (mapIndex < owner->chains.Size() && owner->chains[mapIndex].IsEmpty());

				if (mapIndex == owner->chains.Size()) *this = owner->end();
				else iter = owner->chains[mapIndex].begin();
			}
		}

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Iterator::operator++(int) {
		Iterator it = *this;
		operator++();
		return it;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::ConstIterator::operator++(int) {
		ConstIterator it = *this;
		operator++();
		return it;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::reference HashMap<TKey, TData>::Iterator::operator*() const {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
		if (mapIndex >= owner->chains.Size()) throw std::runtime_error("Invalid index.");
#endif
		return *iter;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::const_reference HashMap<TKey, TData>::ConstIterator::operator*() const {
#ifdef USE_EXCEPTIONS
		if (owner == nullptr) throw std::runtime_error("Iterator not associated with a container.");
		if (mapIndex >= owner->chains.Size()) throw std::runtime_error("Invalid index.");
#endif
		return *iter;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::pointer HashMap<TKey, TData>::Iterator::operator->() const {
		return &(operator*());
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::const_pointer HashMap<TKey, TData>::ConstIterator::operator->() const {
		return &(operator*());
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Iterator::operator==(const Iterator& rhs) const {
		return (owner == rhs.owner && mapIndex == rhs.mapIndex && iter == rhs.iter);
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::ConstIterator::operator==(const ConstIterator& rhs) const {
		return (owner == rhs.owner && mapIndex == rhs.mapIndex && iter == rhs.iter);
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Iterator::operator!=(const Iterator& rhs) const {
		return !(operator==(rhs));
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& rhs) const {
		return !(operator==(rhs));
	}

	template <typename TKey, typename TData>
	TData& HashMap<TKey, TData>::operator[](const TKey& key) {
		return Insert(std::make_pair(key, TData())).first->second;
	}

	template <typename TKey, typename TData>
	const TData& HashMap<TKey, TData>::operator[](const TKey& key) const {
		return At(key);
	}
	
	template <typename TKey, typename TData>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const TKey& key, const TData& data, EqualityFunctor equalityFunctor) {
		return Insert(std::make_pair(key, data), equalityFunctor);
	}

	template <typename TKey, typename TData>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(TKey&& key, TData&& data, EqualityFunctor equalityFunctor) {
		return Insert(std::make_pair(std::move(key), std::move(data)), equalityFunctor);
	}

	template <typename TKey, typename TData>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const pair_type& data, EqualityFunctor equalityFunctor) {
		Iterator iter = Find(data.first, equalityFunctor);

		if(iter != end()) return std::make_pair(iter, false);

		size_type index = hashFunctor(data.first) % chains.Size();
		++size;
		iter = { *this, index, chains[index].PushBack(data) };

		return std::make_pair(iter, true);
	}

	template <typename TKey, typename TData>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(pair_type&& data, EqualityFunctor equalityFunctor) {
		Iterator iter = Find(data.first, equalityFunctor);

		if (iter != end()) return std::make_pair(iter, false);

		size_type index = hashFunctor(data.first) % chains.Size();
		++size;
		iter = { *this, index, chains[index].PushBack(std::move(data)) };

		return std::make_pair(iter, true);
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Remove(const TKey& key, EqualityFunctor equalityFunctor) {
		return RemoveAt(Find(key, equalityFunctor));
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::RemoveAt(const Iterator& position) {
#ifdef USE_EXCEPTIONS
		if (position.owner != this) throw std::runtime_error("Cannot remove using an interator from a different container.");
#endif
		if (position == end()) return false;

		chains[position.mapIndex].RemoveAt(position.iter);
		--size;

		return true;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key, EqualityFunctor equalityFunctor) {
		if (size == 0) return end();

		size_type index = hashFunctor(key) % chains.Size();

		auto iter = chains[index].begin();
		for (; iter != chains[index].end(); ++iter) {
			if (equalityFunctor((*iter).first, key)) {
				break;
			}
		}

		return (iter == chains[index].end()) ? end() : Iterator{ *this, index, iter };
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key, EqualityFunctor equalityFunctor) const {
		if (size == 0) return end();

		size_type index = hashFunctor(key) % chains.Size();

		auto iter = chains[index].begin();
		for (; iter != chains[index].end(); ++iter) {
			if (equalityFunctor((*iter).first, key)) {
				break;
			}
		}

		return (iter == chains[index].end()) ? end() : ConstIterator{ *this, index, iter };
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::ContainsKey(const TKey& key) const {
		return (Find(key) != end());
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::size_type HashMap<TKey, TData>::Size() const {
		return size;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin() {
		for (size_t i = 0; i < chains.Size(); ++i) {
			if (!chains[i].IsEmpty()) {
				return { *this, i, chains[i].begin() };
			}
		}
		return end();
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::begin() const {
		for (size_t i = 0; i < chains.Size(); ++i) {
			if (!chains[i].IsEmpty()) {
				return { *this, i, chains[i].begin() };
			}
		}
		return end();
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const {
		for (size_t i = 0; i < chains.Size(); ++i) {
			if (!chains[i].IsEmpty()) {
				return { *this, i, chains[i].begin() };
			}
		}
		return end();
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end() {
		return { *this, chains.Size(), chain_iterator()};
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::end() const {
		return { *this, chains.Size(), const_chain_iterator() };
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const {
		return { *this, chains.Size(), const_chain_iterator() };
	}

	template <typename TKey, typename TData>
	TData& HashMap<TKey, TData>::At(const TKey& key) {
		auto iter = Find(key);
#ifdef USE_EXCEPTIONS
		if (iter == end()) throw std::runtime_error("Key does not exist in map.");
#endif
		return iter->second;
	}

	template <typename TKey, typename TData>
	const TData& HashMap<TKey, TData>::At(const TKey& key) const {
		auto iter = Find(key);
#ifdef USE_EXCEPTIONS
		if (iter == end()) throw std::runtime_error("Key does not exist in map.");
#endif
		return iter->second;
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Clear() {
		for (auto& chain : chains) {
			chain.Clear();
		}
		size = 0;
	}
}