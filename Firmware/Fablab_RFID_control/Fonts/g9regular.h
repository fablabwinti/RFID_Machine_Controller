/*
Adafruit-GFX font modeled loosely after Geneva 9pt, with compressed
descenders and diacritics so as to fit in an 8-pixel line height.

Copyright 2018 Christian Walther

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

static const uint8_t g9regularBitmaps[] PROGMEM = {
	 /* 0x20 SPACE */
	0xFA,  /* 0x21 EXCLAMATION MARK */
	0xB4,  /* 0x22 QUOTATION MARK */
	0x57, 0xD5, 0xF5, 0x00,  /* 0x23 NUMBER SIGN */
	0x75, 0x68, 0xE2, 0xD5, 0xC4,  /* 0x24 DOLLAR SIGN */
	0x42, 0xA4, 0xA8, 0x4A, 0x15, 0x25, 0x42,  /* 0x25 PERCENT SIGN */
	0x21, 0x45, 0x18, 0x96, 0x27, 0x40,  /* 0x26 AMPERSAND */
	0x58,  /* 0x27 APOSTROPHE */
	0x2A, 0x49, 0x11,  /* 0x28 LEFT PARENTHESIS */
	0x88, 0x92, 0x54,  /* 0x29 RIGHT PARENTHESIS */
	0x51, 0x3E, 0x45, 0x00,  /* 0x2A ASTERISK */
	0x21, 0x3E, 0x42, 0x00,  /* 0x2B PLUS SIGN */
	0x58,  /* 0x2C COMMA */
	0xF0,  /* 0x2D HYPHEN-MINUS */
	0x80,  /* 0x2E FULL STOP */
	0x12, 0x24, 0x48, 0x80,  /* 0x2F SOLIDUS */
	0x74, 0x63, 0x18, 0xC5, 0xC0,  /* 0x30 DIGIT ZERO */
	0x75, 0x54,  /* 0x31 DIGIT ONE */
	0x74, 0x42, 0x22, 0x23, 0xE0,  /* 0x32 DIGIT TWO */
	0xF8, 0x88, 0xE0, 0xC5, 0xC0,  /* 0x33 DIGIT THREE */
	0x11, 0x95, 0x2F, 0x88, 0x40,  /* 0x34 DIGIT FOUR */
	0xFC, 0x3C, 0x10, 0xC5, 0xC0,  /* 0x35 DIGIT FIVE */
	0x32, 0x21, 0xE8, 0xC5, 0xC0,  /* 0x36 DIGIT SIX */
	0xF8, 0x44, 0x42, 0x21, 0x00,  /* 0x37 DIGIT SEVEN */
	0x74, 0x62, 0xE8, 0xC5, 0xC0,  /* 0x38 DIGIT EIGHT */
	0x74, 0x62, 0xF0, 0x89, 0x80,  /* 0x39 DIGIT NINE */
	0x88,  /* 0x3A COLON */
	0x41, 0x60,  /* 0x3B SEMICOLON */
	0x2A, 0x22,  /* 0x3C LESS-THAN SIGN */
	0xF8, 0x3E,  /* 0x3D EQUALS SIGN */
	0x88, 0xA8,  /* 0x3E GREATER-THAN SIGN */
	0x69, 0x12, 0x40, 0x40,  /* 0x3F QUESTION MARK */
	0x38, 0x8A, 0x6D, 0x5A, 0xB2, 0x90, 0x1C,  /* 0x40 COMMERCIAL AT */
	0x21, 0x14, 0xAF, 0xC6, 0x20,  /* 0x41 LATIN CAPITAL LETTER A */
	0xF4, 0x63, 0xE8, 0xC7, 0xC0,  /* 0x42 LATIN CAPITAL LETTER B */
	0x74, 0x61, 0x08, 0x45, 0xC0,  /* 0x43 LATIN CAPITAL LETTER C */
	0xE4, 0xA3, 0x18, 0xCB, 0x80,  /* 0x44 LATIN CAPITAL LETTER D */
	0xF8, 0x8E, 0x88, 0xF0,  /* 0x45 LATIN CAPITAL LETTER E */
	0xF8, 0x8E, 0x88, 0x80,  /* 0x46 LATIN CAPITAL LETTER F */
	0x74, 0x61, 0x38, 0xC5, 0xC0,  /* 0x47 LATIN CAPITAL LETTER G */
	0x8C, 0x63, 0xF8, 0xC6, 0x20,  /* 0x48 LATIN CAPITAL LETTER H */
	0xFE,  /* 0x49 LATIN CAPITAL LETTER I */
	0x11, 0x11, 0x19, 0x60,  /* 0x4A LATIN CAPITAL LETTER J */
	0x8C, 0xA9, 0x8A, 0x4A, 0x20,  /* 0x4B LATIN CAPITAL LETTER K */
	0x88, 0x88, 0x88, 0xF0,  /* 0x4C LATIN CAPITAL LETTER L */
	0x83, 0x8E, 0xAC, 0x98, 0x30, 0x60, 0x80,  /* 0x4D LATIN CAPITAL LETTER M */
	0xCE, 0x6B, 0x59, 0xCE, 0x20,  /* 0x4E LATIN CAPITAL LETTER N */
	0x74, 0x63, 0x18, 0xC5, 0xC0,  /* 0x4F LATIN CAPITAL LETTER O */
	0xF4, 0x63, 0xE8, 0x42, 0x00,  /* 0x50 LATIN CAPITAL LETTER P */
	0x74, 0x63, 0x18, 0xD5, 0xC2,  /* 0x51 LATIN CAPITAL LETTER Q */
	0xF4, 0x63, 0xEA, 0x4A, 0x20,  /* 0x52 LATIN CAPITAL LETTER R */
	0x74, 0x60, 0xE0, 0xC5, 0xC0,  /* 0x53 LATIN CAPITAL LETTER S */
	0xF9, 0x08, 0x42, 0x10, 0x80,  /* 0x54 LATIN CAPITAL LETTER T */
	0x8C, 0x63, 0x18, 0xC5, 0xC0,  /* 0x55 LATIN CAPITAL LETTER U */
	0x8C, 0x62, 0xA5, 0x10, 0x80,  /* 0x56 LATIN CAPITAL LETTER V */
	0x83, 0x05, 0x52, 0xA2, 0x85, 0x0A, 0x00,  /* 0x57 LATIN CAPITAL LETTER W */
	0x8C, 0x54, 0x45, 0x46, 0x20,  /* 0x58 LATIN CAPITAL LETTER X */
	0x8C, 0x54, 0x42, 0x10, 0x80,  /* 0x59 LATIN CAPITAL LETTER Y */
	0xF1, 0x24, 0x88, 0xF0,  /* 0x5A LATIN CAPITAL LETTER Z */
	0xEA, 0xAB,  /* 0x5B LEFT SQUARE BRACKET */
	0x84, 0x42, 0x21, 0x10,  /* 0x5C REVERSE SOLIDUS */
	0xD5, 0x57,  /* 0x5D RIGHT SQUARE BRACKET */
	0x54,  /* 0x5E CIRCUMFLEX ACCENT */
	0xF8,  /* 0x5F LOW LINE */
	0x90,  /* 0x60 GRAVE ACCENT */
	0x61, 0x79, 0x70,  /* 0x61 LATIN SMALL LETTER A */
	0x88, 0xE9, 0x99, 0xE0,  /* 0x62 LATIN SMALL LETTER B */
	0x69, 0x89, 0x60,  /* 0x63 LATIN SMALL LETTER C */
	0x11, 0x79, 0x99, 0x70,  /* 0x64 LATIN SMALL LETTER D */
	0x69, 0xF8, 0x60,  /* 0x65 LATIN SMALL LETTER E */
	0x34, 0xE4, 0x44, 0x40,  /* 0x66 LATIN SMALL LETTER F */
	0x79, 0x97, 0x16,  /* 0x67 LATIN SMALL LETTER G */
	0x88, 0xE9, 0x99, 0x90,  /* 0x68 LATIN SMALL LETTER H */
	0xBE,  /* 0x69 LATIN SMALL LETTER I */
	0x45, 0x56,  /* 0x6A LATIN SMALL LETTER J */
	0x88, 0x9A, 0xCA, 0x90,  /* 0x6B LATIN SMALL LETTER K */
	0xFE,  /* 0x6C LATIN SMALL LETTER L */
	0xED, 0x26, 0x4C, 0x99, 0x20,  /* 0x6D LATIN SMALL LETTER M */
	0xE9, 0x99, 0x90,  /* 0x6E LATIN SMALL LETTER N */
	0x69, 0x99, 0x60,  /* 0x6F LATIN SMALL LETTER O */
	0xE9, 0x9E, 0x88,  /* 0x70 LATIN SMALL LETTER P */
	0x79, 0x97, 0x11,  /* 0x71 LATIN SMALL LETTER Q */
	0xBC, 0x88, 0x80,  /* 0x72 LATIN SMALL LETTER R */
	0x78, 0x61, 0xE0,  /* 0x73 LATIN SMALL LETTER S */
	0x4B, 0xA4, 0x88,  /* 0x74 LATIN SMALL LETTER T */
	0x99, 0x99, 0x70,  /* 0x75 LATIN SMALL LETTER U */
	0x8A, 0x94, 0x42, 0x00,  /* 0x76 LATIN SMALL LETTER V */
	0x82, 0xA9, 0x51, 0x42, 0x80,  /* 0x77 LATIN SMALL LETTER W */
	0x8A, 0x88, 0xA8, 0x80,  /* 0x78 LATIN SMALL LETTER X */
	0x99, 0x97, 0x16,  /* 0x79 LATIN SMALL LETTER Y */
	0xF2, 0x48, 0xF0,  /* 0x7A LATIN SMALL LETTER Z */
	0x29, 0x44, 0x91,  /* 0x7B LEFT CURLY BRACKET */
	0xFE,  /* 0x7C VERTICAL LINE */
	0x89, 0x14, 0x94,  /* 0x7D RIGHT CURLY BRACKET */
	0x6D, 0x80,  /* 0x7E TILDE */
	 /* 0x7F */
	 /* 0x80 */
	 /* 0x81 */
	 /* 0x82 */
	 /* 0x83 */
	 /* 0x84 */
	 /* 0x85 */
	 /* 0x86 */
	 /* 0x87 */
	 /* 0x88 */
	 /* 0x89 */
	 /* 0x8A */
	 /* 0x8B */
	 /* 0x8C */
	 /* 0x8D */
	 /* 0x8E */
	 /* 0x8F */
	 /* 0x90 */
	 /* 0x91 */
	 /* 0x92 */
	 /* 0x93 */
	 /* 0x94 */
	 /* 0x95 */
	 /* 0x96 */
	 /* 0x97 */
	 /* 0x98 */
	 /* 0x99 */
	 /* 0x9A */
	 /* 0x9B */
	 /* 0x9C */
	 /* 0x9D */
	 /* 0x9E */
	 /* 0x9F */
	 /* 0xA0 NO-BREAK SPACE */
	0xBE,  /* 0xA1 INVERTED EXCLAMATION MARK */
	0x27, 0xAA, 0xA7, 0x20,  /* 0xA2 CENT SIGN */
	0x34, 0x4E, 0x48, 0xF0,  /* 0xA3 POUND SIGN */
	0xB5, 0x24, 0x92, 0xB4,  /* 0xA4 CURRENCY SIGN */
	0x8C, 0x55, 0xF2, 0x7C, 0x80,  /* 0xA5 YEN SIGN */
	0xEE,  /* 0xA6 BROKEN BAR */
	0x78, 0xE9, 0x71, 0xE0,  /* 0xA7 SECTION SIGN */
	0xA0,  /* 0xA8 DIAERESIS */
	0x3C, 0x42, 0x99, 0xA1, 0xA1, 0x99, 0x42, 0x3C,  /* 0xA9 COPYRIGHT SIGN */
	0x79, 0x97, 0x0F,  /* 0xAA FEMININE ORDINAL INDICATOR */
	0x2A, 0xA8, 0xA2, 0x80,  /* 0xAB LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
	0xF1,  /* 0xAC NOT SIGN */
	0xF0,  /* 0xAD SOFT HYPHEN */
	0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,  /* 0xAE REGISTERED SIGN */
	0xE0,  /* 0xAF MACRON */
	0x55, 0x00,  /* 0xB0 DEGREE SIGN */
	0x21, 0x3E, 0x42, 0x7C,  /* 0xB1 PLUS-MINUS SIGN */
	0xC5, 0x4E,  /* 0xB2 SUPERSCRIPT TWO */
	0xC5, 0x1C,  /* 0xB3 SUPERSCRIPT THREE */
	0x60,  /* 0xB4 ACUTE ACCENT */
	0x99, 0x99, 0xE8,  /* 0xB5 MICRO SIGN */
	0x7F, 0xAE, 0x9A, 0x28, 0xA2, 0x8A,  /* 0xB6 PILCROW SIGN */
	0xF0,  /* 0xB7 MIDDLE DOT */
	0x47, 0x00,  /* 0xB8 CEDILLA */
	0x75, 0x40,  /* 0xB9 SUPERSCRIPT ONE */
	0x69, 0x96, 0x0F,  /* 0xBA MASCULINE ORDINAL INDICATOR */
	0xA2, 0x8A, 0xAA, 0x00,  /* 0xBB RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
	0x89, 0x22, 0x6D, 0x54, 0xE8, 0x60, 0x80,  /* 0xBC VULGAR FRACTION ONE QUARTER */
	0x89, 0x22, 0x75, 0x14, 0x49, 0x23, 0x80,  /* 0xBD VULGAR FRACTION ONE HALF */
	0xC4, 0x28, 0x4D, 0x35, 0xE7, 0x21, 0x41,  /* 0xBE VULGAR FRACTION THREE QUARTERS */
	0x20, 0x24, 0x89, 0x60,  /* 0xBF INVERTED QUESTION MARK */
	0x41, 0x14, 0xAF, 0xC6, 0x20,  /* 0xC0 LATIN CAPITAL LETTER A WITH GRAVE */
	0x11, 0x14, 0xAF, 0xC6, 0x20,  /* 0xC1 LATIN CAPITAL LETTER A WITH ACUTE */
	0x22, 0x88, 0xA5, 0x7E, 0x20,  /* 0xC2 LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
	0x55, 0x08, 0xA5, 0x7E, 0x20,  /* 0xC3 LATIN CAPITAL LETTER A WITH TILDE */
	0xA9, 0x14, 0xAF, 0xC6, 0x20,  /* 0xC4 LATIN CAPITAL LETTER A WITH DIAERESIS */
	0x72, 0x88, 0xA5, 0x7E, 0x20,  /* 0xC5 LATIN CAPITAL LETTER A WITH RING ABOVE */
	0x1F, 0x28, 0x28, 0x7F, 0x48, 0x88, 0x8F,  /* 0xC6 LATIN CAPITAL LETTER AE */
	0x74, 0x61, 0x08, 0x45, 0xC4,  /* 0xC7 LATIN CAPITAL LETTER C WITH CEDILLA */
	0x4F, 0x8E, 0x88, 0xF0,  /* 0xC8 LATIN CAPITAL LETTER E WITH GRAVE */
	0x2F, 0x8E, 0x88, 0xF0,  /* 0xC9 LATIN CAPITAL LETTER E WITH ACUTE */
	0x6F, 0x8E, 0x88, 0xF0,  /* 0xCA LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
	0x5F, 0x8E, 0x88, 0xF0,  /* 0xCB LATIN CAPITAL LETTER E WITH DIAERESIS */
	0x95, 0x54,  /* 0xCC LATIN CAPITAL LETTER I WITH GRAVE */
	0x6A, 0xA8,  /* 0xCD LATIN CAPITAL LETTER I WITH ACUTE */
	0x55, 0x24, 0x90,  /* 0xCE LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
	0xA9, 0x24, 0x90,  /* 0xCF LATIN CAPITAL LETTER I WITH DIAERESIS */
	0x71, 0x24, 0x79, 0x45, 0x27, 0x00,  /* 0xD0 LATIN CAPITAL LETTER ETH */
	0x2A, 0xB3, 0x5A, 0xCE, 0x20,  /* 0xD1 LATIN CAPITAL LETTER N WITH TILDE */
	0x13, 0x23, 0x18, 0xC5, 0xC0,  /* 0xD2 LATIN CAPITAL LETTER O WITH GRAVE */
	0x41, 0xA3, 0x18, 0xC5, 0xC0,  /* 0xD3 LATIN CAPITAL LETTER O WITH ACUTE */
	0x23, 0xA3, 0x18, 0xC5, 0xC0,  /* 0xD4 LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
	0x2E, 0xA3, 0x18, 0xC5, 0xC0,  /* 0xD5 LATIN CAPITAL LETTER O WITH TILDE */
	0x8B, 0xA3, 0x18, 0xC5, 0xC0,  /* 0xD6 LATIN CAPITAL LETTER O WITH DIAERESIS */
	0xAA, 0x80,  /* 0xD7 MULTIPLICATION SIGN */
	0x3A, 0x89, 0x32, 0xA6, 0x48, 0xAE, 0x00,  /* 0xD8 LATIN CAPITAL LETTER O WITH STROKE */
	0xAC, 0xE3, 0x18, 0xC5, 0xC0,  /* 0xD9 LATIN CAPITAL LETTER U WITH GRAVE */
	0xAE, 0x63, 0x18, 0xC5, 0xC0,  /* 0xDA LATIN CAPITAL LETTER U WITH ACUTE */
	0xAE, 0xE3, 0x18, 0xC5, 0xC0,  /* 0xDB LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
	0x48, 0x23, 0x18, 0xC5, 0xC0,  /* 0xDC LATIN CAPITAL LETTER U WITH DIAERESIS */
	0x15, 0x62, 0xA2, 0x10, 0x80,  /* 0xDD LATIN CAPITAL LETTER Y WITH ACUTE */
	0x87, 0xA3, 0x18, 0xFA, 0x00,  /* 0xDE LATIN CAPITAL LETTER THORN */
	0x64, 0xA9, 0x68, 0xC6, 0xC0,  /* 0xDF LATIN SMALL LETTER SHARP S */
	0x42, 0x61, 0x79, 0x70,  /* 0xE0 LATIN SMALL LETTER A WITH GRAVE */
	0x24, 0x61, 0x79, 0x70,  /* 0xE1 LATIN SMALL LETTER A WITH ACUTE */
	0x25, 0x61, 0x79, 0x70,  /* 0xE2 LATIN SMALL LETTER A WITH CIRCUMFLEX */
	0x5A, 0x61, 0x79, 0x70,  /* 0xE3 LATIN SMALL LETTER A WITH TILDE */
	0x50, 0x61, 0x79, 0x70,  /* 0xE4 LATIN SMALL LETTER A WITH DIAERESIS */
	0x75, 0x61, 0x79, 0x70,  /* 0xE5 LATIN SMALL LETTER A WITH RING ABOVE */
	0x6C, 0x25, 0xFC, 0x86, 0xC0,  /* 0xE6 LATIN SMALL LETTER AE */
	0x69, 0x87, 0x26,  /* 0xE7 LATIN SMALL LETTER C WITH CEDILLA */
	0x42, 0x69, 0xF8, 0x60,  /* 0xE8 LATIN SMALL LETTER E WITH GRAVE */
	0x24, 0x69, 0xF8, 0x60,  /* 0xE9 LATIN SMALL LETTER E WITH ACUTE */
	0x25, 0x69, 0xF8, 0x60,  /* 0xEA LATIN SMALL LETTER E WITH CIRCUMFLEX */
	0x50, 0x69, 0xF8, 0x60,  /* 0xEB LATIN SMALL LETTER E WITH DIAERESIS */
	0x92, 0xA8,  /* 0xEC LATIN SMALL LETTER I WITH GRAVE */
	0x62, 0xA8,  /* 0xED LATIN SMALL LETTER I WITH ACUTE */
	0x54, 0x24, 0x90,  /* 0xEE LATIN SMALL LETTER I WITH CIRCUMFLEX */
	0xA1, 0x24, 0x90,  /* 0xEF LATIN SMALL LETTER I WITH DIAERESIS */
	0xD1, 0x04, 0xF8, 0xC5, 0xC0,  /* 0xF0 LATIN SMALL LETTER ETH */
	0x5A, 0xE9, 0x99, 0x90,  /* 0xF1 LATIN SMALL LETTER N WITH TILDE */
	0x42, 0x69, 0x99, 0x60,  /* 0xF2 LATIN SMALL LETTER O WITH GRAVE */
	0x24, 0x69, 0x99, 0x60,  /* 0xF3 LATIN SMALL LETTER O WITH ACUTE */
	0x25, 0x69, 0x99, 0x60,  /* 0xF4 LATIN SMALL LETTER O WITH CIRCUMFLEX */
	0x5A, 0x69, 0x99, 0x60,  /* 0xF5 LATIN SMALL LETTER O WITH TILDE */
	0x90, 0x69, 0x99, 0x60,  /* 0xF6 LATIN SMALL LETTER O WITH DIAERESIS */
	0x20, 0x3E, 0x02, 0x00,  /* 0xF7 DIVISION SIGN */
	0x35, 0x66, 0x92, 0xB0,  /* 0xF8 LATIN SMALL LETTER O WITH STROKE */
	0x42, 0x99, 0x99, 0x70,  /* 0xF9 LATIN SMALL LETTER U WITH GRAVE */
	0x24, 0x99, 0x99, 0x70,  /* 0xFA LATIN SMALL LETTER U WITH ACUTE */
	0x25, 0x89, 0x99, 0x70,  /* 0xFB LATIN SMALL LETTER U WITH CIRCUMFLEX */
	0x90, 0x99, 0x99, 0x70,  /* 0xFC LATIN SMALL LETTER U WITH DIAERESIS */
	0x24, 0x99, 0x97, 0x16,  /* 0xFD LATIN SMALL LETTER Y WITH ACUTE */
	0x88, 0xE9, 0x99, 0xE8,  /* 0xFE LATIN SMALL LETTER THORN */
	0x90, 0x99, 0x97, 0x16,  /* 0xFF LATIN SMALL LETTER Y WITH DIAERESIS */
};

