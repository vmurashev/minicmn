#include <minicmn/shlib.h>
#include <minicmn/sds.h>
#include <windows.h>

static void minicmn_FreeLibrary(void* ptr)
{
    FreeLibrary(ptr);
}

int minicmn_LoadLibrary(const char* libpath, minicmn_SharedLibrary** shlib, minicmn_Error** err)
{
    wchar_t* libpathW = (wchar_t*)new_utf16(libpath, strlen(libpath));
    HANDLE hLib = LoadLibraryW(libpathW);
    if (hLib == NULL)
    {
        DWORD err_code = GetLastError();
        free(libpathW);
        *err = minicmn_makeWindowsError(err_code, __FILE__, __LINE__);
        return -1;
    }
    minicmn_SharedLibrary* shlib_object = malloc(sizeof(minicmn_SharedLibrary));
    memset(shlib_object, 0, sizeof(minicmn_SharedLibrary));
    shlib_object->Reserved0 = minicmn_FreeLibrary;
    shlib_object->Reserved1 = hLib;
    *shlib = shlib_object;
    return 0;
}

void* minicmn_GetProcAddress(minicmn_SharedLibrary* shlib, const char* procname)
{
    return GetProcAddress((HMODULE)shlib->Reserved1, procname);
}
