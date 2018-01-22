#include <minicmn/shlib.h>
#include <minicmn/sds.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

static void minicmn_FreeLibrary(void* ptr)
{
    dlclose(ptr);
}

int minicmn_LoadLibrary(const char* libpath, minicmn_SharedLibrary** shlib, minicmn_Error** err)
{
    void* hLib = dlopen(libpath, RTLD_LAZY);
    if (hLib == NULL)
    {
        const char* err_text = dlerror();
        *err = minicmn_makeLogicError(err_text, __FILE__, __LINE__);
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
    return dlsym(shlib->Reserved1, procname);
}