static const GFXglyph g9regularGlyphs[] PROGMEM = {
	/* bitmapOffset, width, height, xAdvance, xOffset, yOffset */
	{ 0, 0, 0, 3, 0, -7 },  /* 0x20 SPACE */
	{ 0, 1, 7, 3, 1, -7 },  /* 0x21 EXCLAMATION MARK */
	{ 1, 3, 2, 6, 1, -7 },  /* 0x22 QUOTATION MARK */
	{ 2, 5, 5, 6, 0, -7 },  /* 0x23 NUMBER SIGN */
	{ 6, 5, 8, 6, 0, -7 },  /* 0x24 DOLLAR SIGN */
	{ 11, 8, 7, 8, 0, -7 },  /* 0x25 PERCENT SIGN */
	{ 18, 6, 7, 7, 0, -7 },  /* 0x26 AMPERSAND */
	{ 24, 2, 3, 4, 1, -7 },  /* 0x27 APOSTROPHE */
	{ 25, 3, 8, 4, 0, -7 },  /* 0x28 LEFT PARENTHESIS */
	{ 28, 3, 8, 4, 0, -7 },  /* 0x29 RIGHT PARENTHESIS */
	{ 31, 5, 5, 6, 0, -7 },  /* 0x2A ASTERISK */
	{ 35, 5, 5, 6, 0, -6 },  /* 0x2B PLUS SIGN */
	{ 39, 2, 3, 3, 0, -2 },  /* 0x2C COMMA */
	{ 40, 4, 1, 5, 0, -4 },  /* 0x2D HYPHEN-MINUS */
	{ 41, 1, 1, 3, 1, -1 },  /* 0x2E FULL STOP */
	{ 42, 4, 7, 5, 0, -7 },  /* 0x2F SOLIDUS */
	{ 46, 5, 7, 6, 0, -7 },  /* 0x30 DIGIT ZERO */
	{ 51, 2, 7, 6, 1, -7 },  /* 0x31 DIGIT ONE */
	{ 53, 5, 7, 6, 0, -7 },  /* 0x32 DIGIT TWO */
	{ 58, 5, 7, 6, 0, -7 },  /* 0x33 DIGIT THREE */
	{ 63, 5, 7, 6, 0, -7 },  /* 0x34 DIGIT FOUR */
	{ 68, 5, 7, 6, 0, -7 },  /* 0x35 DIGIT FIVE */
	{ 73, 5, 7, 6, 0, -7 },  /* 0x36 DIGIT SIX */
	{ 78, 5, 7, 6, 0, -7 },  /* 0x37 DIGIT SEVEN */
	{ 83, 5, 7, 6, 0, -7 },  /* 0x38 DIGIT EIGHT */
	{ 88, 5, 7, 6, 0, -7 },  /* 0x39 DIGIT NINE */
	{ 93, 1, 5, 3, 1, -5 },  /* 0x3A COLON */
	{ 94, 2, 6, 3, 0, -5 },  /* 0x3B SEMICOLON */
	{ 96, 3, 5, 4, 0, -6 },  /* 0x3C LESS-THAN SIGN */
	{ 98, 5, 3, 6, 0, -5 },  /* 0x3D EQUALS SIGN */
	{ 100, 3, 5, 4, 0, -6 },  /* 0x3E GREATER-THAN SIGN */
	{ 102, 4, 7, 6, 1, -7 },  /* 0x3F QUESTION MARK */
	{ 106, 7, 8, 8, 0, -7 },  /* 0x40 COMMERCIAL AT */
	{ 113, 5, 7, 6, 0, -7 },  /* 0x41 LATIN CAPITAL LETTER A */
	{ 118, 5, 7, 6, 0, -7 },  /* 0x42 LATIN CAPITAL LETTER B */
	{ 123, 5, 7, 6, 0, -7 },  /* 0x43 LATIN CAPITAL LETTER C */
	{ 128, 5, 7, 6, 0, -7 },  /* 0x44 LATIN CAPITAL LETTER D */
	{ 133, 4, 7, 5, 0, -7 },  /* 0x45 LATIN CAPITAL LETTER E */
	{ 137, 4, 7, 5, 0, -7 },  /* 0x46 LATIN CAPITAL LETTER F */
	{ 141, 5, 7, 6, 0, -7 },  /* 0x47 LATIN CAPITAL LETTER G */
	{ 146, 5, 7, 6, 0, -7 },  /* 0x48 LATIN CAPITAL LETTER H */
	{ 151, 1, 7, 3, 1, -7 },  /* 0x49 LATIN CAPITAL LETTER I */
	{ 152, 4, 7, 5, 0, -7 },  /* 0x4A LATIN CAPITAL LETTER J */
	{ 156, 5, 7, 6, 0, -7 },  /* 0x4B LATIN CAPITAL LETTER K */
	{ 161, 4, 7, 5, 0, -7 },  /* 0x4C LATIN CAPITAL LETTER L */
	{ 165, 7, 7, 8, 0, -7 },  /* 0x4D LATIN CAPITAL LETTER M */
	{ 172, 5, 7, 6, 0, -7 },  /* 0x4E LATIN CAPITAL LETTER N */
	{ 177, 5, 7, 6, 0, -7 },  /* 0x4F LATIN CAPITAL LETTER O */
	{ 182, 5, 7, 6, 0, -7 },  /* 0x50 LATIN CAPITAL LETTER P */
	{ 187, 5, 8, 6, 0, -7 },  /* 0x51 LATIN CAPITAL LETTER Q */
	{ 192, 5, 7, 6, 0, -7 },  /* 0x52 LATIN CAPITAL LETTER R */
	{ 197, 5, 7, 6, 0, -7 },  /* 0x53 LATIN CAPITAL LETTER S */
	{ 202, 5, 7, 6, 0, -7 },  /* 0x54 LATIN CAPITAL LETTER T */
	{ 207, 5, 7, 6, 0, -7 },  /* 0x55 LATIN CAPITAL LETTER U */
	{ 212, 5, 7, 6, 0, -7 },  /* 0x56 LATIN CAPITAL LETTER V */
	{ 217, 7, 7, 8, 0, -7 },  /* 0x57 LATIN CAPITAL LETTER W */
	{ 224, 5, 7, 6, 0, -7 },  /* 0x58 LATIN CAPITAL LETTER X */
	{ 229, 5, 7, 6, 0, -7 },  /* 0x59 LATIN CAPITAL LETTER Y */
	{ 234, 4, 7, 5, 0, -7 },  /* 0x5A LATIN CAPITAL LETTER Z */
	{ 238, 2, 8, 4, 1, -7 },  /* 0x5B LEFT SQUARE BRACKET */
	{ 240, 4, 7, 5, 0, -7 },  /* 0x5C REVERSE SOLIDUS */
	{ 244, 2, 8, 4, 1, -7 },  /* 0x5D RIGHT SQUARE BRACKET */
	{ 246, 3, 2, 4, 0, -7 },  /* 0x5E CIRCUMFLEX ACCENT */
	{ 247, 5, 1, 5, 0, 0 },  /* 0x5F LOW LINE */
	{ 248, 2, 2, 4, 1, -7 },  /* 0x60 GRAVE ACCENT */
	{ 249, 4, 5, 5, 0, -5 },  /* 0x61 LATIN SMALL LETTER A */
	{ 252, 4, 7, 5, 0, -7 },  /* 0x62 LATIN SMALL LETTER B */
	{ 256, 4, 5, 5, 0, -5 },  /* 0x63 LATIN SMALL LETTER C */
	{ 259, 4, 7, 5, 0, -7 },  /* 0x64 LATIN SMALL LETTER D */
	{ 263, 4, 5, 5, 0, -5 },  /* 0x65 LATIN SMALL LETTER E */
	{ 266, 4, 7, 4, 0, -7 },  /* 0x66 LATIN SMALL LETTER F */
	{ 270, 4, 6, 5, 0, -5 },  /* 0x67 LATIN SMALL LETTER G */
	{ 273, 4, 7, 5, 0, -7 },  /* 0x68 LATIN SMALL LETTER H */
	{ 277, 1, 7, 2, 0, -7 },  /* 0x69 LATIN SMALL LETTER I */
	{ 278, 2, 8, 3, 0, -7 },  /* 0x6A LATIN SMALL LETTER J */
	{ 280, 4, 7, 5, 0, -7 },  /* 0x6B LATIN SMALL LETTER K */
	{ 284, 1, 7, 2, 0, -7 },  /* 0x6C LATIN SMALL LETTER L */
	{ 285, 7, 5, 8, 0, -5 },  /* 0x6D LATIN SMALL LETTER M */
	{ 290, 4, 5, 5, 0, -5 },  /* 0x6E LATIN SMALL LETTER N */
	{ 293, 4, 5, 5, 0, -5 },  /* 0x6F LATIN SMALL LETTER O */
	{ 296, 4, 6, 5, 0, -5 },  /* 0x70 LATIN SMALL LETTER P */
	{ 299, 4, 6, 5, 0, -5 },  /* 0x71 LATIN SMALL LETTER Q */
	{ 302, 4, 5, 5, 0, -5 },  /* 0x72 LATIN SMALL LETTER R */
	{ 305, 4, 5, 5, 0, -5 },  /* 0x73 LATIN SMALL LETTER S */
	{ 308, 3, 7, 4, 0, -7 },  /* 0x74 LATIN SMALL LETTER T */
	{ 311, 4, 5, 5, 0, -5 },  /* 0x75 LATIN SMALL LETTER U */
	{ 314, 5, 5, 6, 0, -5 },  /* 0x76 LATIN SMALL LETTER V */
	{ 318, 7, 5, 8, 0, -5 },  /* 0x77 LATIN SMALL LETTER W */
	{ 323, 5, 5, 6, 0, -5 },  /* 0x78 LATIN SMALL LETTER X */
	{ 327, 4, 6, 5, 0, -5 },  /* 0x79 LATIN SMALL LETTER Y */
	{ 330, 4, 5, 5, 0, -5 },  /* 0x7A LATIN SMALL LETTER Z */
	{ 333, 3, 8, 4, 0, -7 },  /* 0x7B LEFT CURLY BRACKET */
	{ 336, 1, 7, 2, 0, -7 },  /* 0x7C VERTICAL LINE */
	{ 337, 3, 8, 4, 0, -7 },  /* 0x7D RIGHT CURLY BRACKET */
	{ 340, 5, 2, 6, 0, -4 },  /* 0x7E TILDE */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x7F */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x80 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x81 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x82 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x83 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x84 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x85 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x86 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x87 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x88 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x89 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x8A */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x8B */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x8C */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x8D */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x8E */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x8F */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x90 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x91 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x92 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x93 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x94 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x95 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x96 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x97 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x98 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x99 */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x9A */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x9B */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x9C */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x9D */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x9E */
	{ 342, 0, 0, 1, 0, -7 },  /* 0x9F */
	{ 342, 0, 0, 3, 0, -7 },  /* 0xA0 NO-BREAK SPACE */
	{ 342, 1, 7, 3, 1, -6 },  /* 0xA1 INVERTED EXCLAMATION MARK */
	{ 343, 4, 7, 5, 0, -7 },  /* 0xA2 CENT SIGN */
	{ 347, 4, 7, 5, 0, -7 },  /* 0xA3 POUND SIGN */
	{ 351, 6, 5, 7, 0, -6 },  /* 0xA4 CURRENCY SIGN */
	{ 355, 5, 7, 6, 0, -7 },  /* 0xA5 YEN SIGN */
	{ 360, 1, 7, 2, 0, -7 },  /* 0xA6 BROKEN BAR */
	{ 361, 4, 7, 5, 0, -7 },  /* 0xA7 SECTION SIGN */
	{ 365, 3, 1, 4, 0, -7 },  /* 0xA8 DIAERESIS */
	{ 366, 8, 8, 9, 0, -7 },  /* 0xA9 COPYRIGHT SIGN */
	{ 374, 4, 6, 5, 0, -7 },  /* 0xAA FEMININE ORDINAL INDICATOR */
	{ 377, 5, 5, 6, 0, -6 },  /* 0xAB LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{ 381, 4, 2, 5, 0, -3 },  /* 0xAC NOT SIGN */
	{ 382, 4, 1, 5, 0, -4 },  /* 0xAD SOFT HYPHEN */
	{ 383, 8, 8, 9, 0, -7 },  /* 0xAE REGISTERED SIGN */
	{ 391, 3, 1, 4, 0, -7 },  /* 0xAF MACRON */
	{ 392, 3, 3, 4, 0, -7 },  /* 0xB0 DEGREE SIGN */
	{ 394, 5, 6, 6, 0, -7 },  /* 0xB1 PLUS-MINUS SIGN */
	{ 398, 3, 5, 4, 0, -7 },  /* 0xB2 SUPERSCRIPT TWO */
	{ 400, 3, 5, 4, 0, -7 },  /* 0xB3 SUPERSCRIPT THREE */
	{ 402, 2, 2, 4, 1, -7 },  /* 0xB4 ACUTE ACCENT */
	{ 403, 4, 6, 5, 0, -5 },  /* 0xB5 MICRO SIGN */
	{ 406, 6, 8, 6, 0, -7 },  /* 0xB6 PILCROW SIGN */
	{ 412, 2, 2, 5, 1, -4 },  /* 0xB7 MIDDLE DOT */
	{ 413, 3, 3, 4, 0, -2 },  /* 0xB8 CEDILLA */
	{ 415, 2, 5, 4, 0, -7 },  /* 0xB9 SUPERSCRIPT ONE */
	{ 417, 4, 6, 5, 0, -7 },  /* 0xBA MASCULINE ORDINAL INDICATOR */
	{ 420, 5, 5, 6, 0, -6 },  /* 0xBB RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
	{ 424, 7, 7, 8, 0, -7 },  /* 0xBC VULGAR FRACTION ONE QUARTER */
	{ 431, 7, 7, 8, 0, -7 },  /* 0xBD VULGAR FRACTION ONE HALF */
	{ 438, 8, 7, 9, 0, -7 },  /* 0xBE VULGAR FRACTION THREE QUARTERS */
	{ 445, 4, 7, 5, 0, -6 },  /* 0xBF INVERTED QUESTION MARK */
	{ 449, 5, 7, 6, 0, -7 },  /* 0xC0 LATIN CAPITAL LETTER A WITH GRAVE */
	{ 454, 5, 7, 6, 0, -7 },  /* 0xC1 LATIN CAPITAL LETTER A WITH ACUTE */
	{ 459, 5, 7, 6, 0, -7 },  /* 0xC2 LATIN CAPITAL LETTER A WITH CIRCUMFLEX */
	{ 464, 5, 7, 6, 0, -7 },  /* 0xC3 LATIN CAPITAL LETTER A WITH TILDE */
	{ 469, 5, 7, 6, 0, -7 },  /* 0xC4 LATIN CAPITAL LETTER A WITH DIAERESIS */
	{ 474, 5, 7, 6, 0, -7 },  /* 0xC5 LATIN CAPITAL LETTER A WITH RING ABOVE */
	{ 479, 8, 7, 9, 0, -7 },  /* 0xC6 LATIN CAPITAL LETTER AE */
	{ 486, 5, 8, 6, 0, -7 },  /* 0xC7 LATIN CAPITAL LETTER C WITH CEDILLA */
	{ 491, 4, 7, 5, 0, -7 },  /* 0xC8 LATIN CAPITAL LETTER E WITH GRAVE */
	{ 495, 4, 7, 5, 0, -7 },  /* 0xC9 LATIN CAPITAL LETTER E WITH ACUTE */
	{ 499, 4, 7, 5, 0, -7 },  /* 0xCA LATIN CAPITAL LETTER E WITH CIRCUMFLEX */
	{ 503, 4, 7, 5, 0, -7 },  /* 0xCB LATIN CAPITAL LETTER E WITH DIAERESIS */
	{ 507, 2, 7, 3, 0, -7 },  /* 0xCC LATIN CAPITAL LETTER I WITH GRAVE */
	{ 509, 2, 7, 3, 1, -7 },  /* 0xCD LATIN CAPITAL LETTER I WITH ACUTE */
	{ 511, 3, 7, 3, 0, -7 },  /* 0xCE LATIN CAPITAL LETTER I WITH CIRCUMFLEX */
	{ 514, 3, 7, 3, 0, -7 },  /* 0xCF LATIN CAPITAL LETTER I WITH DIAERESIS */
	{ 517, 6, 7, 7, 0, -7 },  /* 0xD0 LATIN CAPITAL LETTER ETH */
	{ 523, 5, 7, 6, 0, -7 },  /* 0xD1 LATIN CAPITAL LETTER N WITH TILDE */
	{ 528, 5, 7, 6, 0, -7 },  /* 0xD2 LATIN CAPITAL LETTER O WITH GRAVE */
	{ 533, 5, 7, 6, 0, -7 },  /* 0xD3 LATIN CAPITAL LETTER O WITH ACUTE */
	{ 538, 5, 7, 6, 0, -7 },  /* 0xD4 LATIN CAPITAL LETTER O WITH CIRCUMFLEX */
	{ 543, 5, 7, 6, 0, -7 },  /* 0xD5 LATIN CAPITAL LETTER O WITH TILDE */
	{ 548, 5, 7, 6, 0, -7 },  /* 0xD6 LATIN CAPITAL LETTER O WITH DIAERESIS */
	{ 553, 3, 3, 4, 0, -5 },  /* 0xD7 MULTIPLICATION SIGN */
	{ 555, 7, 7, 7, 0, -7 },  /* 0xD8 LATIN CAPITAL LETTER O WITH STROKE */
	{ 562, 5, 7, 6, 0, -7 },  /* 0xD9 LATIN CAPITAL LETTER U WITH GRAVE */
	{ 567, 5, 7, 6, 0, -7 },  /* 0xDA LATIN CAPITAL LETTER U WITH ACUTE */
	{ 572, 5, 7, 6, 0, -7 },  /* 0xDB LATIN CAPITAL LETTER U WITH CIRCUMFLEX */
	{ 577, 5, 7, 6, 0, -7 },  /* 0xDC LATIN CAPITAL LETTER U WITH DIAERESIS */
	{ 582, 5, 7, 6, 0, -7 },  /* 0xDD LATIN CAPITAL LETTER Y WITH ACUTE */
	{ 587, 5, 7, 6, 0, -7 },  /* 0xDE LATIN CAPITAL LETTER THORN */
	{ 592, 5, 7, 6, 0, -7 },  /* 0xDF LATIN SMALL LETTER SHARP S */
	{ 597, 4, 7, 5, 0, -7 },  /* 0xE0 LATIN SMALL LETTER A WITH GRAVE */
	{ 601, 4, 7, 5, 0, -7 },  /* 0xE1 LATIN SMALL LETTER A WITH ACUTE */
	{ 605, 4, 7, 5, 0, -7 },  /* 0xE2 LATIN SMALL LETTER A WITH CIRCUMFLEX */
	{ 609, 4, 7, 5, 0, -7 },  /* 0xE3 LATIN SMALL LETTER A WITH TILDE */
	{ 613, 4, 7, 5, 0, -7 },  /* 0xE4 LATIN SMALL LETTER A WITH DIAERESIS */
	{ 617, 4, 7, 5, 0, -7 },  /* 0xE5 LATIN SMALL LETTER A WITH RING ABOVE */
	{ 621, 7, 5, 8, 0, -5 },  /* 0xE6 LATIN SMALL LETTER AE */
	{ 626, 4, 6, 5, 0, -5 },  /* 0xE7 LATIN SMALL LETTER C WITH CEDILLA */
	{ 629, 4, 7, 5, 0, -7 },  /* 0xE8 LATIN SMALL LETTER E WITH GRAVE */
	{ 633, 4, 7, 5, 0, -7 },  /* 0xE9 LATIN SMALL LETTER E WITH ACUTE */
	{ 637, 4, 7, 5, 0, -7 },  /* 0xEA LATIN SMALL LETTER E WITH CIRCUMFLEX */
	{ 641, 4, 7, 5, 0, -7 },  /* 0xEB LATIN SMALL LETTER E WITH DIAERESIS */
	{ 645, 2, 7, 3, 0, -7 },  /* 0xEC LATIN SMALL LETTER I WITH GRAVE */
	{ 647, 2, 7, 2, 0, -7 },  /* 0xED LATIN SMALL LETTER I WITH ACUTE */
	{ 649, 3, 7, 4, 0, -7 },  /* 0xEE LATIN SMALL LETTER I WITH CIRCUMFLEX */
	{ 652, 3, 7, 4, 0, -7 },  /* 0xEF LATIN SMALL LETTER I WITH DIAERESIS */
	{ 655, 5, 7, 6, 0, -7 },  /* 0xF0 LATIN SMALL LETTER ETH */
	{ 660, 4, 7, 5, 0, -7 },  /* 0xF1 LATIN SMALL LETTER N WITH TILDE */
	{ 664, 4, 7, 5, 0, -7 },  /* 0xF2 LATIN SMALL LETTER O WITH GRAVE */
	{ 668, 4, 7, 5, 0, -7 },  /* 0xF3 LATIN SMALL LETTER O WITH ACUTE */
	{ 672, 4, 7, 5, 0, -7 },  /* 0xF4 LATIN SMALL LETTER O WITH CIRCUMFLEX */
	{ 676, 4, 7, 5, 0, -7 },  /* 0xF5 LATIN SMALL LETTER O WITH TILDE */
	{ 680, 4, 7, 5, 0, -7 },  /* 0xF6 LATIN SMALL LETTER O WITH DIAERESIS */
	{ 684, 5, 5, 6, 0, -6 },  /* 0xF7 DIVISION SIGN */
	{ 688, 6, 5, 6, 0, -5 },  /* 0xF8 LATIN SMALL LETTER O WITH STROKE */
	{ 692, 4, 7, 5, 0, -7 },  /* 0xF9 LATIN SMALL LETTER U WITH GRAVE */
	{ 696, 4, 7, 5, 0, -7 },  /* 0xFA LATIN SMALL LETTER U WITH ACUTE */
	{ 700, 4, 7, 5, 0, -7 },  /* 0xFB LATIN SMALL LETTER U WITH CIRCUMFLEX */
	{ 704, 4, 7, 5, 0, -7 },  /* 0xFC LATIN SMALL LETTER U WITH DIAERESIS */
	{ 708, 4, 8, 5, 0, -7 },  /* 0xFD LATIN SMALL LETTER Y WITH ACUTE */
	{ 712, 4, 8, 5, 0, -7 },  /* 0xFE LATIN SMALL LETTER THORN */
	{ 716, 4, 8, 5, 0, -7 },  /* 0xFF LATIN SMALL LETTER Y WITH DIAERESIS */
};

const GFXfont g9regularFont PROGMEM = {
	(uint8_t*)g9regularBitmaps,
	(GFXglyph*)g9regularGlyphs,
	0x20, 0xFF, /* first, last */
	8 /* yAdvance */
};
