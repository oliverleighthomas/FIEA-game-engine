#pragma once
#include "Factory.h"

namespace FieaGameEngine {
	template <typename Product>
	Factory<Product>* Factory<Product>::Find(const std::string& className) {
		auto found = factories.Find(className);

		if (found == factories.end()) return nullptr;
		return found->second.get();
	}

	template <typename Product>
	Product* Factory<Product>::Create(const std::string& className) {
		auto factory = Find(className);

		if (factory == nullptr) return nullptr;
		return factory->Create();
	}

	template <typename Product>
	void Factory<Product>::Add(std::unique_ptr<Factory<Product>> toAdd) {
		auto result = factories.Insert(std::make_pair<std::string, std::unique_ptr<Factory<Product>>>(toAdd->ClassName(), std::move(toAdd)));
		assert(result.second); UNREFERENCED_LOCAL(result);
	}

	template <typename Product>
	void Factory<Product>::Remove(std::unique_ptr<Factory<Product>> toRemove) {
		factories.Remove(toRemove->ClassName());
	}

	template <typename Product>
	void Factory<Product>::Clear() {
		factories.Clear();
	}
}