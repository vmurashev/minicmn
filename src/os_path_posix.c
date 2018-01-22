#include <minicmn/os_path.h>
#include <unistd.h>
#include <errno.h>

#if defined(__linux__)
#  include <linux/limits.h>
#  define MY_MAX_PATH PATH_MAX
#elif defined(__APPLE__)
#  include <libproc.h>
#  define MY_MAX_PATH PROC_PIDPATHINFO_MAXSIZE
#endif

#ifdef __linux__
int minicmn_GetFullPathOfExecutable(sds* exe_path, minicmn_Error** err)
{
    sds path = sdsnewlen(NULL, MY_MAX_PATH + 1);
    ssize_t n = readlink("/proc/self/exe", path, MY_MAX_PATH);
    if (n < 0)
    {
        int32_t err_code = errno;
        *err = minicmn_makePosixError(err_code, __FILE__, __LINE__);
        sdsfree(path);
        return -1;
    }
    sdssetlen(path, n);
    *exe_path = path;
    return 0;
}
#endif

#ifdef __APPLE__
int minicmn_GetFullPathOfExecutable(sds* exe_path, minicmn_Error** err)
{
    sds path = sdsnewlen(NULL, MY_MAX_PATH + 1);
    int n = proc_pidpath(getpid(), path, MY_MAX_PATH);
    if (n <= 0)
    {
        int32_t err_code = errno;
        char* err_text = dlerror();
        *err = minicmn_makeErrorWithText(err_code, __FILE__, __LINE__);
        sdsfree(path);
        return -1;
    }
    sdssetlen(path, n);
    *exe_path = path;
    return 0;
}
#endif
