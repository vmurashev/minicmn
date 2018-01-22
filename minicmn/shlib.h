#pragma once

#include <minicmn/data_model.h>

typedef struct _minicmn_SharedLibrary
{
  void* Reserved0;
  void* Reserved1;
} minicmn_SharedLibrary;

int minicmn_LoadLibrary(const char* libpath, minicmn_SharedLibrary** shlib, minicmn_Error** err);
void* minicmn_GetProcAddress(minicmn_SharedLibrary* shlib, const char* procname);
