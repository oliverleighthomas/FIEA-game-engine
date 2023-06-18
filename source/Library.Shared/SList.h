#pragma once
#include <functional>
#include "DefaultEquality.h"

namespace FieaGameEngine {
	/// <summary>
	/// A templated singly-linked list. Utilizes "nodes" that store data of type "T."
	/// </summary>
	template <typename T>
	class SList final {
	public:
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using rvalue_reference = value_type&&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using EqualityFunctor = std::function<bool(const_reference, const_reference)>;

	private:
		/// <summary>
		/// The wrapper for data stored by the SList.
		/// </summary>
		struct Node {
			value_type data;
			Node* next{ nullptr };
			
			template <typename... Args>
			Node(Node* next, Args&&... args);
			Node(const Node&) = default;
			Node(Node&&) noexcept = default;
			Node& operator=(const Node&) = default;
			Node& operator=(Node&&) noexcept = default;
			~Node() = default;
		};

	public:
		/// <summary>
		/// An iterator for SList.
		/// </summary>
		class Iterator {
			friend SList;

		public:
			using size_type = SList::size_type;
			using difference_type = std::ptrdiff_t;
			using value_type = SList::value_type;
			using reference = SList::reference;
			using pointer = value_type*;
			using iterator_category = std::forward_iterator_tag;

			Iterator() = default;
			Iterator(const Iterator&) = default;
			Iterator(Iterator&&) noexcept = default;
			Iterator& operator=(const Iterator&) = default;
			Iterator& operator=(Iterator&&) noexcept = default;
			~Iterator() = default;

			/// <summary>
			/// Compare two iterators and return if they are equal.
			/// </summary>
			/// <param name="rhs">The iterator being compared to "this."</param>
			/// <returns>True if the iterators are equal, false if they are not.</returns>
			bool operator==(const Iterator& rhs) const;

			/// <summary>
			/// Compare two iterators and return if they are not equal.
			/// </summary>
			/// <param name="rhs">The iterator being compared to "this."</param>
			/// <returns>True if the iterators are not equal, false if they are.</returns>
			bool operator!=(const Iterator& rhs) const;

			/// <summary>
			/// Increment an iterator and then return it.
			/// </summary>
			/// <returns>An incremented iterator.</returns>
			Iterator& operator++();

			/// <summary>
			/// Incremet an iterator and return a pre-incremented copy of it.
			/// </summary>
			/// <returns>A copy of the iterator before it is incremented.</returns>
			Iterator operator++(int);

			/// <summary>
			/// Get the data of the node the iterator is pointing to.
			/// </summary>
			/// <returns>The data of the node the iterator is pointing to.</returns>
			[[nodiscard]] reference operator*() const;

		private:
			/// <summary>
			/// The constructor for the SList::Iterator class.
			/// </summary>
			/// <param name="owner">The SList the iterator is owned by.</param>
			/// <param name="node">The node the iterator points to.</param>
			Iterator(const SList& owner, Node* node);

			const SList* owner{ nullptr };
			Node* node{ nullptr };
		};

		/// <summary>
		/// A const iterator for SList.
		/// </summary>
		class ConstIterator {
			friend SList;
		public:
			using size_type = SList::size_type;
			using difference_type = std::ptrdiff_t;
			using value_type = SList::value_type;
			using reference = SList::reference;
			using const_reference = SList::const_reference;
			using pointer = value_type*;
			using iterator_category = std::forward_iterator_tag;

			ConstIterator() = default;
			ConstIterator(const ConstIterator&) = default;
			ConstIterator(ConstIterator&&) noexcept = default;
			ConstIterator& operator=(const ConstIterator& rhs) = default;
			ConstIterator& operator=(ConstIterator&&) noexcept = default;
			~ConstIterator() = default;

			/// <summary>
			/// Compare two const iterators and return if they are equal.
			/// </summary>
			/// <param name="rhs">The const iterator being compared to "this."</param>
			/// <returns>True if the const iterators are equal, false if they are not.</returns>
			bool operator==(const ConstIterator& rhs) const;

			/// <summary>
			/// Compare two const iterators and return if they are not equal.
			/// </summary>
			/// <param name="rhs">The const iterator being compared to "this."</param>
			/// <returns>True if the const iterators are not equal, false if they are.</returns>
			bool operator!=(const ConstIterator& rhs) const;

