/**
 *  @file   stacktrace_resolver_windows.cpp
 *  @brief Stack trace resolver for windows platform
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#if !defined(__APPLE__) && !defined(__linux__)
#include <windows.h>
#include <intrin.h>
#include <dbghelp.h>
#define IS_WINDOWS
#endif

#include <mutex>
#include <map>
#include <cassert>

#include "facekit/core/sys/stacktrace_resolver_windows.hpp"
#include "facekit/core/logger.hpp"
#include "facekit/core/mem/memory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
// Locking mutex to guarantee single access to dbghelp.dll
static std::mutex mu;
  
#ifdef IS_WINDOWNS
  
/**
 *  @class  DbgHelpDll
 *  @brief  Utility class to handle access to `dbghelp.dll`
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  @ingroup core
 */
class DbgHelpDll {
 public:
  
#pragma mark Initialisation
  
  /**
   *  @name   DbgHelpDll
   *  @fn     DbgHelpDll(void)
   *  @brief  Constructor
   */
  DbgHelpDll(void) : module_handle_(nullptr),
                     sym_set_opt_(nullptr),
                     sym_init_(nullptr),
                     get_sym_from_addr64_(nullptr),
                     get_line_from_addr64_(nullptr),
                     stack_walk64_(nullptr),
                     clean_up_(nullptr),
                     fcn_table_access64_(nullptr),
                     get_module_base64_(nullptr),
                     process_(nullptr) {}
  
  /**
   *  @name   ~DbgHelpDll
   *  @fn     ~DbgHelpDll(void)
   *  @brief  Destructor
   */
  ~DbgHelpDll(void) {
    if (IsLoaded()) {
      (*clean_up_)(process_);
      FreeLibrary(module_handle_);
    }
  }
  
  /**
   *  @name   Init
   *  @fn     int Init(void)
   *  @brief  Ensure the dll is loaded.  Return 0 on success and a non-zero
   *          value otherwise.
   */
  int Init(void) {
    return module_handle_ == 0 ? Load() : 0;
  }
  
#pragma mark Usage
  
  /**
   *  @name   SetOptions
   *  @fn     DWORD SetOptions(DWORD opt)
   *  @brief  Invoke the 'SymSetOptions' function of 'dbghelp.dll' with the
   *          specified `opt`, and return the result.
   */
  DWORD SetOptions(DWORD opt);
  
  /**
   *  @name   GetSymFromAddr64
   *  @fn     BOOL GetSymFromAddr64(DWORD64 address,
                                     PDWORD64 displacement,
                                     PIMAGEHLP_SYMBOL64 symbol)
   *  @brief  Invoke the 'SymFromAddr64' function of 'dbghelp.dll' with the
   *          pecified `address`, `displacement`, and `symbol`, and with an
   *          (internally generated) handle for the current Windows process, and
   *          return the result.
   */
  BOOL GetSymFromAddr64(DWORD64 address,
                        PDWORD64 displacement,
                        PIMAGEHLP_SYMBOL64 symbol);
  
  /**
   *  @name   GetLineFromAddr64
   *  @fn     BOOL GetLineFromAddr64(DWORD64 dwAddr,
                                     PDWORD pdwDisplacement,
                                     PIMAGEHLP_LINE64 line)
   *  @brief  Invoke the 'SymGetLineFromAddr64' function of 'dbghelp.dll' with
   *          the specified `dwAddr`, `pdwDisplacement`, and `line`, and with an
   *          (internally generated) handle for the current Windows process, and
   *          return the result.
   */
  BOOL GetLineFromAddr64(DWORD64 dwAddr,
                         PDWORD pdwDisplacement,
                         PIMAGEHLP_LINE64 line);
  
  /**
   *  @name   StackWalk64
   *  @fn     BOOL StackWalk64(DWORD machineType,
                               HANDLE hThread,
                               LPSTACKFRAME64 stackFrame,
                               PVOID contextRecord);
   *  @brief  Invoke the 'StackWalk64' function of 'dbghelp.dll' with the
   *          specified `machineType`, `hThread`, `stackFrame`, and 
   *          `contextRecord`, and with internally set handlers for remaining 
   *          arguments, and return the result.
   */
  BOOL StackWalk64(DWORD machineType,
                   HANDLE hThread,
                   LPSTACKFRAME64 stackFrame,
                   PVOID contextRecord);
  
  
  /**
   *  @name   IsLoaded
   *  @fn     bool IsLoaded() const
   *  @brief  Everything is fully loaded.
   */
  bool IsLoaded() const;
  
#pragma mark Private
 private:
  
