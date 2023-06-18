#pragma once
// Headers used in this file //
#include <assert.h>
#include <functional>
#include <stdexcept>
#include "DefaultIncrement.h"
#include "DefaultEquality.h"

namespace FieaGameEngine {
	template <typename T>
	class Vector final {
	public:
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using rvalue_reference = T&&;
		using pointer = value_type*;
		using EqualityFunctor = std::function<bool(const_reference, const_reference)>;
		using IncrementFunctor = std::function<size_type(size_type, size_type)>;

	public:
		class Iterator final {
			friend Vector;
			
		public:
			using size_type = Vector::size_type;
			using difference_type = std::ptrdiff_t;
			using value_type = Vector::value_type;
			using reference = Vector::reference;
			using pointer = Vector::pointer;
			using iterator_category = std::random_access_iterator_tag;

			Iterator() = default;
			Iterator(const Iterator& other) = default;
			Iterator(Iterator&&) noexcept = default;
			Iterator& operator=(const Iterator& rhs) = default;
			Iterator& operator=(Iterator&&) noexcept = default;
			~Iterator() = default;

			[[nodiscard]] bool operator==(const Iterator& rhs) const;
			[[nodiscard]] bool operator!=(const Iterator& rhs) const;
			[[nodiscard]] bool operator>(const Iterator& rhs) const;
			[[nodiscard]] bool operator>=(const Iterator& rhs) const;
			[[nodiscard]] bool operator<(const Iterator& rhs) const;
			[[nodiscard]] bool operator<=(const Iterator& rhs) const;

			Iterator& operator++();
			Iterator operator++(int);
			Iterator& operator--();
			Iterator operator--(int);

			Iterator& operator+=(difference_type increment);
			difference_type operator+(const Iterator& rhs) const;
			Iterator& operator-=(difference_type decrement);
			difference_type operator-(const Iterator& rhs) const;

			[[nodiscard]] reference operator[](difference_type index);
			[[nodiscard]] reference operator*() const;

		private:
			Iterator(const Vector& owner, size_type index);

			const Vector* owner{ nullptr };
			size_type index{ 0 };
		};

		class ConstIterator final {
			friend Vector;

		public:
			using size_type = Vector::size_type;
			using difference_type = std::ptrdiff_t;
			using value_type = Vector::value_type;
			using reference = Vector::reference;
			using const_reference = Vector::const_reference;
			using pointer = Vector::pointer;
			using iterator_category = std::random_access_iterator_tag;

			ConstIterator() = default;
			ConstIterator(const ConstIterator& other) = default;
			ConstIterator(ConstIterator&&) noexcept = default;
			ConstIterator& operator=(const ConstIterator&) = default;
			ConstIterator& operator=(ConstIterator&&) noexcept = default;
			~ConstIterator() = default;

			[[nodiscard]] bool operator==(const ConstIterator& rhs) const;
			[[nodiscard]] bool operator!=(const ConstIterator& rhs) const;
			[[nodiscard]] bool operator>(const ConstIterator& rhs) const;
			[[nodiscard]] bool operator>=(const ConstIterator& rhs) const;
			[[nodiscard]] bool operator<(const ConstIterator& rhs) const;
			[[nodiscard]] bool operator<=(const ConstIterator& rhs) const;

			ConstIterator& operator++();
			ConstIterator operator++(int);
			ConstIterator& operator--();
			ConstIterator operator--(int);

			ConstIterator& operator+=(difference_type increment);
			difference_type operator+(const ConstIterator& rhs) const;
			ConstIterator& operator-=(difference_type decrement);
			difference_type operator-(const ConstIterator& rhs) const;

			[[nodiscard]] const_reference operator[](difference_type index);
			[[nodiscard]] const_reference operator*() const;

		private:
			ConstIterator(const Vector& owner, size_type index);

			const Vector* owner{ nullptr };
			size_type index{ 0 };
		};


		explicit Vector(size_type initialCapacity = 0, IncrementFunctor incrementFunctor = DefaultIncrement<size_type>{});
		Vector(std::initializer_list<value_type> list);
		Vector(const Vector& rhs);
		Vector(Vector&& rhs) noexcept;
		Vector& operator=(const Vector& rhs);
		Vector& operator=(Vector&& rhs) noexcept;
		Vector& operator=(std::initializer_list<value_type> list);
		~Vector();

		[[nodiscard]] reference operator[](size_type index);
		[[nodiscard]] const_reference operator[](size_type index) const;
		[[nodiscard]] bool operator==(const Vector& rhs) const;
		[[nodiscard]] bool operator!=(const Vector& rhs) const;

		void PushBack(const_reference data);
		void PushBack(rvalue_reference data);
		template <typename... Args>
		void EmplaceBack(Args&&... args);

		void PopBack();

		bool Remove(const_reference data, EqualityFunctor equalityFunctor = DefaultEquality<value_type>{});
		bool RemoveAt(const Iterator& position);
		bool RemoveAt(size_type index);
		bool RemoveRange(Iterator begin, Iterator end);

		[[nodiscard]] reference Front();
		[[nodiscard]] const_reference Front() const;

		[[nodiscard]] reference Back();
		[[nodiscard]] const_reference Back() const;

		[[nodiscard]] reference At(size_type index);
		[[nodiscard]] const_reference At(size_type index) const;

		[[nodiscard]] Iterator begin();
		[[nodiscard]] ConstIterator begin() const;
		[[nodiscard]] ConstIterator cbegin() const;

		[[nodiscard]] Iterator end();
		[[nodiscard]] ConstIterator end() const;
		[[nodiscard]] ConstIterator cend() const;

		[[nodiscard]] Iterator Find(const_reference data, EqualityFunctor equalityFunctor = DefaultEquality<value_type>{});
		[[nodiscard]] ConstIterator Find(const_reference data, EqualityFunctor equalityFunctor = DefaultEquality<value_type>{}) const;

		[[nodiscard]] bool IsEmpty() const;

		[[nodiscard]] size_type Size() const;
		[[nodiscard]] size_type Capacity() const;

		void Reserve(size_type capacity);

		void Resize(size_type size);
		void ShrinkToFit();

		void Clear();

	private:
		value_type* dataStart{ nullptr };
		size_type capacity{ 0 };
		size_type size{ 0 };
		IncrementFunctor incrementFunctor{ DefaultIncrement<size_type>{} };
	};
}

#include "vector.inl"