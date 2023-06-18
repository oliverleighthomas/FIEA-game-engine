#pragma once
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <stdlib.h>
#include "HashMap.h"
#include <memory>
#include <string>

namespace FieaGameEngine {
	template <typename Product>
	class Factory {
	public:
		Factory() = default;
		Factory(const Factory& rhs) = delete;
		Factory& operator=(const Factory& rhs) = delete;
		Factory(Factory&& rhs) noexcept = delete;
		Factory& operator=(Factory&& rhs) noexcept = delete;
		virtual ~Factory() = default;

		/// <summary>
		/// Find the given class in the factories map.
		/// </summary>
		/// <param name="className">The name of the class to find.</param>
		/// <returns>A pointer to the factory of the given name.</returns>
		static Factory<Product>* Find(const std::string& className);
		/// <summary>
		/// Create a product of the given type.
		/// </summary>
		/// <param name="className">The name of the class of product to create.</param>
		/// <returns>A newly created product.</returns>
		static Product* Create(const std::string& className);

		/// <summary>
		/// Virtual function to be called on the product of the factory.
		/// </summary>
		/// <returns>A pointer to the newly created product.</returns>
		virtual Product* Create() = 0;
		/// <summary>
		/// Virtual function to get the name of a product.
		/// </summary>
		/// <returns>The factory's product name.</returns>
		virtual std::string ClassName() const = 0;

		/// <summary>
		/// Add the given factory to the factory table.
		/// </summary>
		/// <param name="factory">The factory to add to the table.</param>
		static void Add(std::unique_ptr<Factory<Product>> factory);
		/// <summary>
		/// Remove the given factory from the table.
		/// </summary>
		/// <param name="toRemove">The factory to remove from the table.</param>
		static void Remove(std::unique_ptr<Factory<Product>> toRemove);
		/// <summary>
		/// Clear the factories map.
		/// </summary>
		static void Clear();

	private:
		inline static HashMap<std::string, std::unique_ptr<Factory<Product>>> factories;
	};
}

#define CONCRETE_FACTORY(ConcreteProductType, BaseProductType)																		\
	class ConcreteProductType##Factory final : public FieaGameEngine::Factory<BaseProductType>										\
	{																																\
	public:																															\
		virtual BaseProductType* Create() override { return new ConcreteProductType(); }											\
		virtual std::string ClassName() const override { return std::string(#ConcreteProductType); }								\
	}																																\

#include "Factory.inl"