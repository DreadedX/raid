#pragma once

#include "typedef.h"

typedef uint32_t crc32;

const uint32 polynomial = 0xEDB88320;

inline crc32 crc32_bitwiser(const void* data, size_t length, crc32 previous_crc = 0) {
	crc32 crc = ~previous_crc;
	const byte* current = static_cast<const byte*>(data);

	while(length--) {
		crc ^= *current++;
		for (unsigned int i = 0; i < 8; ++i) {
			crc = (crc >> 1) ^ (-int(crc & 1) & polynomial);
		}
	}

	return ~crc;
}
