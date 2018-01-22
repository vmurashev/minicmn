#include <minicmn/os_path.h>
#include <windows.h>

int minicmn_GetFullPathOfExecutable(sds* exe_path, minicmn_Error** err)
{
    DWORD code;
    DWORD attr;
    DWORD size;
    DWORD reparsed_path_size;
    wchar_t* reparsed_path;
    wchar_t* path;
    DWORD path_inc_size;
    DWORD path_alloc_size;
    HANDLE fh_exe;
    DWORD (WINAPI* GetFinalPathNameByHandleW_Ptr)(HANDLE, LPWSTR, DWORD, DWORD);

    path_inc_size = MAX_PATH;
    path_alloc_size = path_inc_size;
    path = NULL;
    do
    {
        if (path != NULL)
        {
            free(path);
            path_alloc_size += path_inc_size;
        }
        path = (wchar_t*) malloc(sizeof(wchar_t) * path_alloc_size);
        path[0] = 0;
        size = GetModuleFileNameW(NULL, path, path_alloc_size);
        if (size == 0)
        {
            code = GetLastError();
            *err = minicmn_makeWindowsError(code, __FILE__, __LINE__);
            free(path);
            return -1;
        }
    }
    while (size == path_alloc_size);
    path[size] = 0;

    GetFinalPathNameByHandleW_Ptr = (DWORD (WINAPI *)(HANDLE, LPWSTR, DWORD, DWORD))GetProcAddress(GetModuleHandle("kernel32"), "GetFinalPathNameByHandleW");
    attr = GetFileAttributesW(path);
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        code = GetLastError();
        *err = minicmn_makeWindowsError(code, __FILE__, __LINE__);
        free(path);
        return -1;
    }

    if (GetFinalPathNameByHandleW_Ptr && (attr & FILE_ATTRIBUTE_REPARSE_POINT))
    {
        fh_exe = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (fh_exe == INVALID_HANDLE_VALUE)
        {
            code = GetLastError();
            *err = minicmn_makeWindowsError(code, __FILE__, __LINE__);
            free(path);
            return -1;
        }
        reparsed_path_size = MAX_PATH;
        reparsed_path = (wchar_t*)malloc(sizeof(wchar_t)*(reparsed_path_size+1));
        size = (*GetFinalPathNameByHandleW_Ptr)(fh_exe, reparsed_path, reparsed_path_size, 0);
        if (size == 0)
        {
            code = GetLastError();
            *err = minicmn_makeWindowsError(code, __FILE__, __LINE__);
            free(path);
            free(reparsed_path);
            CloseHandle(fh_exe);
            return -1;
        }
        if (size > reparsed_path_size)
        {
            free(reparsed_path);
            reparsed_path_size = size;
            reparsed_path = (wchar_t*)malloc(sizeof(wchar_t)*(reparsed_path_size+1));
            size = (*GetFinalPathNameByHandleW_Ptr)(fh_exe, reparsed_path, reparsed_path_size, 0);
            if (size == 0)
            {
                code = GetLastError();
                *err = minicmn_makeWindowsError(code, __FILE__, __LINE__);
                free(path);
                free(reparsed_path);
                CloseHandle(fh_exe);
                return -1;
            }
        }
        reparsed_path[size] = 0;
        CloseHandle(fh_exe);
        free(path);
        if (wcsncmp(L"\\\\?\\", reparsed_path, 4) == 0)
          path = &reparsed_path[4];
        else
          path = reparsed_path;
        *exe_path = sdsnew_utf8_from_utf16(path, wcslen(path));
        free(reparsed_path);
    }
    else
    {
        *exe_path = sdsnew_utf8_from_utf16(path, wcslen(path));
        free(path);
    }
    return 0;
}
