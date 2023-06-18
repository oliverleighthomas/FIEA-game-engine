#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"

namespace FieaGameEngine {
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(RTTI::IdType typeID) {
		(*this)["this"] = static_cast<RTTI*>(this);
		Populate(TypeManager::Instance()->FindType(typeID));
	}

	Attributed::Attributed(const Attributed& rhs) :
		Scope(rhs)
	{
		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->FindType(rhs.TypeIdInstance()));
	}

	Attributed::Attributed(Attributed&& rhs) noexcept :
		Scope(std::move(rhs))
	{
		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->FindType(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(const Attributed& rhs) {
		if (this != &rhs) {
			Scope::operator=(rhs);
			(*this)["this"] = static_cast<RTTI*>(this);
			UpdateExternalStorage(TypeManager::Instance()->FindType(rhs.TypeIdInstance()));
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept {
		if (this != &rhs) {
			Scope::operator=(std::move(rhs));
			(*this)["this"] = static_cast<RTTI*>(this);
			UpdateExternalStorage(TypeManager::Instance()->FindType(rhs.TypeIdInstance()));
		}
		return *this;
	}

	bool Attributed::operator==(const Attributed& rhs) const {
		if (this == &rhs) return true;
		if (Attributes().Size() != rhs.Attributes().Size()) return false;

		for (size_t i = 1; i < Size(); ++i) {
			auto found = rhs.Find(Attributes()[i]->first);
			if (found == nullptr || Attributes()[i]->second != *found) return false;
		}

		return true;
	}

	bool Attributed::operator!=(const Attributed& rhs) const {
		return !(operator==(rhs));
	}

	void Attributed::Populate(Type* type) {
		auto* parentType = TypeManager::Instance()->FindType(type->parentTypeId);
		if (parentType) Populate(parentType);

		for (const auto& signature : type->signatures) {
			Datum& datum = Append(signature.name);
			datum.SetType(signature.type);
			if (datum.Type() != Datum::DatumTypes::Table) datum.SetStorage(reinterpret_cast<char*>(this) + signature.offset, signature.size);
		}
	}

	void Attributed::UpdateExternalStorage(Type* type) {
		auto* parentType = TypeManager::Instance()->FindType(type->parentTypeId);
		if (parentType) UpdateExternalStorage(parentType);

		for (const auto& signature : type->signatures) {
			Datum& datum = Append(signature.name);
			if (signature.type != Datum::DatumTypes::Table) datum.SetStorage(reinterpret_cast<char*>(this) + signature.offset, signature.size);
		}
	}

	bool Attributed::IsAttribute(const std::string& name) const {
		return (Find(name) != nullptr);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const {
		if (name == "this") return true;

		auto* type = TypeManager::Instance()->FindType(TypeIdInstance());
		for (const auto& signature : type->signatures) {
			if (signature.name == name) return true;
		}

		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const {
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name) {
		if (IsPrescribedAttribute(name)) {
			throw std::runtime_error("Cannot append perscribed attribute!");
		}
		return Append(name);
	}

	Vector<Scope::table_entry*> Attributed::Attributes() const {
		return orderVector;
	}

	Vector<Scope::table_entry*> Attributed::PrescribedAttributes() const {
		Vector<Scope::table_entry*>perscribedAttributes;
		for (const auto& data : orderVector) {
			if (IsPrescribedAttribute(data->first)) {
				perscribedAttributes.PushBack(data);
			}
		}
		return perscribedAttributes;
	}

	Vector<Scope::table_entry*> Attributed::AuxiliaryAttributes() const {
		Vector<Scope::table_entry*> auxiliaryAttributes;
		for (const auto& data : orderVector) {
			if (IsAuxiliaryAttribute(data->first)) {
				auxiliaryAttributes.PushBack(data);
			}
		}
		return auxiliaryAttributes;
	}

	bool Attributed::Equals(const RTTI* rhs) const {
		if (rhs == nullptr) return false;

		const Attributed* toCompare = rhs->As<Attributed>();
		return (toCompare != nullptr ? *this == *toCompare : false);
	}

	std::string Attributed::ToString() const {
		std::string s = "Attributed(\"this\"{}";

		for (size_type i = 1; i < Attributes().Size(); ++i) {
			auto& [name, datum] = *Attributes()[i];
			s += ",\"" + name + "\"{";
			for (size_type j = 0; j < datum.Size(); ++j) {
				s += datum.ToString(j);
				if (j < datum.Size() - 1) s += ",";
			}
			s += "}";
		}

		s += ")";
		return s;
	}
}