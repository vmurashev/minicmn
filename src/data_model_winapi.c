#include <minicmn/data_model.h>
#include <minicmn/sds.h>

#include <windows.h>

minicmn_Error* _minicmn_ErrorStructAlloc(int32_t err_code, sds err_text, sds code_file, uint32_t code_line);


static sds FormatPlatformErrorMessage(int32_t code)
{
  HMODULE module = NULL;
  DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM;
  if ((code & FACILITY_NT_BIT) != 0)
  {
    code = (code & ~FACILITY_NT_BIT);
    module = GetModuleHandle("NTDLL.DLL");
    flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE;
  }

  WCHAR* buffer = NULL;
  DWORD len = FormatMessageW(flags, module, code, LANG_SYSTEM_DEFAULT, (WCHAR*)(&buffer), 0, 0);
  while (len > 0)
  {
    wchar_t sym = buffer[len - 1];
    if ((sym != '\n') && (sym != '\r'))
      break;
    len -= 1;
  }
  if (len > 0)
  {
    sds text = sdsnew_utf8_from_utf16(buffer, len);
    LocalFree(buffer);
    return text;
  }
  return sdsnew("No error message is available");
}


minicmn_Error* minicmn_makeWindowsError(int32_t err_code, const char* code_file, uint32_t code_line)
{
    sds err_text = FormatPlatformErrorMessage(err_code);
    return _minicmn_ErrorStructAlloc(err_code, err_text, sdsnew(code_file), code_line);
}