  /**
   *  @name   Load
   *  @fn     int Load(void)
   *  @brief  Load the dll and set all the function pointers in this class to
   *          point into it. Return 0 on success and a non-zero value otherwise.
   */
  int Load(void);
  
  /**
   *  @name   Clean
   *  @fn     void Clean()
   *  @brief  Null out all the pointers owned by this class.
   */
  void Clean();
  
  
  // Callback definitions
  typedef DWORD __stdcall SymSetOptionsProc(DWORD);
  typedef BOOL  __stdcall SymInitializeProc(HANDLE, PCSTR, BOOL);
  typedef BOOL  __stdcall SymGetSymFromAddr64Proc(HANDLE,
                                                  DWORD64,
                                                  PDWORD64,
                                                  PIMAGEHLP_SYMBOL64);
  typedef BOOL  __stdcall SymGetLineFromAddr64Proc(HANDLE,
                                                   DWORD64,
                                                   PDWORD,
                                                   PIMAGEHLP_LINE64);
  typedef BOOL  __stdcall StackWalk64Proc(DWORD,
                                          HANDLE,
                                          HANDLE,
                                          LPSTACKFRAME64,
                                          PVOID,
                                          PREAD_PROCESS_MEMORY_ROUTINE64,
                                          PFUNCTION_TABLE_ACCESS_ROUTINE64,
                                          PGET_MODULE_BASE_ROUTINE64,
                                          PTRANSLATE_ADDRESS_ROUTINE64);
  typedef BOOL  __stdcall SymCleanupProc(HANDLE);
  
  // Functions pointer
  
  /** handle of the DLL that we will load the functions from */
  HMODULE module_handle_;
  /** 'SymSetOptions' func */
  SymSetOptionsProc* sym_set_opt_;
  /** 'SymInitialize' func */
  SymInitializeProc* sym_init_;
  /** 'SymGetSymFromAddr64' func */
  SymGetSymFromAddr64Proc* get_sym_from_addr64_;
  /** 'SymGetLineFromAddr64' func */
  SymGetLineFromAddr64Proc* get_line_from_addr64_;
  /** 'StackWalk64' func */
  StackWalk64Proc* stack_walk64_;
  /** 'SymCleanup' func */
  SymCleanupProc* clean_up_;
  /** Pointer to the Windows 'SymFunctionTableAccess64' function. */
  PFUNCTION_TABLE_ACCESS_ROUTINE64 fcn_table_access64_;
  /** Pointer to the Windows 'SymGetModuleBase64' function. */
  PGET_MODULE_BASE_ROUTINE64 get_module_base64_;
  /** Process */
  HANDLE process_;
};
  
/*
 *  @name   Load
 *  @fn     int Load(void)
 *  @brief  Load the dll and set all the function pointers in this class to
 *          point into it. Return 0 on success and a non-zero value otherwise.
 */
int DbgHelpDll::Load(void) {
  module_handle_ = LoadLibraryA("dbghelp.dll");
  if (module_handle_ == 0) {
    FACEKIT_LOG_ERROR("Can not load `dbghelp.dll` library");
    return -1;
  }
  // Get function pointers
  sym_set_opt_ = (SymSetOptionsProc*)GetProcAddress(module_handle_,
                                                    "SymSetOptions");
  sym_init_ = (SymInitializeProc*)GetProcAddress(module_handle_,
                                                 "SymInitialize");
  get_sym_from_addr64_ = (SymGetSymFromAddr64Proc*)GetProcAddress(module_handle_,
                                                                  "SymGetSymFromAddr64");
  get_line_from_addr64_ = (SymGetLineFromAddr64Proc*)GetProcAddress(module_handle_,
                                                                    "SymGetLineFromAddr64");
  stack_walk64_ = (StackWalk64Proc*)GetProcAddress(module_handle_,
                                                   "StackWalk64");
  clean_up_ = (SymCleanupProc*)GetProcAddress(module_handle_,
                                              "SymCleanup");
  fcn_table_access64_ = (PFUNCTION_TABLE_ACCESS_ROUTINE64)GetProcAddress(module_handle_,
                                                                         "SymFunctionTableAccess64");
  get_module_base64_ = (PGET_MODULE_BASE_ROUTINE64)GetProcAddress(module_handle_,
                                                                  "SymGetModuleBase64");
  process_ =  GetCurrentProcess();

  // Check
  if (!IsLoaded()) {
    FACEKIT_LOG_ERROR("DbghelpDll loading failed");
    this->Clean();
    return -1;
  }
  // Thanks to SYMOPT_DEFERRED_LOADS no manual enumeration of libraries is
  // necessary, this method will only load what is actually required
  BOOL rc = (*sym_init_)(process_, 0, TRUE);
  if (!rc) {
    FACEKIT_LOG_ERROR("'SymInitialize' failed");
    Clean();
    return -1;
  }
}
  
