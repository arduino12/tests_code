#ifndef UTF8_H
#define UTF8_H

#include "common.h"

#define UNICODE_BITS		16

#define unicode_t			UINT_BITS(UNICODE_BITS)

static const unicode_t UTF8_OFFSETS[] = {
	0x00000000,
	0x00003080,
#if UNICODE_BITS > 16
	0x000E2080,
	0x03C82080,
#endif
#if UNICODE_BITS > 32
	0xFA082080,
	0x82082080,
#endif
};

uint8_t utf8_trailing(uint8_t c) {
	if (c < 192) return 0;
#if UNICODE_BITS > 16
	if (c < 224) return 1;
	if (c < 240) return 2;
#endif
#if UNICODE_BITS > 32
	if (c < 248) return 3;
	if (c < 252) return 4;
#endif
#if UNICODE_BITS == 16
	return 1;
#elif UNICODE_BITS == 32
	return 3;
#else
	return 5;
#endif
}

uint16_t utf8_to_unicode(const uint8_t * src, unicode_t * dst, uint16_t dst_size) {
	unicode_t ch;
	uint16_t i = 0;
	uint8_t trailing;

	while (i < (dst_size - 1) && *src) {
		trailing = utf8_trailing(*src);
		ch = 0;
		switch (trailing) {
#if UNICODE_BITS > 32
			case 5: ch += *src++; ch <<= 6;
			case 4: ch += *src++; ch <<= 6;
#endif
#if UNICODE_BITS > 16
			case 3: ch += *src++; ch <<= 6;
			case 2: ch += *src++; ch <<= 6;
#endif
			case 1: ch += *src++; ch <<= 6;
			case 0: ch += *src++;
		}
		dst[i++] = ch - UTF8_OFFSETS[trailing];
	}

	dst[i] = 0;
	return i;
}

#endif /* UTF8_H */
