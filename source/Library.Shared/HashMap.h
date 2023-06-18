#pragma once
// Headers used in this file //
#include <assert.h>
#include <functional>
#include "SList.h"
#include "vector.h"
#include "DefaultEquality.h"
#include "DefaultHash.h"

namespace FieaGameEngine {
	template <typename TKey, typename TData>
	class HashMap final {
	public:
		using pair_type = std::pair<const TKey, TData>;
		using chain_type = SList<pair_type>;

		using chain_iterator = typename chain_type::Iterator;
		using const_chain_iterator = typename chain_type::ConstIterator;

		using size_type = std::size_t;
		using reference = pair_type&;
		using const_reference = const pair_type&;
		using pointer = pair_type*;
		using const_pointer = const pair_type*;
		using EqualityFunctor = std::function<bool(const TKey&, const TKey&)>;
		using HashFunctor = std::function<size_type(const TKey&)>;

		static const size_type DefaultSize = 13;

	public:
		class Iterator final {
			friend HashMap;

		public:
			using size_type = HashMap::size_type;
			using difference_type = std::ptrdiff_t;
			using value_type = HashMap::pair_type;
			using reference = HashMap::reference;
			using pointer = HashMap::pointer;
			using iterator_category = std::forward_iterator_tag;

			Iterator() = default;
			Iterator(const Iterator& other) = default;
			Iterator(Iterator&&) noexcept = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator& operator=(Iterator&&) noexcept = default;
			~Iterator() = default;

			Iterator& operator++();
			Iterator operator++(int);

			[[nodiscard]] reference operator*() const;
			[[nodiscard]] pointer operator->() const;

			[[nodiscard]] bool operator==(const Iterator& rhs) const;
			[[nodiscard]] bool operator!=(const Iterator& rhs) const;

		private:
			Iterator(HashMap& owner, const size_type& mapIndex, const chain_iterator& iter);

			HashMap* owner{ nullptr };
			size_type mapIndex{ 0 };
			chain_iterator iter;
		};

		class ConstIterator final {
			friend HashMap;

		public:
			using size_type = HashMap::size_type;
			using difference_type = std::ptrdiff_t;
			using value_type = HashMap::pair_type;
			using reference = HashMap::reference;
			using const_reference = HashMap::const_reference;
			using pointer = HashMap::pointer;
			using const_pointer = HashMap::const_pointer;
			using iterator_category = std::forward_iterator_tag;

			ConstIterator() = default;
			ConstIterator(const ConstIterator& rhs) = default;
			ConstIterator(ConstIterator&&) noexcept = default;
			ConstIterator& operator=(const ConstIterator& rhs) = default;
			ConstIterator& operator=(ConstIterator&&) noexcept = default;
			~ConstIterator() = default;

			ConstIterator& operator++();
			ConstIterator operator++(int);

			[[nodiscard]] const_reference operator*() const;
			[[nodiscard]] const_pointer operator->() const;

			[[nodiscard]] bool operator==(const ConstIterator& rhs) const;
			[[nodiscard]] bool operator!=(const ConstIterator& rhs) const;

		private:
			ConstIterator(const HashMap& owner, const size_type& mapIndex, const const_chain_iterator& iter);

			const HashMap* owner{ nullptr };
			size_type mapIndex{ 0 };
			const_chain_iterator iter;
		};

		explicit HashMap(size_type size = DefaultSize, HashFunctor hashFunctor = DefaultHash<TKey>{});
		HashMap(std::initializer_list<pair_type> list);
		HashMap(const HashMap& rhs) = default;
		HashMap(HashMap&& rhs) noexcept = default;
		HashMap& operator=(const HashMap& other) = default;
		HashMap& operator=(HashMap&& rhs) noexcept = default;
		HashMap& operator=(std::initializer_list<pair_type> list);
		~HashMap() = default;

		[[nodiscard]] TData& operator[](const TKey& key);
		[[nodiscard]] const TData& operator[](const TKey& key) const;

		std::pair<Iterator, bool> Insert(const TKey& key, const TData& data, EqualityFunctor equalityFunctor = DefaultEquality<TKey>{});
		std::pair<Iterator, bool> Insert(TKey&& key, TData&& data, EqualityFunctor equalityFunctor = DefaultEquality<TKey>{});
		std::pair<Iterator, bool> Insert(const pair_type& data, EqualityFunctor equalityFunctor = DefaultEquality<TKey>{});
		std::pair<Iterator, bool> Insert(pair_type&& data, EqualityFunctor equalityFunctor = DefaultEquality<TKey>{});

		bool Remove(const TKey& key, EqualityFunctor equalityFunctor = DefaultEquality<TKey>{});
		bool RemoveAt(const Iterator& position);

		[[nodiscard]] Iterator Find(const TKey& key, EqualityFunctor equalityFunctor = DefaultEquality<TKey>{});
		[[nodiscard]] ConstIterator Find(const TKey& key, EqualityFunctor equalityFunctor = DefaultEquality<TKey>{}) const;

		[[nodiscard]] bool ContainsKey(const TKey& key) const;

		[[nodiscard]] TData& At(const TKey& key);
		[[nodiscard]] const TData& At(const TKey& key) const;

		[[nodiscard]] size_type Size() const;

		[[nodiscard]] Iterator begin();
		[[nodiscard]] ConstIterator begin() const;
		[[nodiscard]] ConstIterator cbegin() const;

		[[nodiscard]] Iterator end();
		[[nodiscard]] ConstIterator end() const;
		[[nodiscard]] ConstIterator cend() const;

		void Clear();

	private:
		Vector<chain_type> chains;
		size_type size { 0 };
		HashFunctor hashFunctor{ DefaultHash<TKey>{} };
	};
}

#include "HashMap.inl"