/*
 *  @name   Clean
 *  @fn     void Clean()
 *  @brief  Null out all the pointers owned by this class.
 */
void DbgHelpDll::Clean() {
  if (module_handle_ != 0) {
    FreeLibrary(module_handle_);
  }
  module_handle_ = 0;
  sym_set_opt_ = 0;
  sym_init_ = 0;
  get_sym_from_addr64_ = 0;
  get_line_from_addr64_ = 0;
  stack_walk64_ = 0;
  clean_up_ = 0;
  fcn_table_access64_ = 0;
  get_module_base64_ = 0;
  process_ = 0;
}
  
/*
 *  @name   IsLoaded
 *  @fn     bool IsLoaded() const
 *  @brief  Everything is fully loaded.
 */
bool DbgHelpDll::IsLoaded() const {
  return ((module_handle_ != 0) &&
          (sym_set_opt_ != 0) &&
          (sym_init_ != 0) &&
          (get_sym_from_addr64_ != 0) &&
          (get_line_from_addr64_ != 0) &&
          (stack_walk64_ != 0) &&
          (clean_up_ != 0) &&
          (fcn_table_access64_ != 0) &&
          (get_module_base64_ != 0) &&
          (process_ != 0));
}
  
/*
 *  @name   SetOptions
 *  @fn     DWORD SetOptions(DWORD opt)
 *  @brief  Invoke the 'SymSetOptions' function of 'dbghelp.dll' with the
 *          specified `opt`, and return the result.
 */
DWORD DbgHelpDll::SetOptions(DWORD opt) {
  int rc = this->Init();
  assert(rc == 0);
  return (*sym_set_opt_)(opt);
}
  
/*
 *  @name   GetSymFromAddr64
 *  @fn     BOOL GetSymFromAddr64(DWORD64 address,
 PDWORD64 displacement,
 PIMAGEHLP_SYMBOL64 symbol)
 *  @brief  Invoke the 'SymFromAddr64' function of 'dbghelp.dll' with the
 *          pecified `address`, `displacement`, and `symbol`, and with an
 *          (internally generated) handle for the current Windows process, and
 *          return the result.
 */
BOOL DbgHelpDll::GetSymFromAddr64(DWORD64 address,
                                  PDWORD64 displacement,
                                  PIMAGEHLP_SYMBOL64 symbol) {
  int rc = this->Init();
  if (0 != rc) {
    return FALSE;
  }
  return (*get_sym_from_addr64_)(process_, address, displacement, symbol);
}
  
/*
 *  @name   GetLineFromAddr64
 *  @fn     BOOL GetLineFromAddr64(DWORD64 dwAddr,
                                   PDWORD pdwDisplacement,
                                   PIMAGEHLP_LINE64 line)
 *  @brief  Invoke the 'SymGetLineFromAddr64' function of 'dbghelp.dll' with
 *          the specified `dwAddr`, `pdwDisplacement`, and `line`, and with an
 *          (internally generated) handle for the current Windows process, and
 *          return the result.
 */
BOOL DbgHelpDll::GetLineFromAddr64(DWORD64 dwAddr,
                                   PDWORD pdwDisplacement,
                                   PIMAGEHLP_LINE64 line) {
  int rc = this->Init();
  if (0 != rc) {
    return FALSE;
  }
  return (*get_line_from_addr64_)(process_, dwAddr, pdwDisplacement, line);
}
  
/*
 *  @name   StackWalk64
 *  @fn     BOOL StackWalk64(DWORD machineType,
                             HANDLE hThread,
                             LPSTACKFRAME64 stackFrame,
                             PVOID contextRecord);
 *  @brief  Invoke the 'StackWalk64' function of 'dbghelp.dll' with the
 *          specified `machineType`, `hThread`, `stackFrame`, and
 *          `contextRecord`, and with internally set handlers for remaining
 *          arguments, and return the result.
 */
