/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include "reader.h"

FILE *inputStream;
int lineNo, colNo;
int currentChar;

// Đọc từng chữ một
int readChar(void)
{
  currentChar = getc(inputStream);
  colNo++;
  if (currentChar == '\n')
  {
    lineNo++;
    colNo = 0;
  }
  return currentChar;
}

// Mở file input
int openInputStream(char *fileName)
{
  inputStream = fopen(fileName, "rt");
  if (inputStream == NULL)
    return IO_ERROR;
  lineNo = 1;
  colNo = 0;
  readChar();
  return IO_SUCCESS;
}

// Đóng file input
void closeInputStream()
{
  fclose(inputStream);
}
