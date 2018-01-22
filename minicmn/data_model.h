#pragma once

#include <stdint.h>

typedef struct _minicmn_Error
{
  void* Reserved0;
  void* Reserved1;
  int32_t ErrorCode;
  const char* CodeFile;
  uint32_t CodeLine;
  const char* ErrorText;
} minicmn_Error;

void minicmn_DataFree(void* ptr);

minicmn_Error* minicmn_makeWindowsError(int32_t err_code, const char* code_file, uint32_t code_line);

minicmn_Error* minicmn_makePosixError(int32_t err_code, const char* code_file, uint32_t code_line);

minicmn_Error* minicmn_makeLogicError(const char* err_text, const char* code_file, uint32_t code_line);