			/// <summary>
			/// Increment a const iterator and then return it.
			/// </summary>
			/// <returns>An incremented const iterator.</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Incremet a const iterator and return a pre-incremented copy of it.
			/// </summary>
			/// <returns>A copy of the const iterator before it is incremented.</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Get the data stored on the node the const iterator is pointing to.
			/// </summary>
			/// <returns>The data on the node the const iterator is pointing to.</returns>
			[[nodiscard]] const_reference operator*() const;
			
		private:
			/// <summary>
			/// The constructor for the SList::ConstIterator class.
			/// </summary>
			/// <param name="owner">The SList the const iterator is owned by.</param>
			/// <param name="node">The node the const iterator points to.</param>
			ConstIterator(const SList& owner, Node* node);

			const SList* owner{ nullptr };
			Node* node{ nullptr };
		};

		SList() = default;
		/// <summary>
		/// Construct an SList using an initializer list.
		/// </summary>
		/// <param name="list">A list of the data to be put into the nodes of the SList.</param>
		SList(std::initializer_list<value_type> list);
		/// <summary>
		/// Construct an SList by copying from another SList.
		/// </summary>
		/// <param name="rhs">The SList to copy from.</param>
		SList(const SList& rhs);
		/// <summary>
		/// Construct an SList by moving another SList into "this."
		/// </summary>
		/// <param name="other">The SList to move data from.</param>
		SList(SList&& rhs) noexcept;
		/// <summary>
		/// Take data from an intitializer list and assign it to "this."
		/// </summary>
		/// <param name="list">The list to take data from.</param>
		/// <returns>A reference to the updated "this."</returns>
		SList& operator=(std::initializer_list<value_type> list);
		/// <summary>
		/// Copy data from another SList and assign it to "this."
		/// </summary>
		/// <param name="other">The SList to copy from.</param>
		/// <returns>A reference to the updated "this."</returns>
		SList& operator=(const SList& rhs);
		/// <summary>
		/// Move data from another SList and assign it to "this."
		/// </summary>
		/// <param name="rhs">The SList to move data from.</param>
		/// <returns>A reference to the updated "this."</returns>
		SList& operator=(SList&& rhs) noexcept;
		/// <summary>
		/// The destructor for SList.
		/// </summary>
		~SList();

		/// <summary>
		/// Copies the given data and puts it at the front of the SList.
		/// </summary>
		/// <param name="data">The data to add to the front.</param>
		/// <returns>An iterator to the newly added data.</returns>
		Iterator PushFront(const_reference data);
		/// <summary>
		/// Moves the given data to the front of the SList.
		/// </summary>
		/// <param name="data">The data to add to the front.</param>
		/// <returns>An iterator to the newly added data.</returns>
		Iterator PushFront(rvalue_reference data);
		/// <summary>
		/// Constructs the given data at the front of the SList.
		/// </summary>
		/// <param name="...args">The data to place at the front of the SList.</param>
		/// <returns>An iterator to the newly added data.</returns>
		template <typename... Args>
		Iterator EmplaceFront(Args&&... args);

		/// <summary>
		/// Copies the given data and puts it at the back of the SList.
		/// </summary>
		/// <param name="data">The data to add to the back.</param>
		/// <returns>An iterator to the newly added data.</returns>
		Iterator PushBack(const_reference data);
		/// <summary>
		/// Moves the given data to the back of the SList.
		/// </summary>
		/// <param name="data">The data to add to the back.</param>
		/// <returns>An iterator to the newly added data.</returns>
		Iterator PushBack(rvalue_reference data);
		/// <summary>
		/// Constructs the given data at the back of the SList.
		/// </summary>
		/// <param name="...args">The data to place at the front of the SList.</param>
		/// <returns>An iterator to the newly added data.</returns>
		template <typename... Args>
		Iterator EmplaceBack(Args&&... args);

		/// <summary>
		/// Copies the given data and inserts it after the location of the given iterator.
		/// </summary>
		/// <param name="data">The data to copy into the SList.</param>
		/// <param name="position">The location after which to copy the given data to.</param>
		/// <returns>An iterator pointing to the new data in the SList.</returns>
		Iterator InsertAfter(const_reference data, const Iterator& position);
		/// <summary>
		/// Moves the given data to after the location of the given iterator.
		/// </summary>
		/// <param name="data">The data to move into the SList.</param>
		/// <param name="position">The location after which to move the given data to.</param>
		/// <returns>An iterator pointing to the new data in the SList.</returns>
		Iterator InsertAfter(rvalue_reference data, const Iterator& position);
		/// <summary>
		/// Constructs the given data after the location of the given iterator.
		/// </summary>
		/// <param name="position">The location after which to construct the given data.</param>
		/// <param name="...args">The data to construct into the SList.</param>
		/// <returns>An iterator pointing to the newly constructed data.</returns>
		template <typename... Args>
		Iterator EmplaceAfter(const Iterator& position, Args&&... args);

