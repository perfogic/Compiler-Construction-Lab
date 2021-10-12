/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __CHARCODE_H__
#define __CHARCODE_H__

// ĐỊnh nghĩa char_code để nhận biết ví dụ CHAR_SPACE là dấu cách, CHAR_LETTER là chữ cái
typedef enum
{
  CHAR_SPACE,
  CHAR_LETTER,
  CHAR_DIGIT,
  CHAR_BACKSLASH,
  CHAR_PLUS,
  CHAR_DOLLAR,
  CHAR_MINUS,
  CHAR_TIMES,
  CHAR_SLASH,
  CHAR_LT,
  CHAR_GT,
  CHAR_EXCLAIMATION,
  CHAR_EQ,
  CHAR_COMMA,
  CHAR_PERIOD,
  CHAR_COLON,
  CHAR_SEMICOLON,
  CHAR_SINGLEQUOTE,
  CHAR_LPAR,
  CHAR_UNDERSCORE,
  CHAR_RPAR,
  CHAR_UNKNOWN
} CharCode;

#endif
