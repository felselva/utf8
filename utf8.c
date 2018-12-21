/*
Copyright © 2018 Felipe Ferreira da Silva

This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

  1. The origin of this software must not be misrepresented; you must not claim
     that you wrote the original software. If you use this software in a
     product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <stdint.h>
#include <stddef.h>
#include "utf8.h"

void utf8_copy(uint8_t *destination, uint8_t *end, uint8_t *source, uint32_t size)
{
	uint8_t *destination_position;
	uint8_t *source_position;
	uint32_t count;
	count = size;
	destination_position = destination;
	source_position = source;
	while (destination_position != end) {
		if (source_position[0] != 0 && count > 0) {
			destination_position[0] = source_position[0];
			source_position = source_position + 1;
			count = count - 1;
		}
		destination_position = destination_position + 1;
	}
	destination_position[0] = 0;
}

void utf8_append(uint8_t *destination, uint8_t *end, uint8_t *source)
{
	uint8_t *destination_position;
	uint8_t *source_position;
	destination_position = destination;
	while (destination_position[0] != 0 && destination_position < end) {
		destination_position = destination_position + 1;
	}
	source_position = source;
	while (destination_position < end) {
		if (source_position[0] != 0) {
			destination_position[0] = source_position[0];
			source_position = source_position + 1;
		}
		destination_position = destination_position + 1;
	}
	destination_position[0] = 0;
}

uint32_t utf8_index(uint8_t *source, uint8_t *reference)
{
	uint32_t i;
	uint8_t *position;
	i = 0;
	position = source;
	while (position != reference) {
		position = utf8_next(position);
		i = i + 1;
	}
	return i;
}

void utf8_last(uint8_t *source, uint8_t **last)
{
	uint32_t codepoint;
	uint8_t *position;
	uint8_t *previous_position;
	position = source;
	previous_position = position;
	position = utf8_next(position);
	utf8_decode(position, &codepoint);
	while (codepoint != 0) {
		previous_position = position;
		position = utf8_next(position);
		utf8_decode(position, &codepoint);
	}
	last[0] = previous_position;
}

void utf8_end(uint8_t *source, uint8_t **end)
{
	uint32_t codepoint;
	uint8_t *position;
	position = source;
	utf8_decode(position, &codepoint);
	while (codepoint != 0) {
		position = utf8_next(position);
		utf8_decode(position, &codepoint);
	}
	if (codepoint != 0) {
		position = utf8_next(position);
	}
	end[0] = position;
}

#define MAXUNICODE 0x10FFFF

uint8_t *utf8_next(uint8_t *source)
{
	uint8_t *p;
	p = source;
	if (p[0] < 0xC0u) {
		p = p + 1;
	} else {
		p = p + 1;
		while ((p[0] & 0xC0u) == 0x80u) {
			p = p + 1;
		}
	}
	return p;
}

uint8_t *utf8_previous(uint8_t *source)
{
	uint8_t *previous;
	previous = source - 1;
	if (previous[0] >> 6 == 0x00000010) {
		previous = previous - 1;
		if (previous[0] >> 6 == 0x00000010) {
			previous = previous - 1;
			if (previous[0] >> 6 == 0x00000010) {
				previous = previous - 1;
			}
		}
	}
	return previous;
}

uint8_t *utf8_find_codepoint(uint8_t *source, uint32_t codepoint)
{
	uint8_t *position;
	uint32_t source_codepoint;
	position = source;
	utf8_decode(position, &source_codepoint);
	while (codepoint != source_codepoint) {
		position = utf8_next(position);
		utf8_decode(position, &source_codepoint);
	}
	return position;
}

void utf8_decode(uint8_t *source, uint32_t *codepoint)
{
	uint32_t limits[4];
	uint32_t c;
	uint32_t res;
	limits[0] = 0xFF;
	limits[1] = 0x7F;
	limits[2] = 0x7FF;
	limits[3] = 0xFFFF;
	c = source[0];
	res = 0;
	if (c < 0x80) {
		res = c;
	} else {
		uint32_t count;
		count = 0;
		while (c & 0x40) {
			uint32_t cc;
			count = count + 1;
			cc = source[count];
			if ((cc & 0xC0) != 0x80) {
				res = 0;
				goto done;
			}
			res = (res << 6) | (cc & 0x3F);
			c <<= 1;
		}
		res = res | ((c & 0x7F) << (count * 5));
		if (count > 3 || res > MAXUNICODE || res <= limits[count]) {
			res = 0;
		}
	}
done:
	codepoint[0] = res;
}

uint32_t utf8_length(uint8_t *source)
{
	uint32_t l;
	uint32_t codepoint;
	uint8_t *position;
	l = 0;
	position = source;
	utf8_decode(source, &codepoint);
	while (codepoint != 0) {
		l = l + 1;
		position = utf8_next(position);
		utf8_decode(position, &codepoint);
	}
	return l;
}

void utf8_encode(uint8_t *destination, uint32_t codepoint)
{
	if (codepoint <= 0x7F) {
		destination[0] = (uint8_t)codepoint;
		destination[1] = 0;
	} else if (codepoint <= 0x07FF) {
		destination[0] = (uint8_t)(((codepoint >> 6) & 0x1F) | 0xC0);
		destination[1] = (uint8_t)(((codepoint >> 0) & 0x3F) | 0x80);
		destination[2] = 0;
	} else if (codepoint <= 0xFFFF) {
		destination[0] = (uint8_t)(((codepoint >> 12) & 0x0F) | 0xE0);
		destination[1] = (uint8_t)(((codepoint >>  6) & 0x3F) | 0x80);
		destination[2] = (uint8_t)(((codepoint >>  0) & 0x3F) | 0x80);
		destination[3] = 0;
	} else if (codepoint <= 0x10FFFF) {
		destination[0] = (uint8_t)(((codepoint >> 18) & 0x07) | 0xF0);
		destination[1] = (uint8_t)(((codepoint >> 12) & 0x3F) | 0x80);
		destination[2] = (uint8_t)(((codepoint >>  6) & 0x3F) | 0x80);
		destination[3] = (uint8_t)(((codepoint >>  0) & 0x3F) | 0x80);
		destination[4] = 0;
	} else {
		destination[0] = (uint8_t)0xEF;
		destination[1] = (uint8_t)0xBF;
		destination[2] = (uint8_t)0xBD;
		destination[3] = 0;
	}
}
