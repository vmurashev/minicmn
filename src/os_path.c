#include <minicmn/os_path.h>
#include <stdlib.h>

#ifdef _WIN32
static const char OS_SEP = '\\';
#else
static const char OS_SEP = '/';
#endif


sds minicmn_GetDirectoryName(const char* path)
{
    sds dirpath = NULL;
    size_t len = 0;
    size_t sep_pos = ~0;
    while (path[len])
    {
        len += 1;
        if (path[len - 1] == OS_SEP)
            sep_pos = len - 1;
    }
    if (sep_pos < len)
        dirpath = sdsnewlen(path, sep_pos);
    else
        dirpath = sdsnew(path);
    return dirpath;
}


sds minicmn_OsPathJoin(const char* path1, const char* path2)
{
    if (!path2[0])
    {
        return sdsnew(path1);
    }
    sds path = sdsnew(path1);
    size_t len = sdslen(path);
    if (len && path[len-1] != OS_SEP)
    {
        path = sdsgrowzero(path, len+1);
        path[len] = OS_SEP;
    }
    path = sdscat(path, path2);
    return path;
}
