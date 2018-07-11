/**
 *  @file   stacktrace_resolver_dladdr.hpp
 *  @brief Stack trace resolver based on DlAddr posix function. Do not provide 
 *         all the necessary information.
 *  @ingroup core
 *  @see https://gist.github.com/fmela/591333
 *
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *    Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#ifndef __FACEKIT_STACKTRACE_RESOLVER_DLADDR__
#define __FACEKIT_STACKTRACE_RESOLVER_DLADDR__

#include "facekit/core/library_export.hpp"
#include "facekit/core/sys/stacktrace_resolver.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
 *  @class  ResolverDlAddr
 *  @brief  Stack trace resolver based on DlAddr posix function
 *  @author Christophe Ecabert
 *  @date   10.07.18
 *  @ingroup core
 */
class FK_EXPORTS ResolverDlAddr : public StackTraceResolver {
 public:
  
  /**
   *  @name   ResolverDlAddr
   *  @fn     ResolverDlAddr(void) = default
   *  @brief  Constructor
   */
  ResolverDlAddr(void) = default;
  
  /**
   *  @name   ~ResolverDlAddr
   *  @fn     ~ResolverDlAddr(void) = default
   *  @brief  Destructor
   */
  ~ResolverDlAddr(void) = default;
  
  /**
   *  @name   Resolve
   *  @fn     Status Resolve(StackTrace* trace)
   *  @brief  Gather information for a given stack trace, using dladdr function
   *  @param[in, out] trace Stack trace to gather information for
   *  @return Status of the operation.
   */
  Status Resolve(StackTrace* trace);
  
};
  
}  // namespace FaceKit
#endif /* __FACEKIT_STACKTRACE_RESOLVER_DLADDR__ */
