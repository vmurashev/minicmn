#pragma once

#include <minicmn/data_model.h>
#include <minicmn/sds.h>

int minicmn_GetFullPathOfExecutable(sds* exe_path, minicmn_Error** err);
sds minicmn_GetDirectoryName(const char* path);
sds minicmn_OsPathJoin(const char* path1, const char* path2);