		/// <summary>
		/// Removes the first value in the SList.
		/// </summary>
		void PopFront();
		/// <summary>
		/// Removes the last value in the SList.
		/// </summary>
		void PopBack();
	
		/// <summary>
		/// Removes the given value from the SList.
		/// </summary>
		/// <param name="value">The value to remove from the SList.</param>
		/// <param name="equalityFunctor">The functor to use when comparing the SList data with the given data.</param>
		/// <returns>A bool indicating if the removal was successful or not.</returns>
		bool Remove(const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{});
		/// <summary>
		/// Removes the value at the given location in the SList.
		/// </summary>
		/// <param name="position">The location at which to remove data.</param>
		/// <returns>A bool indicating if the removal was successful or not.</returns>
		bool RemoveAt(const Iterator& position);

		/// <summary>
		/// Find the given data within the SList.
		/// </summary>
		/// <param name="value">The value to find within the SList.</param>
		/// <param name="equalityFunctor">The functor to use when comparing the SList data with the given data.</param>
		/// <returns>An iterator pointing to the location of the given data. Returns an end iterator if the data was not found.</returns>
		[[nodiscard]] Iterator Find(const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{});
		/// <summary>
		/// Find the given data within the SList.
		/// </summary>
		/// <param name="value">The value to find within the SList.</param>
		/// <param name="equalityFunctor">The functor to use when comparing the SList data with the given data.</param>
		/// <returns>A const iterator pointing to the location of the given data. Returns an end iterator if the data was not found.</returns>
		[[nodiscard]] ConstIterator Find(const_reference value, EqualityFunctor equalityFunctor = DefaultEquality<T>{}) const;

		/// <summary>
		/// Get the size of the SList.
		/// </summary>
		/// <returns>The size of the SList.</returns>
		[[nodiscard]] size_type Size() const;
		/// <summary>
		/// Checks if the given SList is empty.
		/// </summary>
		/// <returns>True if the SList is empty, false if it is not.</returns>
		[[nodiscard]] bool IsEmpty() const;

		/// <summary>
		/// Get the data at the front of the SList.
		/// </summary>
		/// <returns>A reference to the data at the front of the SList.</returns>
		[[nodiscard]] reference Front();
		/// <summary>
		/// Get the data at the front of the SList.
		/// </summary>
		/// <returns>A const reference to the data at the front of the SList.</returns>
		[[nodiscard]] const_reference Front() const;

		/// <summary>
		/// Get the data at the back of the SList.
		/// </summary>
		/// <returns>A reference to the data at the back of the SList.</returns>
		[[nodiscard]] reference Back();
		/// <summary>
		/// Get the data at the back of the SList.
		/// </summary>
		/// <returns>A const reference to the data at the back of the SList.</returns>
		[[nodiscard]] const_reference Back() const;

		/// <summary>
		/// Get the iterator pointing to the front of the SList.
		/// </summary>
		/// <returns>An iterator pointing to the front of the SList.</returns>
		[[nodiscard]] Iterator begin();
		/// <summary>
		/// Get the iterator pointing to the back of the SList.
		/// </summary>
		/// <returns>An iterator pointing to the back of the SList.</returns>
		[[nodiscard]] Iterator end();

		/// <summary>
		/// Get the const iterator pointing to the front of the SList.
		/// </summary>
		/// <returns>A const iterator pointing to the front of the SList.</returns>
		[[nodiscard]] ConstIterator begin() const;
		/// <summary>
		/// Get the const iterator pointing to the back of the SList.
		/// </summary>
		/// <returns>An const iterator pointing to the back of the SList.</returns>
		[[nodiscard]] ConstIterator end() const;

		/// <summary>
		/// Get the const iterator pointing to the front of the SList.
		/// </summary>
		/// <returns>A const iterator pointing to the front of the SList.</returns>
		[[nodiscard]] ConstIterator cbegin() const;
		/// <summary>
		/// Get the const iterator pointing to the back of the SList.
		/// </summary>
		/// <returns>An const iterator pointing to the back of the SList.</returns>
		[[nodiscard]] ConstIterator cend() const;

		/// <summary>
		/// Clears the given SList.
		/// </summary>
		void Clear();

	private:
		Node* front{ nullptr };
		Node* back{ nullptr };
		size_type size{ 0 };
	};
}

#include "SList.inl"