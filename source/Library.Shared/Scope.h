#pragma once
#include <gsl/gsl>
#include "RTTI.h"
#include "vector.h"
#include "HashMap.h"
#include "Datum.h"

namespace FieaGameEngine {

	class Scope : public RTTI {
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		using size_type = std::size_t;
		using table_type = HashMap<std::string, Datum>;
		using table_entry = table_type::pair_type;

		explicit Scope(size_type initialCapacity = 11);
		Scope(const Scope& rhs);
		Scope(Scope&& rhs) noexcept;
		Scope& operator=(const Scope& rhs);
		Scope& operator=(Scope&& rhs) noexcept;
		virtual ~Scope();

		[[nodiscard]] virtual gsl::owner<Scope*> Clone() const;

		[[nodiscard]] bool operator==(const Scope& rhs) const;
		[[nodiscard]] bool operator!=(const Scope& rhs) const;

		[[nodiscard]] Datum& operator[](const std::string& name);
		[[nodiscard]] const Datum& operator[](const std::string& name) const;
		[[nodiscard]] Datum& operator[](size_type index);
		[[nodiscard]] const Datum& operator[](size_type index) const;

		Datum& Append(const std::string& name);
		Scope& AppendScope(const std::string& name);

		[[nodiscard]] Datum* Find(const std::string& toFind);
		[[nodiscard]] const Datum* Find(const std::string& toFind) const;

		[[nodiscard]] Datum* Search(const std::string& toFind);
		[[nodiscard]] Datum* Search(const std::string& toFind, Scope** containingScope);

		[[nodiscard]] std::pair<Datum*, size_type> FindContainedScope(const Scope& toFind);

		Scope& Adopt(Scope& newChild, const std::string& name);
		void Reparent();
		void Orphan();

		[[nodiscard]] bool IsAncestorOf(const Scope& scope) const;
		[[nodiscard]] bool IsDescendantOf(const Scope& scope) const;

		[[nodiscard]] Scope* GetParent() const;
		[[nodiscard]] size_type Size() const;

		void Clear();

		[[nodiscard]] virtual bool Equals(const RTTI* rhs) const override;
		[[nodiscard]] virtual std::string ToString() const override;

	protected:
		Vector<table_entry*> orderVector;

	private:
		Scope* parent = nullptr;
		table_type table;
	};
}

#include "Scope.inl"