#include "pch.h"
#include "Scope.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(Scope);

	Scope::Scope(size_type initialCapacity) :
		table(std::max(initialCapacity, table_type::DefaultSize)), orderVector(initialCapacity)
	{
	}

	Scope::Scope(const Scope& rhs) :
		orderVector{ rhs.orderVector.Size() }, table{ std::max(rhs.table.Size(), table_type::DefaultSize) }
	{ 
		for (auto pair : rhs.orderVector) { 
			auto& [name, existingDatum] = *pair; 
			auto& newDatum = Append(name); 
			if (existingDatum.Type() == Datum::DatumTypes::Table) { 
				newDatum.SetType(Datum::DatumTypes::Table);
				newDatum.Reserve(existingDatum.Size()); 
				for (size_type i = 0; i < existingDatum.Size(); ++i) { 
					Scope* scope = existingDatum[i].Clone();
					scope->parent = this;
					newDatum.PushBack(scope); 
				} 
			} 
			else { 
				newDatum = existingDatum; 
			}
		} 
	}

	Scope& Scope::operator=(const Scope& rhs) {
		if (this != &rhs) {
			Clear();

			orderVector.ShrinkToFit();
			orderVector.Reserve(rhs.orderVector.Size());

			table = table_type(std::max(rhs.table.Size(), table_type::DefaultSize));

			for (auto pair : rhs.orderVector) {
				auto& [name, existingDatum] = *pair;
				auto& newDatum = Append(name);
				if (existingDatum.Type() == Datum::DatumTypes::Table) {
					newDatum.SetType(Datum::DatumTypes::Table);
					newDatum.Reserve(existingDatum.Size());
					for (size_type i = 0; i < existingDatum.Size(); ++i) {
						Scope* scope = existingDatum[i].Clone();
						scope->parent = this;
						newDatum.PushBack(scope);
					}
				}
				else {
					newDatum = existingDatum;
				}
			}
		}

		return *this;
	}

	Scope::Scope(Scope&& rhs) noexcept : 
		table(std::move(rhs.table)), orderVector(std::move(rhs.orderVector)), parent(rhs.parent)
	{
		Reparent();

		rhs.table.Clear();
		rhs.orderVector.Clear();
		rhs.orderVector.ShrinkToFit();
		rhs.parent = nullptr;
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept 
	{
		if (this != &rhs) {
			Clear();
			table = std::move(rhs.table);
			orderVector = std::move(rhs.orderVector);
			parent = rhs.parent;

			Reparent();

			rhs.table.Clear();
			rhs.orderVector.Clear();
			rhs.orderVector.ShrinkToFit();
			rhs.parent = nullptr;
		}
		return *this;
	}

	Scope::~Scope() {
		Clear();
	}

	bool Scope::operator==(const Scope& rhs) const {
		if (this == &rhs) return true;
		if (Size() != rhs.Size()) return false;

		for (const auto& entry : orderVector) {
			auto found = rhs.Find(entry->first);
			if (found == nullptr || entry->second != *found) return false;
		}

		return true;
	}

	const Datum& Scope::operator[](const std::string& name) const {
		const Datum* found = Find(name);
#ifdef USE_EXCEPTIONS
		if (found == nullptr) throw std::runtime_error("Data not found.");
#endif
		return *found;
	}

	Datum& Scope::Append(const std::string& name) {
#ifdef USE_EXCEPTIONS
		if (name.empty()) throw std::runtime_error("Name cannot be blank.");
#endif

		auto [position, addedNew] = table.Insert(std::make_pair(name, Datum()));
		auto& pair = *position;

		if (addedNew) {
			orderVector.PushBack(&pair);
		}

		return pair.second;
	}

	Scope& Scope::AppendScope(const std::string& name) {
		Datum& datum = Append(name);

#ifdef USE_EXCEPTIONS
		if (datum.Type() != Datum::DatumTypes::Table && datum.Type() != Datum::DatumTypes::Unknown) throw std::runtime_error("Cannot append scope to non-scope data!");
#endif
	
		Scope* newChild = new Scope();
		newChild->parent = this;
	
		datum.PushBack(newChild);
	
		return *newChild;
	}

	Datum* Scope::Find(const std::string& toFind) {
		auto foundDatum = table.Find(toFind);
		
		if (foundDatum == table.end()) return nullptr;
		return &(foundDatum->second);
	}
		
	const Datum* Scope::Find(const std::string& toFind) const {
		auto foundDatum = table.Find(toFind);

		if (foundDatum == table.end()) return nullptr;
		return &(foundDatum->second);
	}

	Datum* Scope::Search(const std::string& toFind) {
		Scope* dummy;
		return Search(toFind, &dummy);
	}

	Datum* Scope::Search(const std::string& toFind, Scope** containingScope) {
		Datum* found = Find(toFind);
		if (found == nullptr) {
			if (parent == nullptr) {
				*containingScope = nullptr;
				return nullptr;
			}
			return parent->Search(toFind, containingScope);
		}

		*containingScope = this;
		return found;
	}

	std::pair<Datum*, Scope::size_type> Scope::FindContainedScope(const Scope& toFind) {
		for (auto& entry : orderVector) {
			Datum* currDatum = &entry->second;
			if (currDatum->Type() == Datum::DatumTypes::Table) {
				for (size_type i = 0; i < currDatum->Size(); ++i) {
					if (currDatum->GetScope(i) == &toFind) {
						return std::make_pair(currDatum, i);
					}
				}
			}
		}
		return std::make_pair(nullptr, Size());
	}

	Scope& Scope::Adopt(Scope& newChild, const std::string& name) {
#ifdef USE_EXCEPTIONS
		if (&newChild == this) throw std::runtime_error("Cannot adopt self.");
		if (newChild.IsAncestorOf(*this)) throw std::runtime_error("Cannot adopt ancestor.");
#endif

		Datum& datum = Append(name);
		datum.PushBack(&newChild);
		newChild.Orphan();
		newChild.parent = this;

		return newChild;
	}

	void Scope::Reparent() {
		for (auto pair : orderVector) {
			Datum& datum = pair->second;
			if (datum.Type() == Datum::DatumTypes::Table) {
				for (size_type i = 0; i < orderVector.Size(); ++i) {
					datum[i].parent = this;
				}
			}
		}
	}

	void Scope::Orphan() {
		if (parent == nullptr) return;

		auto [data, index] = parent->FindContainedScope(*this);
		assert(data != nullptr);

		data->RemoveAt(index);
		parent = nullptr;
	}

	bool Scope::IsAncestorOf(const Scope& scope) const {
		Scope* currParent = scope.parent;
		while (currParent != nullptr) {
			if (currParent == this) return true;
			currParent = currParent->parent;
		}
		return false;
	}

	void Scope::Clear() {
		for(auto iter = table.begin(); iter != table.end(); ++iter) {
			Datum& datum = iter->second;
			if (datum.Type() == Datum::DatumTypes::Table) {
				for (size_type i = 0; i < datum.Size(); ++i) {
					Scope* scope = datum.GetScope(i);
					scope->parent = nullptr;
					delete(scope);
				}
			}
		}

		table.Clear();
		orderVector.Clear();
	}

	bool Scope::Equals(const RTTI* rhs) const {
		if (rhs == nullptr) return false;

		const Scope* toCompare = rhs->As<Scope>();
		return (toCompare != nullptr ? *this == *toCompare : false);
	}

	std::string Scope::ToString() const { 
		std::string s = "Scope(";

		for (size_type i = 0; i < orderVector.Size(); ++i) {
			auto& [name, datum] = *orderVector[i];
			s += "\"" + name + "\"{";
			for (size_type j = 0; j < datum.Size(); ++j) {
				s += datum.ToString(j);
				if (j < datum.Size() - 1) s += ",";
			}
			s += "}";
			if (i < orderVector.Size() - 1) s += ",";
		}

		s += ")";
		return s; 
	}
}