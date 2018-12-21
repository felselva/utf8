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

#ifndef UTF8_H
#define UTF8_H

#include <stdint.h>

uint8_t utf8_validate_ascii(uint8_t *source);
void utf8_utf32(uint8_t *destination, uint8_t *end, uint32_t *source);
void utf8_double(uint8_t *destination, uint8_t *end, double v);
void utf8_float(uint8_t *destination, uint8_t *end, float v);
void utf8_int8(uint8_t *destination, uint8_t *end, int8_t v);
void utf8_int16(uint8_t *destination, uint8_t *end, int16_t v);
void utf8_int32(uint8_t *destination, uint8_t *end, int32_t v);
void utf8_uint8(uint8_t *destination, uint8_t *end, uint8_t v);
void utf8_uint16(uint8_t *destination, uint8_t *end, uint16_t v);
void utf8_uint32(uint8_t *destination, uint8_t *end, uint32_t v);
void utf8_copy(uint8_t *destination, uint8_t *end, uint8_t *source, uint32_t size);
void utf8_append(uint8_t *destination, uint8_t *end, uint8_t *source);
uint32_t utf8_index(uint8_t *source, uint8_t *reference);
void utf8_last(uint8_t *source, uint8_t **last);
void utf8_end(uint8_t *source, uint8_t **end);
uint8_t *utf8_next(uint8_t *source);
uint8_t *utf8_previous(uint8_t *source);
uint8_t *utf8_find_codepoint(uint8_t *source, uint32_t codepoint);
void utf8_decode(uint8_t *source, uint32_t *codepoint);
void utf8_encode(uint8_t *destination, uint32_t codepoint);
uint32_t utf8_length(uint8_t *text);

#endif
