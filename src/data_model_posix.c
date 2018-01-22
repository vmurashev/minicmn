#include <minicmn/data_model.h>
#include <minicmn/sds.h>
#include <string.h>

minicmn_Error* _minicmn_ErrorStructAlloc(int32_t err_code, sds err_text, sds code_file, uint32_t code_line);


static sds FormatPosixErrorMessage(int errnum)
{
  char* errmsg = strerror(errnum);
  if (errmsg)
  {
    return sdsnew(errmsg);
  }
  return sdscatprintf(sdsempty(), "errno=%d, No error message is available.", errnum);
}


minicmn_Error* minicmn_makePosixError(int32_t err_code, const char* code_file, uint32_t code_line)
{
    sds err_text = FormatPosixErrorMessage(err_code);
    return _minicmn_ErrorStructAlloc(err_code, err_text, sdsnew(code_file), code_line);
}
