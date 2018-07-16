// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Dialog_bold_5Bitmaps[] PROGMEM = {

  // Bitmap Data:
  0x00, // ' '
  0xD0, // '!'
  0xA0, // '"'
  0x6F,0x6F, // '#'
  0x5F,0xF4, // '$'
  0xD7,0x8E,0xB0, // '%'
  0x64,0xFF, // '&'
  0x80, // '''
  0x69, // '('
  0x96, // ')'
  0xEB,0x80, // '*'
  0x5D,0x00, // '+'
  0xC0, // ','
  0xC0, // '-'
  0x80, // '.'
  0x6A, // '/'
  0xF6,0xF0, // '0'
  0xC9,0x70, // '1'
  0xE5,0xF0, // '2'
  0xEC,0xF0, // '3'
  0x66,0xAF, // '4'
  0xF3,0xF0, // '5'
  0x73,0xF0, // '6'
  0xED,0x20, // '7'
  0xEA,0xF0, // '8'
  0xFC,0xE0, // '9'
  0xA0, // ':'
  0xB0, // ';'
  0x39,0x80, // '<'
  0xE3,0x80, // '='
  0x8F,0x00, // '>'
  0xD1, // '?'
  0x6F,0xF7, // '@'
  0x66,0x6F,0x00, // 'A'
  0xFE,0xF0, // 'B'
  0x72,0x30, // 'C'
  0xF6,0xF0, // 'D'
  0xFE,0x70, // 'E'
  0xFE,0x40, // 'F'
  0x73,0xF0, // 'G'
  0xBE,0xD0, // 'H'
  0xF0, // 'I'
  0x55,0xC0, // 'J'
  0xAC,0xCA, // 'K'
  0x92,0x70, // 'L'
  0xFF,0xD9, // 'M'
  0xFF,0xD0, // 'N'
  0x69,0x96, // 'O'
  0xFE,0x40, // 'P'
  0x69,0x96,0x20, // 'Q'
  0xFB,0xF0, // 'R'
  0xF1,0xF0, // 'S'
  0xE9,0x20, // 'T'
  0xB6,0xF0, // 'U'
  0xF6,0x66, // 'V'
  0xFF,0xDC,0xE2,0x00, // 'W'
  0x0F,0x66,0xF0, // 'X'
  0xE9,0x20, // 'Y'
  0xFF,0x70, // 'Z'
  0xEB, // '['
  0xA9, // '\'
  0xD7, // ']'
  0xC0, // '^'
  0xE0, // '_'
  0x80, // '`'
  0xFF,0x80, // 'a'
  0x93,0xDE, // 'b'
  0xEC, // 'c'
  0x27,0xDE, // 'd'
  0xF3,0x80, // 'e'
  0x6B,0xA4, // 'f'
  0xF7,0xF0, // 'g'
  0x93,0xDA, // 'h'
  0xB8, // 'i'
  0x45,0x70, // 'j'
  0x92,0xEA, // 'k'
  0xF8, // 'l'
  0xFD,0x6A, // 'm'
  0xF6,0x80, // 'n'
  0xF7,0x80, // 'o'
  0xF7,0xC0, // 'p'
  0xF7,0x90, // 'q'
  0xE8, // 'r'
  0xC3,0xEC,0x00, // 's'
  0xBB, // 't'
  0xB7,0x80, // 'u'
  0x4B,0xA4, // 'v'
  0x0F,0xF6, // 'w'
  0x5D,0x74, // 'x'
  0x02,0x27,0x66,0x60, // 'y'
  0xDF,0x00, // 'z'
  0x6B,0x26, // '{'
  0xF8, // '|'
  0xC9,0xAC // '}'
};
const GFXglyph Dialog_bold_5Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
    {     0,   1,   1,   3,    0,    0 }, // ' '
    {     1,   1,   4,   3,    1,   -4 }, // '!'
    {     2,   3,   1,   4,    1,   -4 }, // '"'
    {     3,   4,   4,   5,    0,   -4 }, // '#'
    {     5,   3,   5,   4,    0,   -4 }, // '$'
    {     7,   5,   4,   6,    1,   -4 }, // '%'
    {    10,   4,   4,   5,    1,   -4 }, // '&'
    {    12,   1,   1,   3,    1,   -4 }, // '''
    {    13,   2,   4,   3,    1,   -5 }, // '('
    {    14,   2,   4,   3,    0,   -5 }, // ')'
    {    15,   3,   3,   4,    0,   -4 }, // '*'
    {    17,   3,   3,   5,    1,   -3 }, // '+'
    {    19,   1,   2,   3,    1,   -1 }, // ','
    {    20,   2,   1,   3,    0,   -2 }, // '-'
    {    21,   1,   1,   3,    1,   -1 }, // '.'
    {    22,   2,   4,   3,    0,   -4 }, // '/'
    {    23,   3,   4,   4,    1,   -4 }, // '0'
    {    25,   3,   4,   4,    0,   -4 }, // '1'
    {    27,   3,   4,   4,    1,   -4 }, // '2'
    {    29,   3,   4,   4,    1,   -4 }, // '3'
    {    31,   4,   4,   4,    1,   -4 }, // '4'
    {    33,   3,   4,   4,    1,   -4 }, // '5'
    {    35,   3,   4,   4,    1,   -4 }, // '6'
    {    37,   3,   4,   4,    1,   -4 }, // '7'
    {    39,   3,   4,   4,    1,   -4 }, // '8'
    {    41,   3,   4,   4,    1,   -4 }, // '9'
    {    43,   1,   3,   3,    1,   -3 }, // ':'
    {    44,   1,   4,   3,    1,   -3 }, // ';'
    {    45,   3,   3,   5,    1,   -3 }, // '<'
    {    47,   3,   3,   5,    1,   -3 }, // '='
    {    49,   3,   3,   5,    1,   -3 }, // '>'
    {    51,   2,   4,   4,    0,   -4 }, // '?'
    {    52,   4,   4,   6,    0,   -4 }, // '@'
    {    54,   4,   5,   5,    0,   -4 }, // 'A'
    {    57,   3,   4,   5,    1,   -4 }, // 'B'
    {    59,   3,   4,   5,    1,   -4 }, // 'C'
    {    61,   3,   4,   5,    1,   -4 }, // 'D'
    {    63,   3,   4,   4,    1,   -4 }, // 'E'
    {    65,   3,   4,   4,    1,   -4 }, // 'F'
    {    67,   3,   4,   5,    1,   -4 }, // 'G'
    {    69,   3,   4,   5,    1,   -4 }, // 'H'
    {    71,   1,   4,   3,    1,   -4 }, // 'I'
    {    72,   2,   5,   3,    0,   -4 }, // 'J'
    {    74,   4,   4,   5,    1,   -4 }, // 'K'
    {    76,   3,   4,   4,    1,   -4 }, // 'L'
    {    78,   4,   4,   6,    1,   -4 }, // 'M'
    {    80,   3,   4,   5,    1,   -4 }, // 'N'
    {    82,   4,   4,   5,    1,   -4 }, // 'O'
    {    84,   3,   4,   5,    1,   -4 }, // 'P'
    {    86,   4,   5,   5,    1,   -4 }, // 'Q'
    {    89,   3,   4,   5,    1,   -4 }, // 'R'
    {    91,   3,   4,   5,    1,   -4 }, // 'S'
    {    93,   3,   4,   4,    0,   -4 }, // 'T'
    {    95,   3,   4,   5,    1,   -4 }, // 'U'
    {    97,   4,   4,   5,    0,   -4 }, // 'V'
    {    99,   5,   6,   7,    0,   -4 }, // 'W'
    {   103,   4,   6,   5,    0,   -5 }, // 'X'
    {   106,   3,   4,   5,    0,   -4 }, // 'Y'
    {   108,   3,   4,   5,    0,   -4 }, // 'Z'
    {   110,   2,   4,   3,    1,   -5 }, // '['
    {   111,   2,   4,   3,    0,   -4 }, // '\'
    {   112,   2,   4,   3,    0,   -5 }, // ']'
    {   113,   3,   1,   5,    1,   -4 }, // '^'
    {   114,   3,   1,   4,    0,    0 }, // '_'
    {   115,   1,   1,   4,    0,   -4 }, // '`'
    {   116,   3,   3,   4,    1,   -3 }, // 'a'
    {   118,   3,   5,   5,    1,   -5 }, // 'b'
    {   120,   2,   3,   4,    1,   -3 }, // 'c'
    {   121,   3,   5,   5,    1,   -5 }, // 'd'
    {   123,   3,   3,   4,    1,   -3 }, // 'e'
    {   125,   3,   5,   3,    0,   -5 }, // 'f'
    {   127,   3,   4,   5,    1,   -3 }, // 'g'
    {   129,   3,   5,   5,    1,   -5 }, // 'h'
    {   131,   1,   5,   3,    1,   -5 }, // 'i'
    {   132,   2,   6,   3,    0,   -5 }, // 'j'
    {   134,   3,   5,   4,    1,   -5 }, // 'k'
    {   136,   1,   5,   3,    1,   -5 }, // 'l'
    {   137,   5,   3,   6,    1,   -3 }, // 'm'
    {   139,   3,   3,   5,    1,   -3 }, // 'n'
    {   141,   3,   3,   4,    1,   -3 }, // 'o'
    {   143,   3,   4,   5,    1,   -3 }, // 'p'
    {   145,   3,   4,   5,    1,   -3 }, // 'q'
    {   147,   2,   3,   3,    1,   -3 }, // 'r'
    {   148,   6,   3,   4,    1,   -3 }, // 's'
    {   151,   2,   4,   3,    1,   -4 }, // 't'
    {   152,   3,   3,   5,    1,   -3 }, // 'u'
    {   154,   3,   5,   4,    0,   -5 }, // 'v'
    {   156,   4,   4,   6,    0,   -4 }, // 'w'
    {   158,   3,   5,   4,    0,   -4 }, // 'x'
    {   160,   4,   7,   4,   -1,   -6 }, // 'y'
    {   164,   3,   3,   4,    0,   -3 }, // 'z'
    {   166,   3,   5,   5,    0,   -5 }, // '{'
    {   168,   1,   5,   3,    1,   -4 }, // '|'
    {   169,   3,   5,   5,    0,   -5 } // '}'
};

const GFXfont DialogBold5pt PROGMEM = {
(uint8_t  *)Dialog_bold_5Bitmaps,(GFXglyph *)Dialog_bold_5Glyphs,0x20, 0x7E, 7};