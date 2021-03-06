// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Dialog_bold_20Bitmaps[] PROGMEM = {

  // Bitmap Data:
  0x00, // ' '
  0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xFF,0xF0, // '!'
  0xCF,0x3C,0xF3,0xCC, // '"'
  0x06,0x30,0x18,0xC0,0x63,0x01,0x98,0x7F,0xFD,0xFF,0xF0,0xCE,0x03,0x30,0x18,0xC3,0xFF,0xEF,0xFF,0x86,0x60,0x39,0x80,0xC6,0x03,0x18,0x00, // '#'
  0x06,0x00,0x60,0x1F,0xC7,0xFE,0xE6,0x2E,0x60,0xF6,0x0F,0xF8,0x7F,0xE1,0xFF,0x06,0xF0,0x67,0x86,0x7F,0xFE,0x3F,0xC0,0x60,0x06,0x00,0x60, // '$'
  0x3C,0x06,0x0F,0xC1,0x83,0x9C,0x70,0x61,0x8C,0x0C,0x33,0x81,0xCE,0x60,0x1F,0x98,0x01,0xE7,0x3C,0x00,0xCF,0xC0,0x33,0x9C,0x0E,0x61,0x81,0x8C,0x30,0x71,0xCE,0x0C,0x1F,0x83,0x01,0xE0, // '%'
  0x0F,0x80,0x3F,0x80,0xF1,0x01,0xE0,0x03,0xC0,0x03,0xC0,0x0F,0xC7,0x3F,0x8E,0xF3,0x9D,0xE7,0xF3,0xC7,0xE7,0x87,0x87,0x8F,0x0F,0xFF,0x07,0xEF,0x00, // '&'
  0xFF,0xC0, // '''
  0x3C,0xE7,0x9C,0x73,0x8E,0x38,0xE3,0x8E,0x38,0xE1,0xC7,0x1E,0x38,0xF0, // '('
  0xF1,0xC7,0x8E,0x38,0x71,0xC7,0x1C,0x71,0xC7,0x1C,0xE3,0x9E,0x73,0xC0, // ')'
  0x0C,0x03,0x0C,0xCD,0xFE,0x1E,0x1F,0xEC,0xCC,0x30,0x0C,0x00, // '*'
  0x06,0x00,0x60,0x06,0x00,0x60,0x06,0x0F,0xFF,0xFF,0xF0,0x60,0x06,0x00,0x60,0x06,0x00,0x60, // '+'
  0x7B,0xDE,0xF7,0x73,0x00, // ','
  0xFF,0xFF,0xC0, // '-'
  0xFF,0xFF, // '.'
  0x06,0x1C,0x30,0x60,0xC3,0x06,0x0C,0x30,0x60,0xC3,0x06,0x0C,0x38,0x60, // '/'
  0x1F,0x83,0xFC,0x79,0xE7,0x0E,0xF0,0xFF,0x0F,0xF0,0xFF,0x0F,0xF0,0xFF,0x0F,0xF0,0xF7,0x0E,0x79,0xE3,0xFC,0x1F,0x80, // '0'
  0x7E,0x3F,0x8D,0xE0,0x78,0x1E,0x07,0x81,0xE0,0x78,0x1E,0x07,0x81,0xE0,0x78,0x1E,0x3F,0xFF,0xFC, // '1'
  0x3F,0x1F,0xFB,0x0F,0xC0,0xF0,0x1E,0x03,0xC0,0xF8,0x1F,0x07,0xC1,0xF8,0x7C,0x1F,0x07,0xC1,0xFF,0xFF,0xF8, // '2'
  0x7F,0x9F,0xFA,0x0F,0x80,0xF0,0x1E,0x07,0x87,0xE0,0xFE,0x03,0xE0,0x3C,0x07,0x80,0xF8,0x3F,0xFF,0x9F,0xC0, // '3'
  0x07,0xC0,0x7C,0x0F,0xC1,0xFC,0x1B,0xC3,0xBC,0x73,0xC6,0x3C,0xE3,0xCC,0x3C,0xFF,0xFF,0xFF,0x03,0xC0,0x3C,0x03,0xC0, // '4'
  0x7F,0xCF,0xF9,0xC0,0x38,0x07,0x00,0xFE,0x1F,0xF2,0x1E,0x01,0xE0,0x3C,0x07,0x80,0xF8,0x3D,0xFF,0x9F,0xC0, // '5'
  0x0F,0xC3,0xFE,0x3C,0x27,0x80,0xF0,0x0F,0x78,0xFF,0xEF,0x9F,0xF0,0xFF,0x0F,0xF0,0xF7,0x0F,0x79,0xE3,0xFC,0x1F,0x80, // '6'
  0xFF,0xFF,0xFC,0x0F,0x81,0xE0,0x3C,0x0F,0x81,0xE0,0x7C,0x0F,0x03,0xE0,0x78,0x1F,0x03,0xC0,0x78,0x1F,0x00, // '7'
  0x3F,0xC7,0xFE,0xF9,0xFF,0x0F,0xF0,0xF7,0x9E,0x3F,0xC3,0xFC,0x79,0xEF,0x0F,0xF0,0xFF,0x0F,0xF9,0xF7,0xFE,0x1F,0x80, // '8'
  0x1F,0x03,0xFC,0x79,0xEF,0x0E,0xF0,0xFF,0x0F,0xF0,0xFF,0x9F,0x7F,0xF1,0xEF,0x00,0xF0,0x1E,0x43,0xC7,0xFC,0x3F,0x00, // '9'
  0xFF,0xFF,0x00,0x0F,0xFF,0xF0, // ':'
  0x7B,0xDE,0xF0,0x00,0x0F,0x7B,0xDE,0xEE,0x60, // ';'
  0x00,0x08,0x03,0xC0,0xFE,0x3F,0x0F,0xC0,0x70,0x03,0xF0,0x03,0xF0,0x03,0xF8,0x03,0xC0,0x02, // '<'
  0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x0F,0xFF,0xFF,0xFC, // '='
  0x80,0x07,0x80,0x3F,0x80,0x1F,0x80,0x1F,0x80,0x1C,0x07,0xE1,0xF8,0xFE,0x07,0x80,0x20,0x00, // '>'
  0x7E,0x7F,0xA1,0xE0,0xF0,0x78,0x7C,0x7C,0x7C,0x3C,0x1E,0x00,0x07,0x83,0xC1,0xE0,0xF0, // '?'
  0x03,0xF0,0x07,0xFE,0x07,0x07,0x87,0x00,0xE7,0x3D,0xB3,0x3F,0xDF,0x38,0xE7,0x98,0x33,0xCC,0x19,0xE6,0x0C,0xF3,0x06,0x79,0xC7,0x66,0x7F,0xE3,0x9E,0xE0,0xE0,0x10,0x38,0x38,0x0F,0xF8,0x01,0xF0,0x00, // '@'
  0x07,0xC0,0x0F,0x80,0x3F,0x80,0x7F,0x00,0xEE,0x03,0xDE,0x07,0xBC,0x0E,0x38,0x3C,0x78,0x78,0xF0,0xFF,0xE3,0xFF,0xE7,0x83,0xCF,0x07,0xBC,0x07,0x80, // 'A'
  0xFF,0x8F,0xFC,0xF1,0xEF,0x1E,0xF1,0xEF,0x1E,0xFF,0xCF,0xFE,0xF1,0xEF,0x0F,0xF0,0xFF,0x0F,0xF1,0xFF,0xFE,0xFF,0x80, // 'B'
  0x0F,0xE1,0xFF,0x3C,0x17,0x80,0xF8,0x0F,0x00,0xF0,0x0F,0x00,0xF0,0x0F,0x00,0xF8,0x07,0x80,0x3C,0x11,0xFF,0x0F,0xE0, // 'C'
  0xFF,0x83,0xFF,0x8F,0x0F,0x3C,0x1E,0xF0,0x7F,0xC0,0xFF,0x03,0xFC,0x0F,0xF0,0x3F,0xC0,0xFF,0x07,0xFC,0x1E,0xF0,0xF3,0xFF,0x8F,0xF8,0x00, // 'D'
  0xFF,0xFF,0xFF,0x03,0xC0,0xF0,0x3C,0x0F,0xFB,0xFE,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3F,0xFF,0xFC, // 'E'
  0xFF,0xFF,0xFF,0x03,0xC0,0xF0,0x3C,0x0F,0xFF,0xFF,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x00, // 'F'
  0x07,0xF0,0x7F,0xE3,0xC0,0x9E,0x00,0xF8,0x03,0xC0,0x0F,0x00,0x3C,0x3F,0xF0,0xFF,0xC0,0xFF,0x03,0xDE,0x0F,0x3C,0x3C,0x7F,0xF0,0x7F,0x00, // 'G'
  0xF0,0x7F,0x83,0xFC,0x1F,0xE0,0xFF,0x07,0xF8,0x3F,0xFF,0xFF,0xFF,0xF0,0x7F,0x83,0xFC,0x1F,0xE0,0xFF,0x07,0xF8,0x3F,0xC1,0xE0, // 'H'
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0, // 'I'
  0x1E,0x3C,0x78,0xF1,0xE3,0xC7,0x8F,0x1E,0x3C,0x78,0xF1,0xE3,0xC7,0x8F,0x3F,0xFB,0xC0, // 'J'
  0xF0,0x7B,0xC3,0xCF,0x1E,0x3C,0xF0,0xF7,0x83,0xFC,0x0F,0xE0,0x3F,0x80,0xFF,0x03,0xFE,0x0F,0x7C,0x3C,0xF8,0xF1,0xF3,0xC3,0xEF,0x07,0xC0, // 'K'
  0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3F,0xFF,0xFC, // 'L'
  0xF8,0x1F,0xF8,0x1F,0xFC,0x3F,0xFC,0x3F,0xFC,0x3F,0xFE,0x7F,0xF6,0x6F,0xF6,0x6F,0xF7,0xEF,0xF3,0xCF,0xF3,0xCF,0xF1,0x8F,0xF0,0x0F,0xF0,0x0F,0xF0,0x0F, // 'M'
  0xF8,0x7F,0xC3,0xFF,0x1F,0xF8,0xFF,0xE7,0xFB,0x3F,0xD9,0xFE,0xEF,0xF3,0x7F,0x9B,0xFC,0xFF,0xE3,0xFF,0x1F,0xF8,0x7F,0xC3,0xE0, // 'N'
  0x0F,0xE0,0x3F,0xE0,0xF1,0xE3,0xC1,0xEF,0x01,0xFE,0x03,0xFC,0x07,0xF8,0x0F,0xF0,0x1F,0xE0,0x3F,0xC0,0x7B,0xC1,0xE3,0xC7,0x83,0xFE,0x03,0xF8,0x00, // 'O'
  0xFF,0x8F,0xFE,0xF1,0xEF,0x0F,0xF0,0xFF,0x0F,0xF0,0xFF,0x1E,0xFF,0xEF,0xF8,0xF0,0x0F,0x00,0xF0,0x0F,0x00,0xF0,0x00, // 'P'
  0x0F,0xE0,0x3F,0xE0,0xF1,0xE3,0xC1,0xEF,0x01,0xFE,0x03,0xFC,0x07,0xF8,0x0F,0xF0,0x1F,0xE0,0x3F,0xC0,0x7B,0xC1,0xE3,0xC7,0x83,0xFE,0x03,0xF8,0x00,0x78,0x00,0x78,0x00,0x70, // 'Q'
  0xFF,0x87,0xFE,0x3C,0x79,0xE3,0xCF,0x1E,0x78,0xF3,0xC7,0x1F,0xF0,0xFF,0x87,0x9E,0x3C,0xF9,0xE3,0xCF,0x1F,0x78,0x7B,0xC3,0xE0, // 'R'
  0x1F,0xE7,0xFE,0xF8,0x6F,0x02,0xF0,0x0F,0x80,0xFF,0x87,0xFE,0x1F,0xF0,0x1F,0x00,0xF8,0x0F,0xE1,0xFF,0xFE,0x7F,0x80, // 'S'
  0xFF,0xFF,0xFF,0xF0,0x78,0x01,0xE0,0x07,0x80,0x1E,0x00,0x78,0x01,0xE0,0x07,0x80,0x1E,0x00,0x78,0x01,0xE0,0x07,0x80,0x1E,0x00,0x78,0x00, // 'T'
  0xF0,0x7F,0x83,0xFC,0x1F,0xE0,0xFF,0x07,0xF8,0x3F,0xC1,0xFE,0x0F,0xF0,0x7F,0x83,0xFC,0x1F,0xE0,0xF7,0x8F,0x3F,0xF8,0x7F,0x00, // 'U'
  0xF0,0x1E,0xF0,0x79,0xE0,0xF3,0xC1,0xE3,0xC7,0x87,0x8F,0x0F,0x1E,0x0F,0x78,0x1E,0xF0,0x3D,0xE0,0x3F,0x80,0x7F,0x00,0xFE,0x00,0xF8,0x01,0xF0,0x00, // 'V'
  0xF0,0x70,0x7F,0x83,0x83,0xDE,0x3E,0x3C,0xF1,0xF1,0xE7,0x8D,0x8F,0x1C,0xEE,0x70,0xF7,0x77,0x87,0xB1,0xBC,0x3D,0x8D,0xE0,0xFC,0x7E,0x07,0xE3,0xF0,0x3E,0x0F,0x80,0xF0,0x78,0x07,0x83,0xC0,0x38,0x0E,0x00, // 'W'
  0xF8,0x3E,0xF0,0x78,0xF1,0xE1,0xF7,0xC1,0xEF,0x01,0xFC,0x01,0xF0,0x03,0xE0,0x0F,0xE0,0x1F,0xC0,0x7B,0xC1,0xF7,0xC3,0xC7,0x8F,0x07,0xBE,0x0F,0x80, // 'X'
  0xF8,0x1F,0x7C,0x3E,0x3C,0x3C,0x3E,0x7C,0x1E,0x78,0x0F,0xF0,0x0F,0xF0,0x07,0xE0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0, // 'Y'
  0xFF,0xFF,0xFF,0xC0,0x3E,0x03,0xE0,0x3E,0x01,0xF0,0x1F,0x01,0xF0,0x1F,0x01,0xF0,0x0F,0x80,0xF8,0x0F,0x80,0x7F,0xFF,0xFF,0xE0, // 'Z'
  0xFF,0xFE,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xFF,0xF0, // '['
  0xC1,0xC1,0x83,0x06,0x06,0x0C,0x18,0x18,0x30,0x60,0x60,0xC1,0x83,0x83, // '\'
  0xFF,0xF1,0xC7,0x1C,0x71,0xC7,0x1C,0x71,0xC7,0x1C,0x71,0xC7,0xFF,0xF0, // ']'
  0x07,0x00,0x7C,0x07,0x70,0x71,0xC6,0x03,0x00, // '^'
  0xFF,0xFF,0xF0, // '_'
  0xE1,0xC3,0x87, // '`'
  0x3F,0x8F,0xF9,0x07,0x80,0xF3,0xFE,0xFF,0xFC,0x7F,0x8F,0xF3,0xEF,0xFC,0xF7,0x80, // 'a'
  0xF0,0x0F,0x00,0xF0,0x0F,0x00,0xF3,0xCF,0xFE,0xF9,0xEF,0x0F,0xF0,0xFF,0x0F,0xF0,0xFF,0x0F,0xF9,0xEF,0xFE,0xF3,0xC0, // 'b'
  0x1F,0x8F,0xF7,0x87,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0x78,0x4F,0xF1,0xF8, // 'c'
  0x00,0xF0,0x0F,0x00,0xF0,0x0F,0x3C,0xF7,0xFF,0x79,0xFF,0x0F,0xF0,0xFF,0x0F,0xF0,0xFF,0x0F,0x79,0xF7,0xFF,0x3C,0xF0, // 'd'
  0x1F,0x83,0xFE,0x79,0xEF,0x0F,0xFF,0xFF,0xFF,0xF0,0x0F,0x00,0x78,0x23,0xFE,0x1F,0xC0, // 'e'
  0x0F,0x9F,0xCF,0x07,0x8F,0xFF,0xFC,0xF0,0x78,0x3C,0x1E,0x0F,0x07,0x83,0xC1,0xE0,0xF0, // 'f'
  0x3C,0xF7,0xFF,0x79,0xFF,0x0F,0xF0,0xFF,0x0F,0xF0,0xFF,0x0F,0x79,0xF7,0xFF,0x1C,0xF0,0x0F,0x41,0xE7,0xFC,0x3F,0x80, // 'g'
  0xF0,0x1E,0x03,0xC0,0x78,0x0F,0x39,0xFF,0xBE,0x7F,0x8F,0xF1,0xFE,0x3F,0xC7,0xF8,0xFF,0x1F,0xE3,0xFC,0x78, // 'h'
  0xFF,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0, // 'i'
  0x3C,0xF3,0xC0,0x3C,0xF3,0xCF,0x3C,0xF3,0xCF,0x3C,0xF3,0xCF,0x3F,0xEF,0x00, // 'j'
  0xF0,0x0F,0x00,0xF0,0x0F,0x00,0xF0,0xEF,0x1C,0xF3,0x8F,0x70,0xFE,0x0F,0xE0,0xFF,0x0F,0x78,0xF3,0xCF,0x1E,0xF0,0xF0, // 'k'
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0, // 'l'
  0xF7,0x8F,0x3F,0xF7,0xEF,0x9F,0x3F,0xC7,0x8F,0xF1,0xE3,0xFC,0x78,0xFF,0x1E,0x3F,0xC7,0x8F,0xF1,0xE3,0xFC,0x78,0xFF,0x1E,0x3C, // 'm'
  0xF3,0x9F,0xFB,0xE7,0xF8,0xFF,0x1F,0xE3,0xFC,0x7F,0x8F,0xF1,0xFE,0x3F,0xC7,0x80, // 'n'
  0x1F,0x83,0xFC,0x79,0xEF,0x0F,0xF0,0xFF,0x0F,0xF0,0xFF,0x0F,0x79,0xE3,0xFC,0x1F,0x80, // 'o'
  0xF3,0xCF,0xFE,0xF9,0xEF,0x0F,0xF0,0xFF,0x0F,0xF0,0xFF,0x0F,0xF9,0xEF,0xFE,0xF3,0xCF,0x00,0xF0,0x0F,0x00,0xF0,0x00, // 'p'
  0x3C,0xF7,0xFF,0x79,0xFF,0x0F,0xF0,0xFF,0x0F,0xF0,0xFF,0x0F,0x79,0xF7,0xFF,0x3C,0xF0,0x0F,0x00,0xF0,0x0F,0x00,0xF0, // 'q'
  0xF7,0xFF,0xF8,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0, // 'r'
  0x3F,0x9F,0xFF,0x07,0xC0,0xFC,0x1F,0xE1,0xFC,0x0F,0x83,0xFF,0xE7,0xF0, // 's'
  0x3C,0x0F,0x03,0xC3,0xFF,0xFF,0xCF,0x03,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3F,0x87,0xE0, // 't'
  0xF1,0xFE,0x3F,0xC7,0xF8,0xFF,0x1F,0xE3,0xFC,0x7F,0x8F,0xF3,0xEF,0xFC,0xE7,0x80, // 'u'
  0xF0,0xF7,0x0E,0x79,0xE7,0x9E,0x39,0xC3,0x9C,0x3F,0xC1,0xF8,0x1F,0x80,0xF0,0x0F,0x00, // 'v'
  0xF1,0xC7,0xF8,0xE3,0xDC,0x71,0xCF,0x7D,0xE7,0xB6,0xF1,0xDB,0x70,0xED,0xB8,0x7C,0x7C,0x3E,0x3E,0x0F,0x1E,0x07,0x8F,0x00, // 'w'
  0xF0,0xF7,0x9E,0x39,0xC1,0xF8,0x1F,0x80,0xF0,0x1F,0x83,0xFC,0x39,0xC7,0x9E,0xF0,0xF0, // 'x'
  0xF0,0xF7,0x0F,0x78,0xE7,0x9E,0x39,0xC3,0x9C,0x1D,0xC1,0xF8,0x1F,0x80,0xF8,0x0F,0x00,0xF0,0x1E,0x07,0xE0,0x7C,0x00, // 'y'
  0xFF,0xFF,0xF0,0x3C,0x1E,0x0F,0x07,0x83,0xC1,0xE0,0xF0,0x3F,0xFF,0xFC, // 'z'
  0x0F,0x8F,0xC7,0x03,0x81,0xC0,0xE0,0x70,0x78,0xF8,0x7C,0x0F,0x03,0x81,0xC0,0xE0,0x70,0x38,0x1F,0x87,0xC0, // '{'
  0xFF,0xFF,0xFF,0xFF,0xFF, // '|'
  0xF8,0x7E,0x07,0x03,0x81,0xC0,0xE0,0x70,0x3C,0x0F,0x87,0xC7,0x83,0x81,0xC0,0xE0,0x70,0x38,0xFC,0x7C,0x00 // '}'
};
const GFXglyph Dialog_bold_20Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
    {     0,   1,   1,   8,    0,    0 }, // ' '
    {     1,   4,  15,  10,    3,  -15 }, // '!'
    {     9,   6,   5,  11,    2,  -15 }, // '"'
    {    13,  14,  15,  18,    1,  -15 }, // '#'
    {    40,  12,  18,  15,    1,  -15 }, // '$'
    {    67,  19,  15,  21,    1,  -15 }, // '%'
    {   103,  15,  15,  18,    1,  -15 }, // '&'
    {   132,   2,   5,   7,    2,  -15 }, // '''
    {   134,   6,  18,  10,    2,  -15 }, // '('
    {   148,   6,  18,  10,    1,  -15 }, // ')'
    {   162,  10,   9,  11,    0,  -15 }, // '*'
    {   174,  12,  12,  18,    2,  -12 }, // '+'
    {   192,   5,   7,   9,    1,   -4 }, // ','
    {   197,   6,   3,   9,    1,   -7 }, // '-'
    {   200,   4,   4,   9,    2,   -4 }, // '.'
    {   202,   7,  16,   8,    0,  -15 }, // '/'
    {   216,  12,  15,  15,    1,  -15 }, // '0'
    {   239,  10,  15,  15,    3,  -15 }, // '1'
    {   258,  11,  15,  15,    2,  -15 }, // '2'
    {   279,  11,  15,  15,    1,  -15 }, // '3'
    {   300,  12,  15,  15,    1,  -15 }, // '4'
    {   323,  11,  15,  15,    2,  -15 }, // '5'
    {   344,  12,  15,  15,    1,  -15 }, // '6'
    {   367,  11,  15,  15,    1,  -15 }, // '7'
    {   388,  12,  15,  15,    1,  -15 }, // '8'
    {   411,  12,  15,  15,    1,  -15 }, // '9'
    {   434,   4,  11,   9,    2,  -11 }, // ':'
    {   440,   5,  14,   9,    1,  -11 }, // ';'
    {   449,  13,  11,  18,    2,  -12 }, // '<'
    {   467,  13,   6,  18,    2,   -9 }, // '='
    {   477,  13,  11,  18,    2,  -12 }, // '>'
    {   495,   9,  15,  13,    1,  -15 }, // '?'
    {   512,  17,  18,  21,    1,  -15 }, // '@'
    {   551,  15,  15,  16,    0,  -15 }, // 'A'
    {   580,  12,  15,  16,    2,  -15 }, // 'B'
    {   603,  12,  15,  16,    1,  -15 }, // 'C'
    {   626,  14,  15,  18,    2,  -15 }, // 'D'
    {   653,  10,  15,  15,    2,  -15 }, // 'E'
    {   672,  10,  15,  15,    2,  -15 }, // 'F'
    {   691,  14,  15,  17,    1,  -15 }, // 'G'
    {   718,  13,  15,  18,    2,  -15 }, // 'H'
    {   743,   4,  15,   8,    2,  -15 }, // 'I'
    {   751,   7,  19,   8,   -1,  -15 }, // 'J'
    {   768,  14,  15,  17,    2,  -15 }, // 'K'
    {   795,  10,  15,  14,    2,  -15 }, // 'L'
    {   814,  16,  15,  21,    2,  -15 }, // 'M'
    {   844,  13,  15,  18,    2,  -15 }, // 'N'
    {   869,  15,  15,  18,    1,  -15 }, // 'O'
    {   898,  12,  15,  16,    2,  -15 }, // 'P'
    {   921,  15,  18,  18,    1,  -15 }, // 'Q'
    {   955,  13,  15,  16,    2,  -15 }, // 'R'
    {   980,  12,  15,  15,    1,  -15 }, // 'S'
    {  1003,  14,  15,  15,    0,  -15 }, // 'T'
    {  1030,  13,  15,  17,    2,  -15 }, // 'U'
    {  1055,  15,  15,  16,    0,  -15 }, // 'V'
    {  1084,  21,  15,  23,    1,  -15 }, // 'W'
    {  1124,  15,  15,  16,    0,  -15 }, // 'X'
    {  1153,  16,  15,  15,   -1,  -15 }, // 'Y'
    {  1183,  13,  15,  16,    1,  -15 }, // 'Z'
    {  1208,   6,  18,  10,    2,  -15 }, // '['
    {  1222,   7,  16,   8,    0,  -15 }, // '\'
    {  1236,   6,  18,  10,    1,  -15 }, // ']'
    {  1250,  13,   5,  18,    2,  -15 }, // '^'
    {  1259,  10,   2,  11,    0,    3 }, // '_'
    {  1262,   6,   4,  11,    1,  -16 }, // '`'
    {  1265,  11,  11,  15,    1,  -11 }, // 'a'
    {  1281,  12,  15,  15,    2,  -15 }, // 'b'
    {  1304,  10,  11,  13,    1,  -11 }, // 'c'
    {  1318,  12,  15,  15,    1,  -15 }, // 'd'
    {  1341,  12,  11,  15,    1,  -11 }, // 'e'
    {  1358,   9,  15,  10,    0,  -15 }, // 'f'
    {  1375,  12,  15,  15,    1,  -11 }, // 'g'
    {  1398,  11,  15,  15,    2,  -15 }, // 'h'
    {  1419,   4,  15,   8,    2,  -15 }, // 'i'
    {  1427,   6,  19,   8,    0,  -15 }, // 'j'
    {  1442,  12,  15,  14,    2,  -15 }, // 'k'
    {  1465,   4,  15,   8,    2,  -15 }, // 'l'
    {  1473,  18,  11,  22,    2,  -11 }, // 'm'
    {  1498,  11,  11,  15,    2,  -11 }, // 'n'
    {  1514,  12,  11,  15,    1,  -11 }, // 'o'
    {  1531,  12,  15,  15,    2,  -11 }, // 'p'
    {  1554,  12,  15,  15,    1,  -11 }, // 'q'
    {  1577,   8,  11,  11,    2,  -11 }, // 'r'
    {  1588,  10,  11,  13,    1,  -11 }, // 's'
    {  1602,  10,  14,  11,    0,  -14 }, // 't'
    {  1620,  11,  11,  15,    2,  -11 }, // 'u'
    {  1636,  12,  11,  14,    0,  -11 }, // 'v'
    {  1653,  17,  11,  19,    1,  -11 }, // 'w'
    {  1677,  12,  11,  14,    1,  -11 }, // 'x'
    {  1694,  12,  15,  14,    1,  -11 }, // 'y'
    {  1717,  10,  11,  13,    1,  -11 }, // 'z'
    {  1731,   9,  18,  15,    3,  -15 }, // '{'
    {  1752,   2,  20,   8,    3,  -15 }, // '|'
    {  1757,   9,  18,  15,    2,  -15 } // '}'
};
const GFXfont Dialogbold20 PROGMEM = {
(uint8_t  *)Dialog_bold_20Bitmaps,(GFXglyph *)Dialog_bold_20Glyphs,0x20, 0x7E, 24};