/**
 *  @file   stacktrace_resolver_dladdr.cpp
 *  @brief Stack trace resolver based on DlAddr posix function. Do not provide
 *         all the necessary information.
 *  @ingroup core
 *  @see https://gist.github.com/fmela/591333
 *
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#if defined(__APPLE__) || defined(__linux__)
#include <dlfcn.h>    // for dladdr
#include <cxxabi.h>   // for __cxa_demangle
#define IS_POSIX
#endif

#include "facekit/core/sys/stacktrace_resolver_dladdr.hpp"
#include "facekit/core/mem/memory.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/*
 *  @name   Resolve
 *  @fn     Status Resolve(StackTrace* trace)
 *  @brief  Gather information for a given stack trace, using dladdr function
 *  @param[in, out] trace Stack trace to gather information for
 *  @return Status of the operation.
 */
Status ResolverDlAddr::Resolve(StackTrace* trace) {
#ifdef IS_POSIX
  if (trace->IsTraceValid()) {
    Status s;
    // Goes through the stacks
    for (size_t k = 0; k < trace->Size(); ++k) {
      // Access element
      auto& frame = trace->At(k);
      // Query info for this frame
      Dl_info info;
      std::memset(&info, 0, sizeof(info));
      
      // Ignore the status returned by 'dladdr' -- it returns 0 on failure, and
      // doesn't set errno, and returns 0 sometimes when it succeeds.
      dladdr(frame.get_address(), &info);
      
      if (!info.dli_fname) {
        info.dli_fname = "";
      }
      if (!info.dli_sname) {
        info.dli_sname = "";
      }
      
      frame.set_library_name(info.dli_fname);
      frame.set_mangled_symbol_name(info.dli_sname);
      size_t offset = (size_t)frame.get_address() - (size_t)info.dli_saddr;
      frame.set_offset(offset);
      // Unmangle name, if possible
      int rc = 0;
      char* demangled = abi::__cxa_demangle(info.dli_sname,
                                            nullptr,
                                            nullptr,
                                            &rc);
      Mem::ScopedBuffer buffer(demangled);
      frame.set_symbol_name(rc == 0 ? demangled : "");
      
      if (frame.get_symbol_name().empty()) {
        // Either demangling was turned off, demangling just failed, or it was
        // a static symbol.  For some reason, on Darwin, the demangler reduces
        // static symbols to nothing.  If that happened, just use the mangled
        // symbol name.
        frame.set_symbol_name(frame.get_mangled_symbol_name());
      }
    }
    return s;
  } else {
    return  Status(Status::Type::kInternalError, "Can not retrieve callstack");
  }
#else
  return Status(Status::Type::kUnimplemented, "Not supported");
#endif
}

// Register to factory
REGISTER_STACKTRACE_RESOLVER_SYSTEM("DlAddr", ResolverDlAddr);
  
}  // namespace FaceKit
