#pragma once

#include <cstdint>


enum class ReleaseType : std::uint8_t {
	NEW,
	FULL,
	UPDATE
};