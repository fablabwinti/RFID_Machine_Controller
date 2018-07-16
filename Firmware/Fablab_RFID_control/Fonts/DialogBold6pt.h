// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Dialog_bold_6Bitmaps[] PROGMEM = {

  // Bitmap Data:
  0x00, // ' '
  0xD0, // '!'
  0xB4, // '"'
  0xF6,0xFA, // '#'
  0x4F,0xC7,0xF4, // '$'
  0xCB,0x42,0xD3, // '%'
  0x77,0x7C,0xF0, // '&'
  0xC0, // '''
  0x6A,0x40, // '('
  0x95,0x80, // ')'
  0xEB,0x80, // '*'
  0x5D,0x00, // '+'
  0xC0, // ','
  0xC0, // '-'
  0x80, // '.'
  0x5A,0x80, // '/'
  0xF6,0xF0, // '0'
  0xC9,0x70, // '1'
  0xE5,0xF0, // '2'
  0xF7,0x1F, // '3'
  0x66,0xF2, // '4'
  0xFF,0x1F, // '5'
  0x7E,0xF0, // '6'
  0xED,0x20, // '7'
  0xEA,0xF0, // '8'
  0xF7,0xE0, // '9'
  0xA0, // ':'
  0xB0, // ';'
  0x3C,0x70, // '<'
  0xF0,0xF0, // '='
  0xC3,0xE0, // '>'
  0xEC,0x20, // '?'
  0x77,0xF7,0xF7,0x00, // '@'
  0x23,0x95,0xF0, // 'A'
  0xFF,0x9F, // 'B'
  0x78,0x87, // 'C'
  0xE9,0x9E, // 'D'
  0xFE,0x70, // 'E'
  0xFE,0x40, // 'F'
  0x78,0xB7, // 'G'
  0x9F,0x99, // 'H'
  0xF0, // 'I'
  0x55,0xC0, // 'J'
  0xAC,0xCA, // 'K'
  0x92,0x70, // 'L'
  0xDE,0xEB,0x10, // 'M'
  0xDD,0xBB, // 'N'
  0x69,0x96, // 'O'
  0xF9,0xF8, // 'P'
  0x69,0x96,0x20, // 'Q'
  0xE0,0x00,0x00,0x50,0x00,0x00,0x30,0x00,0x00,0x1F,0xFF,0xFF,0xC0, // 'R'
  0xF1,0xF0, // 'S'
  0xE9,0x20, // 'T'
  0x99,0x9F, // 'U'
  0xDB,0x9C,0x40, // 'V'
  0xFF,0xF7,0x9E, // 'W'
  0x0F,0x66,0xF0, // 'X'
  0xFB,0x88,0x40, // 'Y'
  0xF6,0xEF, // 'Z'
  0xEA,0xC0, // '['
  0xA9,0x40, // '\'
  0xD5,0xC0, // ']'
  0x69, // '^'
  0xE0, // '_'
  0x40, // '`'
  0xFF,0x80, // 'a'
  0x88,0xF9,0xF0, // 'b'
  0xF3,0x80, // 'c'
  0x11,0xF9,0xF0, // 'd'
  0xFF,0x80, // 'e'
  0x6B,0xA4, // 'f'
  0xF9,0xFF, // 'g'
  0x93,0xDA, // 'h'
  0xB8, // 'i'
  0x45,0x70, // 'j'
  0x88,0xAC,0xA0, // 'k'
  0xF8, // 'l'
  0xFD,0x6A, // 'm'
  0xF6,0x80, // 'n'
  0xF7,0x80, // 'o'
  0xF9,0xF8, // 'p'
  0xF9,0xF1, // 'q'
  0xE8, // 'r'
  0xFF,0x80, // 's'
  0x5D,0x30, // 't'
  0xB7,0x80, // 'u'
  0xF6,0x60, // 'v'
  0xFF,0xDC,0x00, // 'w'
  0x0F,0x6F,0x00, // 'x'
  0x03,0xCC,0xE2,0x00, // 'y'
  0xE6,0xE0, // 'z'
  0x69,0x64,0xC0, // '{'
  0xFC, // '|'
  0xC9,0x35,0x80 // '}'
};
const GFXglyph Dialog_bold_6Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
    {     0,   1,   1,   3,    0,    0 }, // ' '
    {     1,   1,   4,   4,    1,   -4 }, // '!'
    {     2,   3,   2,   4,    1,   -4 }, // '"'
    {     3,   4,   4,   6,    0,   -4 }, // '#'
    {     5,   4,   6,   5,    0,   -5 }, // '$'
    {     8,   6,   4,   7,    1,   -4 }, // '%'
    {    11,   5,   4,   6,    1,   -4 }, // '&'
    {    14,   1,   2,   3,    1,   -4 }, // '''
    {    15,   2,   5,   4,    1,   -5 }, // '('
    {    17,   2,   5,   4,    0,   -5 }, // ')'
    {    19,   3,   3,   4,    0,   -4 }, // '*'
    {    21,   3,   3,   6,    1,   -3 }, // '+'
    {    23,   1,   2,   3,    1,   -1 }, // ','
    {    24,   2,   1,   3,    0,   -2 }, // '-'
    {    25,   1,   1,   3,    1,   -1 }, // '.'
    {    26,   2,   5,   3,    0,   -4 }, // '/'
    {    28,   3,   4,   5,    1,   -4 }, // '0'
    {    30,   3,   4,   5,    1,   -4 }, // '1'
    {    32,   3,   4,   5,    1,   -4 }, // '2'
    {    34,   4,   4,   5,    1,   -4 }, // '3'
    {    36,   4,   4,   5,    1,   -4 }, // '4'
    {    38,   4,   4,   5,    1,   -4 }, // '5'
    {    40,   3,   4,   5,    1,   -4 }, // '6'
    {    42,   3,   4,   5,    1,   -4 }, // '7'
    {    44,   3,   4,   5,    1,   -4 }, // '8'
    {    46,   3,   4,   5,    1,   -4 }, // '9'
    {    48,   1,   3,   3,    1,   -3 }, // ':'
    {    49,   1,   4,   3,    1,   -3 }, // ';'
    {    50,   4,   3,   6,    1,   -4 }, // '<'
    {    52,   4,   3,   6,    1,   -4 }, // '='
    {    54,   4,   3,   6,    1,   -4 }, // '>'
    {    56,   3,   4,   4,    0,   -4 }, // '?'
    {    58,   5,   5,   7,    0,   -4 }, // '@'
    {    62,   5,   4,   6,    0,   -4 }, // 'A'
    {    65,   4,   4,   6,    1,   -4 }, // 'B'
    {    67,   4,   4,   5,    1,   -4 }, // 'C'
    {    69,   4,   4,   6,    1,   -4 }, // 'D'
    {    71,   3,   4,   5,    1,   -4 }, // 'E'
    {    73,   3,   4,   5,    1,   -4 }, // 'F'
    {    75,   4,   4,   6,    1,   -4 }, // 'G'
    {    77,   4,   4,   6,    1,   -4 }, // 'H'
    {    79,   1,   4,   3,    1,   -4 }, // 'I'
    {    80,   2,   5,   3,    0,   -4 }, // 'J'
    {    82,   4,   4,   6,    1,   -4 }, // 'K'
    {    84,   3,   4,   5,    1,   -4 }, // 'L'
    {    86,   5,   4,   7,    1,   -4 }, // 'M'
    {    89,   4,   4,   6,    1,   -4 }, // 'N'
    {    91,   4,   4,   6,    1,   -4 }, // 'O'
    {    93,   4,   4,   5,    1,   -4 }, // 'P'
    {    95,   4,   5,   6,    1,   -4 }, // 'Q'
    {    98,  25,   4,   6,    1,   -4 }, // 'R'
    {   111,   3,   4,   5,    1,   -4 }, // 'S'
    {   113,   3,   4,   5,    0,   -4 }, // 'T'
    {   115,   4,   4,   6,    1,   -4 }, // 'U'
    {   117,   5,   4,   6,    0,   -4 }, // 'V'
    {   120,   6,   4,   8,    0,   -4 }, // 'W'
    {   123,   4,   6,   6,    0,   -5 }, // 'X'
    {   126,   5,   4,   5,    0,   -4 }, // 'Y'
    {   129,   4,   4,   5,    0,   -4 }, // 'Z'
    {   131,   2,   5,   4,    1,   -5 }, // '['
    {   133,   2,   5,   3,    0,   -4 }, // '\'
    {   135,   2,   5,   4,    0,   -5 }, // ']'
    {   137,   4,   2,   6,    1,   -4 }, // '^'
    {   138,   3,   1,   4,    0,    0 }, // '_'
    {   139,   2,   1,   4,    0,   -5 }, // '`'
    {   140,   3,   3,   5,    1,   -3 }, // 'a'
    {   142,   4,   5,   5,    1,   -5 }, // 'b'
    {   145,   3,   3,   5,    1,   -3 }, // 'c'
    {   147,   4,   5,   5,    1,   -5 }, // 'd'
    {   150,   3,   3,   5,    1,   -3 }, // 'e'
    {   152,   3,   5,   4,    0,   -5 }, // 'f'
    {   154,   4,   4,   5,    1,   -3 }, // 'g'
    {   156,   3,   5,   5,    1,   -5 }, // 'h'
    {   158,   1,   5,   3,    1,   -5 }, // 'i'
    {   159,   2,   6,   3,    0,   -5 }, // 'j'
    {   161,   4,   5,   5,    1,   -5 }, // 'k'
    {   164,   1,   5,   3,    1,   -5 }, // 'l'
    {   165,   5,   3,   7,    1,   -3 }, // 'm'
    {   167,   3,   3,   5,    1,   -3 }, // 'n'
    {   169,   3,   3,   5,    1,   -3 }, // 'o'
    {   171,   4,   4,   5,    1,   -3 }, // 'p'
    {   173,   4,   4,   5,    1,   -3 }, // 'q'
    {   175,   2,   3,   4,    1,   -3 }, // 'r'
    {   176,   3,   3,   5,    1,   -3 }, // 's'
    {   178,   3,   4,   4,    0,   -4 }, // 't'
    {   180,   3,   3,   5,    1,   -3 }, // 'u'
    {   182,   4,   3,   5,    0,   -3 }, // 'v'
    {   184,   5,   4,   7,    0,   -3 }, // 'w'
    {   187,   4,   5,   5,    0,   -4 }, // 'x'
    {   190,   5,   5,   5,   -1,   -4 }, // 'y'
    {   194,   4,   3,   5,    0,   -3 }, // 'z'
    {   196,   3,   6,   5,    1,   -5 }, // '{'
    {   199,   1,   6,   3,    1,   -5 }, // '|'
    {   200,   3,   6,   5,    1,   -5 } // '}'
};
const GFXfont DialogBold6pt PROGMEM = {
(uint8_t  *)Dialog_bold_6Bitmaps,(GFXglyph *)Dialog_bold_6Glyphs,0x20, 0x7E, 8};