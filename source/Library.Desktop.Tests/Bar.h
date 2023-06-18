#pragma once

#include <cstdint>
#include <string>
#include <assert.h>
#include "RTTI.h"

namespace UnitTests
{
	class Bar final : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Bar, FieaGameEngine::RTTI);

	public:
		explicit Bar(std::int32_t data = 0);
		Bar(const Bar& rhs);
		Bar(Bar&& rhs) noexcept;
		Bar& operator=(const Bar& rhs);
		Bar& operator=(Bar&& rhs) noexcept;
		virtual ~Bar();

		bool Equals(const RTTI* rhs) const override;

		std::string ToString() const override;

		std::int32_t Data() const;
		void SetData(std::int32_t data);

	private:
		std::int32_t* _data;
	};
}