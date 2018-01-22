#include <minicmn/data_model.h>
#include <minicmn/sds.h>
#include <stdlib.h>
#include <string.h>

typedef struct _minicmn_DtorHeader
{
  void (*dtor)(void*);
  void* impl;
} minicmn_DtorHeader;


void minicmn_DataFree(void* ptr)
{
    if (ptr)
    {
        minicmn_DtorHeader* h = (minicmn_DtorHeader*)ptr;
        if (h->impl)
        {
            if (h->dtor)
                h->dtor(h->impl);
        }
        free(ptr);
    }
}

static void minicmn_ErrorStructFree(void* ptr)
{
    minicmn_Error* err = (minicmn_Error*)(ptr);
    sdsfree((sds)err->CodeFile);
    sdsfree((sds)err->ErrorText);
}

minicmn_Error* _minicmn_ErrorStructAlloc(int32_t err_code, sds err_text, sds code_file, uint32_t code_line)
{
    minicmn_Error* err = (minicmn_Error*)malloc(sizeof(minicmn_Error));
    memset(err, 0, sizeof(minicmn_Error));
    err->Reserved0 = minicmn_ErrorStructFree;
    err->Reserved1 = err;
    err->ErrorCode = err_code;
    err->CodeFile = code_file;
    err->CodeLine = code_line;
    err->ErrorText = err_text;
    return err;
}

minicmn_Error* minicmn_makeLogicError(const char* err_text, const char* code_file, uint32_t code_line)
{
    return _minicmn_ErrorStructAlloc(~0, sdsnew(err_text), sdsnew(code_file), code_line);
}
