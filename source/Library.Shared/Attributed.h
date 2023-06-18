#pragma once
#include "Scope.h"

namespace FieaGameEngine {
	struct Type;
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:
		Attributed(const Attributed& rhs);
		Attributed(Attributed&& rhs) noexcept;
		Attributed& operator=(const Attributed& rhs);
		Attributed& operator=(Attributed&& rhs) noexcept;
		virtual ~Attributed() = default;

		[[nodiscard]] virtual gsl::owner<Attributed*> Clone() const override = 0;

		[[nodiscard]] bool operator==(const Attributed& rhs) const;
		[[nodiscard]] bool operator!=(const Attributed& rhs) const;

		[[nodiscard]] bool IsAttribute(const std::string& name) const;
		[[nodiscard]] bool IsPrescribedAttribute(const std::string& name) const;
		[[nodiscard]] bool IsAuxiliaryAttribute(const std::string& name) const;

		Datum& AppendAuxiliaryAttribute(const std::string& name);

		[[nodiscard]] Vector<Scope::table_entry*> Attributes() const;
		[[nodiscard]] Vector<Scope::table_entry*> PrescribedAttributes() const;
		[[nodiscard]] Vector<Scope::table_entry*> AuxiliaryAttributes() const;

		[[nodiscard]] virtual bool Equals(const RTTI* rhs) const override;
		[[nodiscard]] virtual std::string ToString() const override;

	protected:
		explicit Attributed(RTTI::IdType typeID);
		void Populate(Type* type);
		void UpdateExternalStorage(Type* type);
	};
}