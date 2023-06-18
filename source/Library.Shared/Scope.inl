#pragma once
#include "Scope.h"

namespace FieaGameEngine {
	inline bool Scope::operator!=(const Scope& rhs) const {
		return !(operator==(rhs));
	}

	inline Datum& Scope::operator[](const std::string& name) {
		return Append(name);
	}

	inline Datum& Scope::operator[](size_type index) {
		return orderVector[index]->second;
	}

	inline const Datum& Scope::operator[](size_type index) const {
		return orderVector[index]->second;
	}

	inline gsl::owner<Scope*> Scope::Clone() const {
		return new Scope(*this);
	}

	inline bool Scope::IsDescendantOf(const Scope& scope) const {
		return scope.IsAncestorOf(*this);
	}

	inline Scope* Scope::GetParent() const {
		return parent;
	}

	inline Scope::size_type Scope::Size() const {
		return orderVector.Size();
	}
}