BOOL DbgHelpDll::StackWalk64(DWORD machineType,
                             HANDLE hThread,
                             LPSTACKFRAME64 stackFrame,
                             PVOID contextRecord) {
  int rc = this->Init();
  if (0 != rc) {
    return FALSE;
  }
  return (*stack_walk64_)(machineType,
                          process_,
                          hThread,stackFrame,
                          contextRecord,
                          0,
                          fcn_table_access64_,
                          get_module_base64_,
                          0);
}
  
// Static dll accessor
static DbgHelpDll dbg_help_dll;
  
#endif
  
  
/*
 *  @name   Resolve
 *  @fn     Status Resolve(StackTrace* trace)
 *  @brief  Gather information for a given stack trace
 *  @param[in, out] trace Stack trace to gather information for
 *  @return Status of the operation.
 */
Status ResolverWindows::Resolve(StackTrace* trace) {
#ifdef IS_WINDOWS
  // Set up the symbol options so that we can gather information from the
  // current executable's PDB files, as well as the Microsoft symbol servers.
  // We also want to undecorate the symbol names we're returned.  If you want,
  // you can add other symbol servers or paths via a semi-colon separated list
  // in SymInitialized.
  std::lock_guard<std::mutex> lock(mu);
  
  dbg_help_dll.SetOptions(SYMOPT_NO_PROMPTS |
                          SYMOPT_LOAD_LINES |
                          SYMOPT_DEFERRED_LOADS);
  
  
  size_t sz = sizeof(IMAGEHLP_SYMBOL64) + 2000 * sizeof(TCHAR);
  IMAGEHLP_SYMBOL64 *sym = (IMAGEHLP_SYMBOL64*)Mem::Malloc(sz);
  char* lib_name_arr = (char*)Mem::Malloc(MAX_PATH);
  Mem::ScopedBuffer buffer(sym);
  Mem::ScopedBuffer lib_name_buffer(lib_name_arr);
  
  // Dictionary for library name
  std::map<HMODULE, const char*> libNameMap;
  
  
  size_t n_frames = trace->Size();
  for (size_t k = 0; k < n_frames; ++k) {
    // Get address of the frame
    auto& f = trace->At(k);
    DWORD64 address = (DWORD64) frame->address();
    
    // Query line number + source file
    IMAGEHLP_LINE64 line;
    ZeroMemory(&line, sizeof(IMAGEHLP_LINE64));
    line.SizeOfStruct = sizeof(line);
    DWORD offsetFromLine;
    int rc = dbg_help_dll.GetLineFromAddr64(address, &offsetFromLine, &line);
    if (rc) {
      f.set_src_file_name(line.FileName);
      f.set_line_number(line.LineNumber);
    }
    // Get symbol + offset
    DWORD64 offsetFromSymbol = 0;
    ZeroMemory(sym, SIZEOF_SEGMENT);
    sym->SizeOfStruct = sizeof(*sym);
    sym->MaxNameLength = 2000;
    rc = dbg_help_dll.GetSymFromAddr64(address,
                                       &offsetFromSymbol,
                                       sym);
    if (rc) {
      // windows is always demangled
      ((TCHAR *) sym)[sz - 1] = 0;
      f.set_mangled_symbol_name(sym->Name);
      f.set_symbol_name(f.get_mangled_symbol_name());
      f.set_offset(offsetFromSymbol);
    }
    
    // Get library name
    HMODULE hModule = NULL;
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery((LPCVOID) address, &mbi, sizeof(mbi))) {
      hModule = (HMODULE)(mbi.AllocationBase);
    }
    
    auto it = libNameMap.find(hModule);
    if (libNameMap.end() != it) {
      // If the library name in the map is "", leave the library file
      // name in the frame null.
      if (it->second) {
        f.set_library_name(it->second);
      }
    } else {
      // Not known
      rc = GetModuleFileNameA(hModule,
                              lib_name_arr,
                              MAX_PATH);
      lib_name_arr[MAX_PATH-1] = 0;
      if (!rc) {
        // Failed.  Put a null lib name into the map so we won't waste
        // time looking up the same library again.  Leave the
        // libraryFileName in the frame null.
        libNameMap[hModule] = "";
      } else {
        f.set_library_name(lib_name_arr);
        libNameMap[hModule] = f.get_library_name().c_str();
      }
    }
  }
  return Status();
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}
  
// Register
REGISTER_STACKTRACE_RESOLVER_SYSTEM("Windows", ResolverWindows);
  
}  // namespace FaceKit


