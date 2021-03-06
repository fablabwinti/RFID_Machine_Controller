// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Dialog_plain_5Bitmaps[] PROGMEM = {

  // Bitmap Data:
  0x00, // ' '
  0xD0, // '!'
  0xA0, // '"'
  0x1F,0x80, // '#'
  0x5D,0xF4, // '$'
  0xEE,0x77, // '%'
  0x64,0x77, // '&'
  0x80, // '''
  0x60, // '('
  0x60, // ')'
  0xFC, // '*'
  0x5D,0x00, // '+'
  0x80, // ','
  0x80, // '-'
  0x80, // '.'
  0x5A, // '/'
  0xFF, // '0'
  0xC9,0x70, // '1'
  0xDF, // '2'
  0xDB, // '3'
  0x4B,0x70, // '4'
  0xEF, // '5'
  0xEF, // '6'
  0xD5, // '7'
  0xFB, // '8'
  0xF7, // '9'
  0xA0, // ':'
  0xA0, // ';'
  0x38,0x80, // '<'
  0xE3,0x80, // '='
  0x8E,0x00, // '>'
  0xC1, // '?'
  0x7D,0xE4, // '@'
  0x4B,0xF0, // 'A'
  0xF7,0x70, // 'B'
  0x72,0x30, // 'C'
  0xF6,0xF0, // 'D'
  0xEF, // 'E'
  0xEE, // 'F'
  0xFE,0xF0, // 'G'
  0xB7,0xD0, // 'H'
  0xF0, // 'I'
  0x55,0xC0, // 'J'
  0xFB,0x70, // 'K'
  0xAB, // 'L'
  0x49,0x7F,0xE8, // 'M'
  0xFF,0xD0, // 'N'
  0xF6,0xF0, // 'O'
  0xFA, // 'P'
  0xF6,0xE4, // 'Q'
  0xEF, // 'R'
  0xFC,0xF0, // 'S'
  0xE9,0x20, // 'T'
  0xB6,0xF0, // 'U'
  0xBD,0x20, // 'V'
  0xFE,0xD4,0xA0, // 'W'
  0xA9,0x50, // 'X'
  0xA9,0x20, // 'Y'
  0xEB,0x70, // 'Z'
  0xEB, // '['
  0xA5, // '\'
  0xD7, // ']'
  0xC0, // '^'
  0xE0, // '_'
  0x80, // '`'
  0x7C, // 'a'
  0xBF, // 'b'
  0xEC, // 'c'
  0x7F, // 'd'
  0xFC, // 'e'
  0xFA, // 'f'
  0xFF, // 'g'
  0xBF, // 'h'
  0xF0, // 'i'
  0x55,0xC0, // 'j'
  0xBB, // 'k'
  0xF0, // 'l'
  0xFF,0x80, // 'm'
  0xFC, // 'n'
  0xFC, // 'o'
  0xFE, // 'p'
  0xFD, // 'q'
  0xE8, // 'r'
  0xDC, // 's'
  0xBB, // 't'
  0xFC, // 'u'
  0xA9,0x00, // 'v'
  0xFF,0xA0, // 'w'
  0xEB,0x80, // 'x'
  0x52,0x64, // 'y'
  0xDC, // 'z'
  0x71,0x26, // '{'
  0xF8, // '|'
  0xC5,0x2C // '}'
};
const GFXglyph Dialog_plain_5Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
    {     0,   1,   1,   3,    0,    0 }, // ' '
    {     1,   1,   4,   3,    1,   -4 }, // '!'
    {     2,   3,   1,   3,    1,   -4 }, // '"'
    {     3,   3,   4,   5,    1,   -4 }, // '#'
    {     5,   3,   5,   4,    1,   -4 }, // '$'
    {     7,   4,   4,   6,    1,   -4 }, // '%'
    {     9,   4,   4,   5,    0,   -4 }, // '&'
    {    11,   1,   1,   2,    1,   -4 }, // '''
    {    12,   1,   4,   3,    1,   -4 }, // '('
    {    13,   1,   4,   3,    1,   -4 }, // ')'
    {    14,   3,   2,   4,    0,   -4 }, // '*'
    {    15,   3,   3,   5,    1,   -3 }, // '+'
    {    17,   1,   1,   3,    1,   -1 }, // ','
    {    18,   1,   1,   3,    1,   -2 }, // '-'
    {    19,   1,   1,   3,    1,   -1 }, // '.'
    {    20,   2,   4,   3,    0,   -4 }, // '/'
    {    21,   2,   4,   4,    1,   -4 }, // '0'
    {    22,   3,   4,   4,    0,   -4 }, // '1'
    {    24,   2,   4,   4,    1,   -4 }, // '2'
    {    25,   2,   4,   4,    1,   -4 }, // '3'
    {    26,   3,   4,   4,    1,   -4 }, // '4'
    {    28,   2,   4,   4,    1,   -4 }, // '5'
    {    29,   2,   4,   4,    1,   -4 }, // '6'
    {    30,   2,   4,   4,    1,   -4 }, // '7'
    {    31,   2,   4,   4,    1,   -4 }, // '8'
    {    32,   2,   4,   4,    1,   -4 }, // '9'
    {    33,   1,   3,   3,    1,   -3 }, // ':'
    {    34,   1,   3,   3,    1,   -3 }, // ';'
    {    35,   3,   3,   5,    1,   -3 }, // '<'
    {    37,   3,   3,   5,    1,   -3 }, // '='
    {    39,   3,   3,   5,    1,   -3 }, // '>'
    {    41,   2,   4,   4,    1,   -4 }, // '?'
    {    42,   4,   4,   6,    1,   -4 }, // '@'
    {    44,   3,   4,   4,    0,   -4 }, // 'A'
    {    46,   3,   4,   4,    1,   -4 }, // 'B'
    {    48,   3,   4,   4,    1,   -4 }, // 'C'
    {    50,   3,   4,   5,    1,   -4 }, // 'D'
    {    52,   2,   4,   4,    1,   -4 }, // 'E'
    {    53,   2,   4,   4,    1,   -4 }, // 'F'
    {    54,   3,   4,   5,    1,   -4 }, // 'G'
    {    56,   3,   4,   5,    1,   -4 }, // 'H'
    {    58,   1,   4,   2,    1,   -4 }, // 'I'
    {    59,   2,   5,   2,    0,   -4 }, // 'J'
    {    61,   3,   4,   4,    1,   -4 }, // 'K'
    {    63,   2,   4,   4,    1,   -4 }, // 'L'
    {    64,   3,   7,   5,    1,   -7 }, // 'M'
    {    67,   3,   4,   5,    1,   -4 }, // 'N'
    {    69,   3,   4,   5,    1,   -4 }, // 'O'
    {    71,   2,   4,   4,    1,   -4 }, // 'P'
    {    72,   3,   5,   5,    1,   -4 }, // 'Q'
    {    74,   2,   4,   4,    1,   -4 }, // 'R'
    {    75,   3,   4,   4,    1,   -4 }, // 'S'
    {    77,   3,   4,   4,    0,   -4 }, // 'T'
    {    79,   3,   4,   5,    1,   -4 }, // 'U'
    {    81,   3,   4,   4,    0,   -4 }, // 'V'
    {    83,   5,   4,   6,    0,   -4 }, // 'W'
    {    86,   3,   4,   4,    0,   -4 }, // 'X'
    {    88,   3,   4,   4,    0,   -4 }, // 'Y'
    {    90,   3,   4,   4,    1,   -4 }, // 'Z'
    {    92,   2,   4,   3,    1,   -4 }, // '['
    {    93,   2,   4,   3,    0,   -4 }, // '\'
    {    94,   2,   4,   3,    1,   -4 }, // ']'
    {    95,   3,   1,   5,    1,   -4 }, // '^'
    {    96,   3,   1,   4,    0,    0 }, // '_'
    {    97,   1,   1,   4,    1,   -4 }, // '`'
    {    98,   2,   3,   4,    1,   -3 }, // 'a'
    {    99,   2,   4,   4,    1,   -4 }, // 'b'
    {   100,   2,   3,   4,    1,   -3 }, // 'c'
    {   101,   2,   4,   4,    1,   -4 }, // 'd'
    {   102,   2,   3,   4,    1,   -3 }, // 'e'
    {   103,   2,   4,   3,    1,   -4 }, // 'f'
    {   104,   2,   4,   4,    1,   -3 }, // 'g'
    {   105,   2,   4,   4,    1,   -4 }, // 'h'
    {   106,   1,   4,   2,    1,   -4 }, // 'i'
    {   107,   2,   5,   2,    0,   -4 }, // 'j'
    {   109,   2,   4,   4,    1,   -4 }, // 'k'
    {   110,   1,   4,   2,    1,   -4 }, // 'l'
    {   111,   3,   3,   6,    1,   -3 }, // 'm'
    {   113,   2,   3,   4,    1,   -3 }, // 'n'
    {   114,   2,   3,   4,    1,   -3 }, // 'o'
    {   115,   2,   4,   4,    1,   -3 }, // 'p'
    {   116,   2,   4,   4,    1,   -3 }, // 'q'
    {   117,   2,   3,   3,    1,   -3 }, // 'r'
    {   118,   2,   3,   4,    1,   -3 }, // 's'
    {   119,   2,   4,   3,    1,   -4 }, // 't'
    {   120,   2,   3,   4,    1,   -3 }, // 'u'
    {   121,   3,   3,   4,    0,   -3 }, // 'v'
    {   123,   4,   3,   5,    0,   -3 }, // 'w'
    {   125,   3,   3,   4,    0,   -3 }, // 'x'
    {   127,   4,   4,   4,   -1,   -3 }, // 'y'
    {   129,   2,   3,   4,    1,   -3 }, // 'z'
    {   130,   3,   5,   4,    0,   -4 }, // '{'
    {   132,   1,   5,   3,    1,   -4 }, // '|'
    {   133,   3,   5,   4,    0,   -4 } // '}'
};
const GFXfont DialogPlain5 PROGMEM = {
(uint8_t  *)Dialog_plain_5Bitmaps,(GFXglyph *)Dialog_plain_5Glyphs,0x20, 0x7E, 7};