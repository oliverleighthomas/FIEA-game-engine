#pragma once
#include "pch.h"
#include "SList.h"

namespace FieaGameEngine {
	template <typename T>
	class Stack final {
		using size_type = std::size_t;
		using value_type = T;
		using reference = T&;
		using const_reference = const reference;
		using rvalue_reference = T&&;

	public:
		Stack() = default;
		Stack(const Stack& rhs) = default;
		Stack& operator=(const Stack& rhs) = default;
		Stack(Stack&& rhs) noexcept = default;
		Stack& operator=(Stack&& rhs) noexcept = default;
		~Stack() = default;

		void Push(const_reference data);
		void Push(rvalue_reference data);
		template <typename... Args>
		void EmplaceBack(Args&&... args);

		void Pop();

		reference Top();
		const_reference Top() const;

		void Size() const;
		bool IsEmpty() const;

		void Clear();

	private:
		SList<value_type> list;
	};
}

#include "Stack.inl"